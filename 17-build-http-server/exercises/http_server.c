/*
 * http_server.c — A Complete HTTP/1.0 Static File Server
 *
 * Module 17: Build an HTTP Server
 *
 * This is the real deal: a working web server that serves files from a
 * directory. It combines everything from Module 16 (sockets) with the
 * parsing logic from http_parser.c.
 *
 * Usage:  ./http_server [port]
 *         Default port is 8080. Serves files from ./www/
 *
 * Test:   curl http://localhost:8080/index.html
 *         Or open that URL in a browser.
 *
 * Compile: gcc -std=c99 -Wall -Wextra -pedantic -g -o http_server http_server.c
 * Run:     ./http_server 8080
 *
 * Sample www/ directory structure:
 *   www/
 *   +-- index.html      (main page)
 *   +-- style.css        (stylesheet)
 *   +-- about.html       (another page)
 *   +-- images/
 *       +-- logo.png
 */

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

/* =========================================================================
 * Test Harness
 * ========================================================================= */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

/* =========================================================================
 * Constants
 * ========================================================================= */

#define DEFAULT_PORT      8080
#define BACKLOG           10
#define MAX_REQUEST_SIZE  8192
#define MAX_PATH_LEN      1024
#define MAX_RESPONSE_LEN  65536
#define MAX_FILE_SIZE     (1024 * 1024)  /* 1 MB max file size */
#define WWW_ROOT          "www"

/* Global server socket for signal handler cleanup */
static volatile int server_fd = -1;
static volatile int running = 1;

/* =========================================================================
 * get_mime_type — Return MIME type for a file extension
 *
 * Same logic as in http_parser.c. We duplicate it here so this file
 * is self-contained. In a real project, you'd put it in a shared header.
 * ========================================================================= */
const char *get_mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (ext == NULL) return "application/octet-stream";
    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0)  return "text/css";
    if (strcmp(ext, ".js") == 0)   return "application/javascript";
    if (strcmp(ext, ".png") == 0)  return "image/png";
    if (strcmp(ext, ".jpg") == 0)  return "image/jpeg";
    if (strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0)  return "image/gif";
    if (strcmp(ext, ".txt") == 0)  return "text/plain";
    if (strcmp(ext, ".json") == 0) return "application/json";
    return "application/octet-stream";
}

/* =========================================================================
 * get_reason_phrase — Map status code to human-readable string
 * ========================================================================= */
const char *get_reason_phrase(int code) {
    switch (code) {
    case 200: return "OK";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 500: return "Internal Server Error";
    default:  return "Unknown";
    }
}

/* =========================================================================
 * signal_handler — Catch SIGINT (Ctrl+C) for clean shutdown
 *
 * When the user presses Ctrl+C, we want to:
 * 1. Stop the accept loop
 * 2. Close the server socket
 * 3. Print a shutdown message
 * ========================================================================= */
void signal_handler(int sig) {
    (void)sig;
    running = 0;
    if (server_fd != -1) {
        close(server_fd);
        server_fd = -1;
    }
    /* write is async-signal-safe, printf is NOT */
    const char msg[] = "\nShutting down server...\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

/* =========================================================================
 * is_path_safe — Check that a URL path doesn't escape www_root
 *
 * Returns: 1 if safe, 0 if dangerous
 *
 * Must reject paths containing ".." to prevent directory traversal.
 * ========================================================================= */
int is_path_safe(const char *path) {
    if (path == NULL) return 0;
    if (strstr(path, "..") != NULL) return 0;
    if (path[0] != '/') return 0;
    return 1;
}

/* =========================================================================
 * send_error — Send an HTTP error response
 *
 * Parameters:
 *   client_fd   — socket to write to
 *   status_code — HTTP status code (404, 400, etc.)
 *   message     — human-readable error message for the body
 *
 * Sends a complete HTTP response with an HTML error page.
 *
 * Hint: Use snprintf to build the body first, then build the full
 *       response (status line + headers + blank line + body), then
 *       write() it all to the socket.
 * ========================================================================= */
void send_error(int client_fd, int status_code, const char *message) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Build an HTML body string:
     *    "<html><body><h1>%d %s</h1><p>%s</p></body></html>"
     * 2. Build the full HTTP response:
     *    "HTTP/1.1 %d %s\r\n"
     *    "Content-Type: text/html\r\n"
     *    "Content-Length: %d\r\n"
     *    "Connection: close\r\n"
     *    "\r\n"
     *    "%s"   (the body)
     * 3. Write the response to client_fd using write()
     * 4. Log the error to stderr: fprintf(stderr, "%d %s: %s\n", ...)
     */

    (void)client_fd;
    (void)status_code;
    (void)message;
}

