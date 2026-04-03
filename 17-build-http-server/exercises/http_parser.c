/*
 * http_parser.c — Parse HTTP Requests and Build Responses
 *
 * Module 17: Build an HTTP Server
 *
 * HTTP is just structured text. In this exercise you'll write functions
 * to parse that text into usable data, and to build response text from
 * structured data.
 *
 * Compile: gcc -std=c99 -Wall -Wextra -pedantic -g -o http_parser http_parser.c
 * Run:     ./http_parser
 */

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

#define MAX_METHOD_LEN    16
#define MAX_PATH_LEN      1024
#define MAX_VERSION_LEN   16
#define MAX_HEADER_KEY    128
#define MAX_HEADER_VAL    512
#define MAX_HEADERS       32
#define MAX_BODY_LEN      65536
#define MAX_RESPONSE_LEN  65536

/* =========================================================================
 * Data Structures
 * ========================================================================= */

typedef struct {
    char key[MAX_HEADER_KEY];
    char value[MAX_HEADER_VAL];
} http_header_t;

typedef struct {
    char method[MAX_METHOD_LEN];       /* GET, POST, etc. */
    char path[MAX_PATH_LEN];           /* /index.html */
    char version[MAX_VERSION_LEN];     /* HTTP/1.1 */
    http_header_t headers[MAX_HEADERS];
    int header_count;
    char body[MAX_BODY_LEN];
    int body_len;
} http_request_t;

/* =========================================================================
 * Function Declarations
 * ========================================================================= */

int parse_request_line(const char *raw, char *method, char *path, char *version);
int parse_header(const char *line, char *key, char *value);
int parse_request(const char *raw, http_request_t *req);
const char *get_mime_type(const char *file_path);
int build_response(int status_code, const char *content_type,
                   const char *body, int body_len, char *output_buf);

/* =========================================================================
 * parse_request_line — Parse "GET /path HTTP/1.1" into parts
 *
 * Parameters:
 *   raw     — the request line string (without trailing \r\n)
 *   method  — output buffer for method (at least MAX_METHOD_LEN bytes)
 *   path    — output buffer for path (at least MAX_PATH_LEN bytes)
 *   version — output buffer for version (at least MAX_VERSION_LEN bytes)
 *
 * Returns: 0 on success, -1 on error
 *
 * Example:
 *   Input:  "GET /index.html HTTP/1.1"
 *   Output: method="GET", path="/index.html", version="HTTP/1.1"
 *
 * Hint: Use sscanf with %s to split on whitespace, or find spaces manually.
 *       sscanf(raw, "%s %s %s", method, path, version) does most of the work,
 *       but you need to validate that all three were parsed.
 * ========================================================================= */
int parse_request_line(const char *raw, char *method, char *path, char *version) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Check that raw is not NULL
     * 2. Use sscanf to extract method, path, and version
     * 3. Verify all three fields were parsed (sscanf returns count of matches)
     * 4. If path is empty or just whitespace, return -1
     * 5. Return 0 on success, -1 on failure
     */

    (void)raw;
    (void)method;
    (void)path;
    (void)version;
    return -1; /* Replace with your implementation */
}

/* =========================================================================
 * parse_header — Parse "Key: Value" into separate key and value strings
 *
 * Parameters:
 *   line  — one header line (without trailing \r\n)
 *   key   — output buffer (at least MAX_HEADER_KEY bytes)
 *   value — output buffer (at least MAX_HEADER_VAL bytes)
 *
 * Returns: 0 on success, -1 on error
 *
 * Example:
 *   Input:  "Content-Type: text/html"
 *   Output: key="Content-Type", value="text/html"
 *
 * Hint: Find the ':' character with strchr(). The value starts after ": "
 *       (colon + space). Copy everything before ':' into key, and
 *       everything after ": " into value.
 * ========================================================================= */
int parse_header(const char *line, char *key, char *value) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Find the ':' character using strchr
     * 2. If no ':' found, return -1
     * 3. Copy everything before ':' into key (use strncpy or memcpy)
     * 4. Skip the ':' and any leading spaces in the value part
     * 5. Copy the rest into value
     * 6. Null-terminate both strings
     * 7. Return 0
     */

    (void)line;
    (void)key;
    (void)value;
    return -1; /* Replace with your implementation */
}

/* =========================================================================
 * parse_request — Parse a full HTTP request into a struct
 *
 * Parameters:
 *   raw — the complete raw HTTP request text
 *   req — pointer to http_request_t struct to fill
 *
 * Returns: 0 on success, -1 on error
 *
 * A full request looks like:
 *   "GET /index.html HTTP/1.1\r\nHost: localhost\r\nAccept: text/html\r\n\r\n"
 *
 * Hint: Process line by line. Each line ends with CRLF (\r\n).
 *       The first line is the request line.
 *       Subsequent lines until the blank line (double CRLF) are headers.
 *       Anything after the blank line is the body.
 *
 *       Use strstr to find "\r\n" for line boundaries.
 * ========================================================================= */
