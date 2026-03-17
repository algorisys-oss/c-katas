/*
 * Solution: HTTP Request/Response Parser
 *
 * Parse HTTP requests and responses as plain strings — no actual networking.
 * This is exactly what a real web server does internally: every request that
 * arrives over a socket is just a string of bytes that must be parsed.
 *
 * Functions:
 *   - http_parse_request  (extract method, path, version from request line)
 *   - http_parse_header   (extract a specific header's value)
 *   - http_build_response (construct a well-formed HTTP response string)
 *
 * 14 tests.
 */

#include <stdio.h>
#include <string.h>

#define MAX_METHOD   16
#define MAX_PATH     256
#define MAX_VERSION  16
#define MAX_HEADER   512
#define MAX_RESPONSE 4096

/* ── http_parse_request ──────────────────────────────────────────
 *
 * Parse the request line from a raw HTTP request string.
 *
 * Input:  "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n"
 * Output: method="GET", path="/index.html", version="HTTP/1.1"
 *
 * Returns 0 on success, -1 on parse failure.
 */
int http_parse_request(const char *raw, char *method, char *path, char *version)
{
    if (raw == NULL || method == NULL || path == NULL || version == NULL) {
        return -1;
    }

    /* Find first space (end of method) */
    const char *p = raw;
    const char *space1 = strchr(p, ' ');
    if (space1 == NULL) {
        return -1;
    }

    int method_len = (int)(space1 - p);
    if (method_len <= 0 || method_len >= MAX_METHOD) {
        return -1;
    }
    strncpy(method, p, method_len);
    method[method_len] = '\0';

    /* Find second space (end of path) */
    p = space1 + 1;
    const char *space2 = strchr(p, ' ');
    if (space2 == NULL) {
        return -1;
    }

    int path_len = (int)(space2 - p);
    if (path_len <= 0 || path_len >= MAX_PATH) {
        return -1;
    }
    strncpy(path, p, path_len);
    path[path_len] = '\0';

    /* Version ends at \r\n or end of string */
    p = space2 + 1;
    const char *eol = strstr(p, "\r\n");
    int version_len;
    if (eol != NULL) {
        version_len = (int)(eol - p);
    } else {
        version_len = (int)strlen(p);
    }
    if (version_len <= 0 || version_len >= MAX_VERSION) {
        return -1;
    }
    strncpy(version, p, version_len);
    version[version_len] = '\0';

    return 0;
}

/* ── http_parse_header ───────────────────────────────────────────
 *
 * Search a raw HTTP message for a header by name and copy its value
 * into out_value.
 *
 * Input:  raw = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n"
 *         name = "Host"
 * Output: out_value = "example.com"
 *
 * Returns 0 on success, -1 if header not found.
 * Header name comparison is case-insensitive.
 */
int http_parse_header(const char *raw, const char *name, char *out_value, int size)
{
    if (raw == NULL || name == NULL || out_value == NULL || size <= 0) {
        return -1;
    }

    int name_len = (int)strlen(name);

    /* Search line by line for "Name: value\r\n" */
    const char *p = raw;
    while (*p != '\0') {
        /* Find end of current line */
        const char *eol = strstr(p, "\r\n");
        if (eol == NULL) {
            break;
        }

        /* Check if line starts with "name:" (case-insensitive) */
        int line_len = (int)(eol - p);
        if (line_len > name_len + 1) {
            /* Case-insensitive comparison of header name */
            int match = 1;
            for (int i = 0; i < name_len; i++) {
                char a = p[i];
                char b = name[i];
                /* Simple ASCII case folding */
                if (a >= 'A' && a <= 'Z') a += 32;
                if (b >= 'A' && b <= 'Z') b += 32;
                if (a != b) {
                    match = 0;
                    break;
                }
            }

            if (match && p[name_len] == ':') {
                /* Skip colon and optional whitespace */
                const char *val_start = p + name_len + 1;
                while (val_start < eol && *val_start == ' ') {
                    val_start++;
                }

                int val_len = (int)(eol - val_start);
                if (val_len >= size) {
                    val_len = size - 1;
                }
                strncpy(out_value, val_start, val_len);
                out_value[val_len] = '\0';
                return 0;
            }
        }

        /* Move to next line */
        p = eol + 2;
    }

    return -1;
}