/* =========================================================================
 * serve_file — Read a file and send it as an HTTP response
 *
 * Parameters:
 *   client_fd — socket to write to
 *   filepath  — full filesystem path to the file
 *
 * Returns: 0 on success, -1 on error
 *
 * This function:
 * 1. Opens the file in binary mode ("rb")
 * 2. Gets the file size
 * 3. Reads the file into a buffer
 * 4. Determines the MIME type
 * 5. Builds and sends the HTTP response
 * ========================================================================= */
int serve_file(int client_fd, const char *filepath) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Open the file with fopen(filepath, "rb")
     *    - If fopen fails, call send_error(client_fd, 404, "File not found")
     *      and return -1
     *
     * 2. Get file size:
     *    fseek(fp, 0, SEEK_END);
     *    long file_size = ftell(fp);
     *    fseek(fp, 0, SEEK_SET);  // rewind
     *
     * 3. Check file_size <= MAX_FILE_SIZE
     *    - If too large, send 500 error, fclose, return -1
     *
     * 4. Allocate buffer: malloc(file_size)
     *    - If malloc fails, send 500 error, fclose, return -1
     *
     * 5. Read file: fread(buffer, 1, file_size, fp)
     *    - fclose(fp) after reading
     *
     * 6. Get MIME type: get_mime_type(filepath)
     *
     * 7. Build HTTP response headers using snprintf:
     *    "HTTP/1.1 200 OK\r\n"
     *    "Content-Type: %s\r\n"
     *    "Content-Length: %ld\r\n"
     *    "Connection: close\r\n"
     *    "\r\n"
     *
     * 8. Send headers: write(client_fd, header_buf, header_len)
     *
     * 9. Send body: write(client_fd, buffer, file_size)
     *
     * 10. Free buffer
     *
     * 11. Log: printf("[200] %s (%ld bytes, %s)\n", filepath, file_size, mime)
     *
     * 12. Return 0
     */

    (void)client_fd;
    (void)filepath;
    return -1; /* Replace with your implementation */
}

/* =========================================================================
 * serve_directory — List files in a directory as HTML links
 *
 * Parameters:
 *   client_fd — socket to write to
 *   dirpath   — filesystem path to the directory
 *   url_path  — the URL path (for building links)
 *
 * Returns: 0 on success, -1 on error
 * ========================================================================= */
int serve_directory(int client_fd, const char *dirpath, const char *url_path) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Open directory with opendir(dirpath)
     *    - If fails, send 404, return -1
     *
     * 2. Build HTML body with a list of files:
     *    Start with: "<html><body><h1>Index of %s</h1><ul>\n"
     *
     * 3. Loop with readdir():
     *    - Skip "." and ".."
     *    - For each entry, append: "<li><a href=\"%s/%s\">%s</a></li>\n"
     *      where the href is url_path + "/" + entry name
     *
     * 4. End with: "</ul></body></html>\n"
     *
     * 5. closedir()
     *
     * 6. Build and send HTTP response with Content-Type: text/html
     *
     * 7. Return 0
     */

    (void)client_fd;
    (void)dirpath;
    (void)url_path;
    return -1; /* Replace with your implementation */
}

/* =========================================================================
 * handle_client — Read request, parse it, serve the response
 *
 * Parameters:
 *   client_fd — connected client socket
 *   www_root  — path to the www directory
 *
 * This is the main request handling function. It:
 * 1. Reads the raw request from the socket
 * 2. Parses the method and path
 * 3. Maps the URL path to a filesystem path
 * 4. Serves the file or sends an error
 * ========================================================================= */