int parse_request(const char *raw, http_request_t *req) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Initialize req (memset to 0)
     * 2. Find the first \r\n — everything before it is the request line
     * 3. Call parse_request_line on the request line
     * 4. Move past the first \r\n
     * 5. Loop: find next \r\n, extract the line
     *    - If the line is empty (we hit \r\n\r\n), break — headers are done
     *    - Otherwise, call parse_header and store in req->headers[]
     *    - Increment req->header_count
     * 6. If there's data after \r\n\r\n, copy it to req->body
     * 7. If path is "/", change it to "/index.html" (default document)
     * 8. Return 0 on success, -1 on parse error
     */

    (void)raw;
    (void)req;
    return -1; /* Replace with your implementation */
}

/* =========================================================================
 * get_mime_type — Return the MIME type string for a given file path
 *
 * Parameters:
 *   file_path — path or filename (e.g., "style.css" or "/www/style.css")
 *
 * Returns: MIME type string (e.g., "text/css")
 *
 * Mapping:
 *   .html  -> text/html
 *   .css   -> text/css
 *   .js    -> application/javascript
 *   .png   -> image/png
 *   .jpg   -> image/jpeg
 *   .jpeg  -> image/jpeg
 *   .gif   -> image/gif
 *   .txt   -> text/plain
 *   .json  -> application/json
 *   other  -> application/octet-stream
 *
 * Hint: Find the last '.' in the filename with strrchr('.').
 *       Then use strcmp to match against known extensions.
 * ========================================================================= */
const char *get_mime_type(const char *file_path) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Find the last '.' in file_path using strrchr
     * 2. If no '.' found, return "application/octet-stream"
     * 3. Compare the extension (including the dot) against known types
     *    using strcmp:
     *      ".html" -> "text/html"
     *      ".css"  -> "text/css"
     *      ".js"   -> "application/javascript"
     *      ... etc.
     * 4. If no match, return "application/octet-stream"
     */

    (void)file_path;
    return "application/octet-stream"; /* Replace with your implementation */
}

/* =========================================================================
 * build_response — Build a complete HTTP response string
 *
 * Parameters:
 *   status_code  — HTTP status code (200, 404, etc.)
 *   content_type — MIME type string (e.g., "text/html")
 *   body         — response body bytes
 *   body_len     — length of body in bytes
 *   output_buf   — output buffer (at least MAX_RESPONSE_LEN bytes)
 *
 * Returns: total length of the response written to output_buf
 *
 * The response format is:
 *   HTTP/1.1 <code> <reason>\r\n
 *   Content-Type: <type>\r\n
 *   Content-Length: <len>\r\n
 *   Connection: close\r\n
 *   \r\n
 *   <body bytes>
 *
 * Hint: Use snprintf to build the header part. Then memcpy the body
 *       after the headers (body might contain binary data, so don't
 *       use string functions on it).
 * ========================================================================= */
int build_response(int status_code, const char *content_type,
                   const char *body, int body_len, char *output_buf) {
    /* TODO: Implement this function
     *
     * Steps:
     * 1. Determine the reason phrase from the status code:
     *      200 -> "OK"
     *      400 -> "Bad Request"
     *      403 -> "Forbidden"
     *      404 -> "Not Found"
     *      500 -> "Internal Server Error"
     * 2. Use snprintf to write the status line and headers into output_buf:
     *      "HTTP/1.1 %d %s\r\n"
     *      "Content-Type: %s\r\n"
     *      "Content-Length: %d\r\n"
     *      "Connection: close\r\n"
     *      "\r\n"
     * 3. The snprintf return value tells you how many header bytes were written
     * 4. Use memcpy to copy the body after the headers
     * 5. Return total length (header_len + body_len)
     */

    (void)status_code;
    (void)content_type;
    (void)body;
    (void)body_len;
    (void)output_buf;
    return 0; /* Replace with your implementation */
}

/* =========================================================================
 * Helper: get reason phrase for a status code
 * (You can use this in build_response)
 * ========================================================================= */
const char *get_reason_phrase(int status_code) {
    switch (status_code) {
    case 200: return "OK";
    case 400: return "Bad Request";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 500: return "Internal Server Error";
    default:  return "Unknown";
    }
}

/* =========================================================================
 * Tests
 * ========================================================================= */