/* ── http_build_response ─────────────────────────────────────────
 *
 * Build an HTTP response string from the given parts.
 *
 * Example output:
 *   "HTTP/1.1 200 OK\r\n"
 *   "Content-Type: text/html\r\n"
 *   "Content-Length: 13\r\n"
 *   "\r\n"
 *   "Hello, World!"
 *
 * Returns the number of bytes written (excluding null terminator),
 * or -1 on error.
 */
int http_build_response(int status_code, const char *status_text,
                        const char *content_type, const char *body,
                        char *out_buf, int size)
{
    if (status_text == NULL || content_type == NULL || out_buf == NULL || size <= 0) {
        return -1;
    }

    int body_len = (body != NULL) ? (int)strlen(body) : 0;

    int written = snprintf(out_buf, size,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        status_code, status_text,
        content_type,
        body_len,
        (body != NULL) ? body : "");

    if (written < 0 || written >= size) {
        return -1;
    }

    return written;
}

/* ── Test Harness ────────────────────────────────────────────── */

static int tests_total = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-45s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== HTTP Parser ===\n\n");

    /* --- http_parse_request tests --- */

    printf("-- http_parse_request --\n");

    /* Test 1: Parse a simple GET request */
    {
        char method[MAX_METHOD], path[MAX_PATH], version[MAX_VERSION];
        const char *raw = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
        int rc = http_parse_request(raw, method, path, version);
        TEST("parse GET request");
        EXPECT(rc == 0 &&
               strcmp(method, "GET") == 0 &&
               strcmp(path, "/index.html") == 0 &&
               strcmp(version, "HTTP/1.1") == 0);
    }

    /* Test 2: Parse a POST request */
    {
        char method[MAX_METHOD], path[MAX_PATH], version[MAX_VERSION];
        const char *raw = "POST /api/data HTTP/1.1\r\nContent-Length: 11\r\n\r\nHello World";
        int rc = http_parse_request(raw, method, path, version);
        TEST("parse POST request");
        EXPECT(rc == 0 &&
               strcmp(method, "POST") == 0 &&
               strcmp(path, "/api/data") == 0 &&
               strcmp(version, "HTTP/1.1") == 0);
    }

    /* Test 3: Parse request with root path */
    {
        char method[MAX_METHOD], path[MAX_PATH], version[MAX_VERSION];
        const char *raw = "GET / HTTP/1.0\r\n\r\n";
        int rc = http_parse_request(raw, method, path, version);
        TEST("parse root path and HTTP/1.0");
        EXPECT(rc == 0 &&
               strcmp(method, "GET") == 0 &&
               strcmp(path, "/") == 0 &&
               strcmp(version, "HTTP/1.0") == 0);
    }

    /* Test 4: Parse DELETE request */
    {
        char method[MAX_METHOD], path[MAX_PATH], version[MAX_VERSION];
        const char *raw = "DELETE /users/42 HTTP/1.1\r\n\r\n";
        int rc = http_parse_request(raw, method, path, version);
        TEST("parse DELETE request");
        EXPECT(rc == 0 &&
               strcmp(method, "DELETE") == 0 &&
               strcmp(path, "/users/42") == 0);
    }

    /* Test 5: Reject malformed request (no spaces) */
    {
        char method[MAX_METHOD], path[MAX_PATH], version[MAX_VERSION];
        const char *raw = "GETHTTP1.1\r\n\r\n";
        int rc = http_parse_request(raw, method, path, version);
        TEST("reject malformed request (no spaces)");
        EXPECT(rc == -1);
    }

    /* Test 6: NULL input returns error */
    {
        char method[MAX_METHOD], path[MAX_PATH], version[MAX_VERSION];
        int rc = http_parse_request(NULL, method, path, version);
        TEST("NULL raw input returns -1");
        EXPECT(rc == -1);
    }

    printf("\n");

    /* --- http_parse_header tests --- */

    printf("-- http_parse_header --\n");

    /* Test 7: Extract Host header */
    {
        char value[MAX_HEADER];
        const char *raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\nAccept: text/html\r\n\r\n";
        int rc = http_parse_header(raw, "Host", value, MAX_HEADER);
        TEST("extract Host header");
        EXPECT(rc == 0 && strcmp(value, "www.example.com") == 0);
    }

    /* Test 8: Extract Content-Type header */
    {
        char value[MAX_HEADER];
        const char *raw = "POST /api HTTP/1.1\r\nHost: localhost\r\nContent-Type: application/json\r\n\r\n{}";
        int rc = http_parse_header(raw, "Content-Type", value, MAX_HEADER);
        TEST("extract Content-Type header");
        EXPECT(rc == 0 && strcmp(value, "application/json") == 0);
    }

    /* Test 9: Case-insensitive header match */
    {
        char value[MAX_HEADER];
        const char *raw = "GET / HTTP/1.1\r\ncontent-type: text/plain\r\n\r\n";
        int rc = http_parse_header(raw, "Content-Type", value, MAX_HEADER);
        TEST("case-insensitive header name");
        EXPECT(rc == 0 && strcmp(value, "text/plain") == 0);
    }

    /* Test 10: Header not found */
    {
        char value[MAX_HEADER];
        const char *raw = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
        int rc = http_parse_header(raw, "Authorization", value, MAX_HEADER);
        TEST("header not found returns -1");
        EXPECT(rc == -1);
    }

    /* Test 11: Extract Content-Length with numeric value */
    {
        char value[MAX_HEADER];
        const char *raw = "POST /data HTTP/1.1\r\nContent-Length: 1024\r\n\r\n";
        int rc = http_parse_header(raw, "Content-Length", value, MAX_HEADER);
        TEST("extract Content-Length header");
        EXPECT(rc == 0 && strcmp(value, "1024") == 0);
    }

    printf("\n");

    /* --- http_build_response tests --- */

    printf("-- http_build_response --\n");

    /* Test 12: Build a 200 OK response */
    {
        char buf[MAX_RESPONSE];
        int n = http_build_response(200, "OK", "text/html", "<h1>Hello</h1>", buf, MAX_RESPONSE);
        TEST("build 200 OK response");
        EXPECT(n > 0 &&
               strstr(buf, "HTTP/1.1 200 OK\r\n") == buf &&
               strstr(buf, "Content-Type: text/html\r\n") != NULL &&
               strstr(buf, "Content-Length: 14\r\n") != NULL &&
               strstr(buf, "\r\n\r\n<h1>Hello</h1>") != NULL);
    }

    /* Test 13: Build a 404 Not Found response */
    {
        char buf[MAX_RESPONSE];
        int n = http_build_response(404, "Not Found", "text/plain", "Page not found", buf, MAX_RESPONSE);
        TEST("build 404 Not Found response");
        EXPECT(n > 0 &&
               strstr(buf, "HTTP/1.1 404 Not Found\r\n") == buf &&
               strstr(buf, "Content-Length: 14\r\n") != NULL &&
               strstr(buf, "Page not found") != NULL);
    }

    /* Test 14: Build response with empty body */
    {
        char buf[MAX_RESPONSE];
        int n = http_build_response(204, "No Content", "text/plain", "", buf, MAX_RESPONSE);
        TEST("build response with empty body");
        EXPECT(n > 0 &&
               strstr(buf, "HTTP/1.1 204 No Content\r\n") == buf &&
               strstr(buf, "Content-Length: 0\r\n") != NULL);
    }

    printf("\n");

    /* --- Summary --- */
    printf("Results: %d / %d passed\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("ALL TESTS PASSED\n");
    }

    return tests_passed == tests_total ? 0 : 1;
}
