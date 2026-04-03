/*
 * Kata: HTTP Request/Response Parser
 *
 * Parse HTTP requests and responses as plain strings — no actual networking.
 * This is exactly what a real web server does internally: every request that
 * arrives over a socket is just a string of bytes that must be parsed.
 *
 * Functions to implement:
 *   - http_parse_request  (extract method, path, version from request line)
 *   - http_parse_header   (extract a specific header's value)
 *   - http_build_response (construct a well-formed HTTP response string)
 *
 * 14 tests.
 *
 * Hints:
 *   - strchr() finds a character in a string — great for finding spaces
 *   - strstr() finds a substring — great for finding "\r\n"
 *   - strncpy() copies N characters — use it to extract substrings
 *   - snprintf() builds formatted strings safely (returns bytes written)
 *   - HTTP lines end with \r\n (carriage return + line feed), not just \n
 *   - Header names are case-insensitive: "Host" == "host" == "HOST"
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
 * The request line format is:
 *   METHOD SP PATH SP VERSION CRLF
 *
 * Algorithm:
 *   1. Find the first space — everything before it is the method
 *   2. Find the second space — everything between is the path
 *   3. Find \r\n (or end of string) — everything between is the version
 *   4. Copy each part into its output buffer
 *
 * Returns 0 on success, -1 on parse failure.
 */
int http_parse_request(const char *raw, char *method, char *path, char *version)
{
    /* TODO: Implement this function
     *
     * Steps:
     *   1. Check for NULL inputs — return -1 if any are NULL
     *   2. Use strchr(raw, ' ') to find the first space
     *   3. Copy characters from raw up to that space into method
     *   4. From after the first space, find the second space
     *   5. Copy the path between the two spaces
     *   6. From after the second space, find "\r\n" with strstr()
     *   7. Copy the version string
     *   8. Null-terminate all output strings
     *   9. Return 0 on success
     */
    (void)raw; (void)method; (void)path; (void)version;
    return -1;
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
 * Header format: "Name: value\r\n"
 * Note: there may be a space after the colon (skip it).
 * Note: header name matching must be case-insensitive.
 *
 * Returns 0 on success, -1 if header not found.
 */
int http_parse_header(const char *raw, const char *name, char *out_value, int size)
{
    /* TODO: Implement this function
     *
     * Steps:
     *   1. Check for NULL inputs
     *   2. Walk through the raw string line by line (find each \r\n)
     *   3. For each line, check if it starts with "name:" (case-insensitive)
     *      - To compare case-insensitively, convert both chars to lowercase
     *        before comparing: if (c >= 'A' && c <= 'Z') c += 32;
     *   4. If match found, skip the colon and any spaces after it
     *   5. Copy from there to the next \r\n into out_value
     *   6. Return 0 on success, -1 if not found
     */
    (void)raw; (void)name; (void)out_value; (void)size;
    return -1;
}

/* ── http_build_response ─────────────────────────────────────────
 *
 * Build an HTTP response string from the given parts.
 *
 * Example output for (200, "OK", "text/html", "<h1>Hi</h1>"):
 *   "HTTP/1.1 200 OK\r\n"
 *   "Content-Type: text/html\r\n"
 *   "Content-Length: 11\r\n"
 *   "\r\n"
 *   "<h1>Hi</h1>"
 *
 * The Content-Length header should be set to strlen(body).
 * If body is NULL, treat it as empty string (length 0).
 *
 * Returns the number of bytes written (excluding null terminator),
 * or -1 on error.
 */
int http_build_response(int status_code, const char *status_text,
                        const char *content_type, const char *body,
                        char *out_buf, int size)
{
    /* TODO: Implement this function
     *
     * Steps:
     *   1. Check for NULL inputs (status_text, content_type, out_buf)
     *   2. Calculate body_len = strlen(body), or 0 if body is NULL
     *   3. Use snprintf() to write the full response into out_buf:
     *        "HTTP/1.1 %d %s\r\n"
     *        "Content-Type: %s\r\n"
     *        "Content-Length: %d\r\n"
     *        "\r\n"
     *        "%s"
     *   4. Check snprintf return value — if it would overflow, return -1
     *   5. Return the number of bytes written
     */
    (void)status_code; (void)status_text; (void)content_type;
    (void)body; (void)out_buf; (void)size;
    return -1;
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