void handle_client(int client_fd, const char *www_root) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Read the request:
     *    char request[MAX_REQUEST_SIZE];
     *    ssize_t bytes_read = read(client_fd, request, sizeof(request) - 1);
     *    - If bytes_read <= 0, close and return
     *    - Null-terminate: request[bytes_read] = '\0';
     *
     * 2. Parse the request line (first line only — we need method and path):
     *    - Find the first \r\n or \n
     *    - Extract method and path using sscanf or manual parsing
     *    - Only handle GET requests; send 400 for others
     *
     * 3. Validate the path:
     *    - Call is_path_safe(path)
     *    - If unsafe, send 403 Forbidden
     *
     * 4. Default path: if path is "/", change to "/index.html"
     *
     * 5. Build filesystem path:
     *    snprintf(filepath, sizeof(filepath), "%s%s", www_root, path);
     *
     * 6. Check if it's a directory (use stat()):
     *    struct stat st;
     *    if (stat(filepath, &st) == 0 && S_ISDIR(st.st_mode))
     *        serve_directory(client_fd, filepath, path);
     *    else
     *        serve_file(client_fd, filepath);
     *
     * 7. Close client_fd
     */

    (void)client_fd;
    (void)www_root;
}

/* =========================================================================
 * create_server — Create, bind, and listen on a TCP socket
 *
 * Parameters:
 *   port — port number to listen on
 *
 * Returns: server socket fd on success, -1 on error
 *
 * This is the same socket setup from Module 16, wrapped in a function.
 * ========================================================================= */
int create_server(int port) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Create socket:
     *    int fd = socket(AF_INET, SOCK_STREAM, 0);
     *    - Check for error (fd == -1)
     *
     * 2. Set SO_REUSEADDR option (so we can restart quickly):
     *    int opt = 1;
     *    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
     *
     * 3. Set up address struct:
     *    struct sockaddr_in addr;
     *    memset(&addr, 0, sizeof(addr));
     *    addr.sin_family = AF_INET;
     *    addr.sin_addr.s_addr = INADDR_ANY;  // listen on all interfaces
     *    addr.sin_port = htons(port);
     *
     * 4. Bind:
     *    bind(fd, (struct sockaddr *)&addr, sizeof(addr))
     *    - Check for error, print with perror("bind")
     *
     * 5. Listen:
     *    listen(fd, BACKLOG)
     *    - Check for error
     *
     * 6. Return fd
     */

    (void)port;
    return -1; /* Replace with your implementation */
}

/* =========================================================================
 * reap_children — Clean up zombie child processes
 *
 * When we fork(), child processes become "zombies" after they exit unless
 * the parent calls wait(). This function reaps all finished children.
 * ========================================================================= */
void reap_children(void) {
    /* waitpid with WNOHANG returns immediately if no child has exited */
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        /* reaped a zombie */
    }
}

/* =========================================================================
 * run_server — Main server loop
 *
 * Parameters:
 *   port     — port to listen on
 *   www_root — directory to serve files from
 * ========================================================================= */
void run_server(int port, const char *www_root) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Install signal handler:
     *    signal(SIGINT, signal_handler);
     *
     * 2. Create server socket:
     *    server_fd = create_server(port);
     *    - If -1, print error and exit
     *
     * 3. Print startup message:
     *    printf("HTTP Server listening on port %d\n", port);
     *    printf("Serving files from: %s/\n", www_root);
     *    printf("Press Ctrl+C to stop.\n\n");
     *
     * 4. Main loop (while running):
     *    a. Reap zombie children: reap_children();
     *
     *    b. Accept connection:
     *       struct sockaddr_in client_addr;
     *       socklen_t client_len = sizeof(client_addr);
     *       int client_fd = accept(server_fd,
     *                              (struct sockaddr *)&client_addr,
     *                              &client_len);
     *       - If client_fd < 0:
     *           if running, perror and continue
     *           else break (shutting down)
     *
     *    c. Log connection:
     *       printf("Connection from %s:%d\n",
     *              inet_ntoa(client_addr.sin_addr),
     *              ntohs(client_addr.sin_port));
     *
     *    d. Fork:
     *       pid_t pid = fork();
     *       if (pid == 0) {
     *           // Child process
     *           close(server_fd);   // child doesn't need listener
     *           handle_client(client_fd, www_root);
     *           exit(0);
     *       } else if (pid > 0) {
     *           // Parent process
     *           close(client_fd);   // parent doesn't need this connection
     *       } else {
     *           perror("fork");
     *           close(client_fd);
     *       }
     *
     * 5. After loop: close server_fd if still open
     *    printf("Server stopped.\n");
     */

    (void)port;
    (void)www_root;
}