void run_tests(void) {
    printf("Running HTTP Parser Tests\n");
    printf("=========================\n\n");

    /* --- Test 1: Parse GET request line --- */
    {
        char method[MAX_METHOD_LEN], path[MAX_PATH_LEN], version[MAX_VERSION_LEN];
        int rc = parse_request_line("GET /index.html HTTP/1.1", method, path, version);
        TEST("Parse GET request line")
        EXPECT(rc == 0
               && strcmp(method, "GET") == 0
               && strcmp(path, "/index.html") == 0
               && strcmp(version, "HTTP/1.1") == 0);
    }

    /* --- Test 2: Parse POST request line --- */
    {
        char method[MAX_METHOD_LEN], path[MAX_PATH_LEN], version[MAX_VERSION_LEN];
        int rc = parse_request_line("POST /api/data HTTP/1.1", method, path, version);
        TEST("Parse POST request line")
        EXPECT(rc == 0
               && strcmp(method, "POST") == 0
               && strcmp(path, "/api/data") == 0);
    }

    /* --- Test 3: Parse header line --- */
    {
        char key[MAX_HEADER_KEY], value[MAX_HEADER_VAL];
        int rc = parse_header("Content-Type: text/html", key, value);
        TEST("Parse Content-Type header")
        EXPECT(rc == 0
               && strcmp(key, "Content-Type") == 0
               && strcmp(value, "text/html") == 0);
    }

    /* --- Test 4: Parse Host header --- */
    {
        char key[MAX_HEADER_KEY], value[MAX_HEADER_VAL];
        int rc = parse_header("Host: localhost:8080", key, value);
        TEST("Parse Host header")
        EXPECT(rc == 0
               && strcmp(key, "Host") == 0
               && strcmp(value, "localhost:8080") == 0);
    }

    /* --- Test 5: Reject malformed header (no colon) --- */
    {
        char key[MAX_HEADER_KEY], value[MAX_HEADER_VAL];
        int rc = parse_header("InvalidHeader", key, value);
        TEST("Reject header with no colon")
        EXPECT(rc == -1);
    }

    /* --- Test 6: Parse full GET request --- */
    {
        const char *raw =
            "GET /page.html HTTP/1.1\r\n"
            "Host: localhost:8080\r\n"
            "Accept: text/html\r\n"
            "\r\n";
        http_request_t req;
        int rc = parse_request(raw, &req);
        TEST("Parse full GET request")
        EXPECT(rc == 0
               && strcmp(req.method, "GET") == 0
               && strcmp(req.path, "/page.html") == 0
               && req.header_count == 2);
    }

    /* --- Test 7: Parse POST request with body --- */
    {
        const char *raw =
            "POST /submit HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "hello=world";
        http_request_t req;
        int rc = parse_request(raw, &req);
        TEST("Parse POST request with body")
        EXPECT(rc == 0
               && strcmp(req.method, "POST") == 0
               && strcmp(req.path, "/submit") == 0
               && req.header_count == 2
               && strcmp(req.body, "hello=world") == 0);
    }

    /* --- Test 8: Empty path defaults to /index.html --- */
    {
        const char *raw =
            "GET / HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        http_request_t req;
        int rc = parse_request(raw, &req);
        TEST("Root path '/' defaults to /index.html")
        EXPECT(rc == 0 && strcmp(req.path, "/index.html") == 0);
    }

    /* --- Test 9: Malformed request (no method) --- */
    {
        const char *raw = "GARBAGE\r\n\r\n";
        http_request_t req;
        int rc = parse_request(raw, &req);
        TEST("Reject malformed request (missing parts)")
        EXPECT(rc == -1);
    }

    /* --- Test 10: Parse path with query string --- */
    {
        char method[MAX_METHOD_LEN], path[MAX_PATH_LEN], version[MAX_VERSION_LEN];
        int rc = parse_request_line("GET /search?q=hello&lang=en HTTP/1.1",
                                    method, path, version);
        TEST("Parse path with query string")
        EXPECT(rc == 0 && strcmp(path, "/search?q=hello&lang=en") == 0);
    }

    /* --- Test 11: MIME type for .html --- */
    TEST("MIME type: .html -> text/html")
    EXPECT(strcmp(get_mime_type("index.html"), "text/html") == 0);

    /* --- Test 12: MIME type for .css --- */
    TEST("MIME type: .css -> text/css")
    EXPECT(strcmp(get_mime_type("/www/style.css"), "text/css") == 0);

    /* --- Test 13: MIME type for .js --- */
    TEST("MIME type: .js -> application/javascript")
    EXPECT(strcmp(get_mime_type("app.js"), "application/javascript") == 0);

    /* --- Test 14: MIME type for .png --- */
    TEST("MIME type: .png -> image/png")
    EXPECT(strcmp(get_mime_type("photo.png"), "image/png") == 0);

    /* --- Test 15: MIME type for unknown extension --- */
    TEST("MIME type: unknown -> application/octet-stream")
    EXPECT(strcmp(get_mime_type("data.xyz"), "application/octet-stream") == 0);

    /* --- Test 16: Build 200 OK response --- */
    {
        char buf[MAX_RESPONSE_LEN];
        const char *body = "<h1>Hello</h1>";
        int len = build_response(200, "text/html", body, (int)strlen(body), buf);
        TEST("Build 200 OK response")
        EXPECT(len > 0
               && strstr(buf, "HTTP/1.1 200 OK\r\n") != NULL
               && strstr(buf, "Content-Type: text/html\r\n") != NULL
               && strstr(buf, "Content-Length: 14\r\n") != NULL
               && strstr(buf, "<h1>Hello</h1>") != NULL);
    }

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

int main(void) {
    run_tests();
    return (tests_passed == tests_total) ? 0 : 1;
}