/* =========================================================================
 * Tests — Test helper functions (not the server loop)
 * ========================================================================= */

void run_tests(void) {
    printf("Running HTTP Server Tests\n");
    printf("=========================\n\n");

    /* --- Test 1: MIME type for .html --- */
    TEST("MIME type for .html")
    EXPECT(strcmp(get_mime_type("index.html"), "text/html") == 0);

    /* --- Test 2: MIME type for .css --- */
    TEST("MIME type for .css")
    EXPECT(strcmp(get_mime_type("style.css"), "text/css") == 0);

    /* --- Test 3: MIME type for .png --- */
    TEST("MIME type for .png")
    EXPECT(strcmp(get_mime_type("image.png"), "image/png") == 0);

    /* --- Test 4: MIME type for unknown --- */
    TEST("MIME type for unknown extension")
    EXPECT(strcmp(get_mime_type("file.xyz"), "application/octet-stream") == 0);

    /* --- Test 5: Path safety — normal path --- */
    TEST("Path safety: /index.html is safe")
    EXPECT(is_path_safe("/index.html") == 1);

    /* --- Test 6: Path safety — traversal attack --- */
    TEST("Path safety: /../etc/passwd is unsafe")
    EXPECT(is_path_safe("/../../../etc/passwd") == 0);

    /* --- Test 7: Path safety — embedded .. --- */
    TEST("Path safety: /images/../secret is unsafe")
    EXPECT(is_path_safe("/images/../secret") == 0);

    /* --- Test 8: Path safety — no leading slash --- */
    TEST("Path safety: relative path is unsafe")
    EXPECT(is_path_safe("etc/passwd") == 0);

    /* --- Test 9: Path safety — NULL --- */
    TEST("Path safety: NULL is unsafe")
    EXPECT(is_path_safe(NULL) == 0);

    /* --- Test 10: Reason phrase lookup --- */
    TEST("Reason phrase for common codes")
    EXPECT(strcmp(get_reason_phrase(200), "OK") == 0
           && strcmp(get_reason_phrase(404), "Not Found") == 0
           && strcmp(get_reason_phrase(400), "Bad Request") == 0
           && strcmp(get_reason_phrase(500), "Internal Server Error") == 0);

    printf("\n=========================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("All tests passed!\n");
    } else {
        printf("%d test(s) FAILED.\n", tests_total - tests_passed);
    }
}

/* =========================================================================
 * Main
 * ========================================================================= */

int main(int argc, char *argv[]) {
    /* If run with --test flag, run tests instead of starting the server */
    if (argc >= 2 && strcmp(argv[1], "--test") == 0) {
        run_tests();
        return (tests_passed == tests_total) ? 0 : 1;
    }

    /* Parse port from command line or use default */
    int port = DEFAULT_PORT;
    if (argc >= 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Usage: %s [port]\n", argv[0]);
            fprintf(stderr, "  port must be 1-65535 (default: %d)\n", DEFAULT_PORT);
            return 1;
        }
    }

    /* Check that www/ directory exists */
    struct stat st;
    if (stat(WWW_ROOT, &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: '%s' directory not found.\n", WWW_ROOT);
        fprintf(stderr, "Create it and add some HTML files:\n");
        fprintf(stderr, "  mkdir -p %s\n", WWW_ROOT);
        fprintf(stderr, "  echo '<h1>Hello</h1>' > %s/index.html\n", WWW_ROOT);
        return 1;
    }

    run_server(port, WWW_ROOT);
    return 0;
}
