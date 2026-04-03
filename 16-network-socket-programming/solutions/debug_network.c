/*
 * Debug Kata: Network Programming Bugs — SOLUTIONS
 *
 * All 4 bugs fixed. See comments marked "FIX:" for each change.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -o debug_network debug_network.c
 * Run:      ./debug_network
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

/* ── Test harness ─────────────────────────────────────────────── */

#define TEST(name) printf("  %-40s", name)
#define PASS() printf("[PASS]\n")
static int tests_passed = 0, tests_failed = 0;
#define EXPECT(cond) do { \
    if (cond) { PASS(); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); tests_failed++; } \
} while(0)

/* ================================================================
 * Bug 1 FIX: Use htonl() to convert length to network byte order.
 * ================================================================ */

int frame_message(const char *payload, int payload_len,
                  uint8_t *out_buf, int out_buf_size)
{
    int total = 4 + payload_len;
    if (total > out_buf_size) {
        return -1;
    }

    /* FIX: convert to network byte order with htonl() */
    uint32_t len = htonl((uint32_t)payload_len);
    memcpy(out_buf, &len, 4);

    memcpy(out_buf + 4, payload, payload_len);
    return total;
}

uint32_t read_frame_length(const uint8_t *buf)
{
    uint32_t net_len;
    memcpy(&net_len, buf, 4);
    return ntohl(net_len);
}

/* ================================================================
 * Bug 2 FIX: Loop until all requested bytes are read.
 * ================================================================ */

typedef struct {
    const uint8_t *data;
    int total_len;
    int read_pos;
} sim_socket_t;

int simulated_read(sim_socket_t *sock, uint8_t *dest, int requested,
                   int max_chunk)
{
    int available = sock->total_len - sock->read_pos;
    if (available <= 0) return 0;

    int to_copy = requested;
    if (to_copy > available) to_copy = available;
    if (to_copy > max_chunk) to_copy = max_chunk;

    memcpy(dest, sock->data + sock->read_pos, to_copy);
    sock->read_pos += to_copy;
    return to_copy;
}

int read_full(sim_socket_t *sock, uint8_t *dest, int wanted)
{
    /* FIX: loop until we have all requested bytes or source is exhausted */
    int total_read = 0;
    while (total_read < wanted) {
        int got = simulated_read(sock, dest + total_read,
                                 wanted - total_read, 3);
        if (got <= 0) break;  /* no more data available */
        total_read += got;
    }
    return total_read;
}

/* ================================================================
 * Bug 3 FIX: Check value length before copying.
 * ================================================================ */

#define HEADER_VALUE_MAX 32

int parse_header_value(const char *header_line,
                       char *out_value, int out_value_size)
{
    const char *colon = strstr(header_line, ": ");
    if (!colon) {
        return -1;
    }

    const char *value = colon + 2;

    /* FIX: check that value fits in the output buffer (including '\0') */
    int value_len = strlen(value);
    if (value_len + 1 > out_value_size) {
        return -1;  /* value too long for buffer */
    }

    strcpy(out_value, value);
    return 0;
}

/* ================================================================
 * Bug 4 FIX: Copy exactly content_length bytes (not content_length - 1).
 * ================================================================ */

int extract_http_body(const char *response, char *out_body, int out_size)
{
    const char *cl = strstr(response, "Content-Length: ");
    if (!cl) return -1;

    int content_length = atoi(cl + strlen("Content-Length: "));
    if (content_length <= 0 || content_length > out_size) return -1;

    const char *body = strstr(response, "\r\n\r\n");
    if (!body) return -1;
    body += 4;

    /* FIX: copy exactly content_length bytes, not content_length - 1 */
    memcpy(out_body, body, content_length);
    out_body[content_length] = '\0';

    return content_length;
}

/* ── main ─────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Network Programming Bugs (SOLUTIONS) ===\n\n");

    printf("Bug 1: Forgot htonl\n");
    {
        uint8_t buf[64];
        const char *msg = "Hello";
        int msg_len = 5;

        int written = frame_message(msg, msg_len, buf, sizeof(buf));
        TEST("frame writes correct total length");
        EXPECT(written == 9);

        uint32_t decoded_len = read_frame_length(buf);
        TEST("receiver decodes correct payload length");
        EXPECT(decoded_len == 5);

        TEST("payload data is correct");
        EXPECT(memcmp(buf + 4, "Hello", 5) == 0);
    }

    printf("Bug 2: Partial read not handled\n");
    {
        uint8_t source[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        sim_socket_t sock = {source, 10, 0};
        uint8_t dest[10];
        memset(dest, 0, sizeof(dest));

        int got = read_full(&sock, dest, 10);
        TEST("read_full returns all 10 bytes");
        EXPECT(got == 10);

        TEST("all bytes match source");
        EXPECT(memcmp(dest, source, 10) == 0);
    }

    printf("Bug 3: Buffer overflow in header parsing\n");
    {
        char value[HEADER_VALUE_MAX];
        int rc = parse_header_value("Content-Type: text/html",
                                    value, HEADER_VALUE_MAX);
        TEST("parses short header value");
        EXPECT(rc == 0 && strcmp(value, "text/html") == 0);

        const char *long_header =
            "X-Custom: "
            "this-value-is-way-too-long-for-the-buffer-and-will-overflow";
        char small_buf[HEADER_VALUE_MAX];
        memset(small_buf, 0, sizeof(small_buf));

        rc = parse_header_value(long_header, small_buf, HEADER_VALUE_MAX);
        TEST("rejects or truncates oversized value");
        EXPECT(rc == -1);
    }

    printf("Bug 4: Off-by-one in Content-Length\n");
    {
        const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, World!";

        char body[64];
        memset(body, 0, sizeof(body));

        int body_len = extract_http_body(response, body, sizeof(body));

        TEST("body length matches Content-Length");
        EXPECT(body_len == 13);

        TEST("body content is correct");
        EXPECT(memcmp(body, "Hello, World!", 13) == 0);
    }

    printf("\n  %d passed, %d failed\n\n", tests_passed, tests_failed);
    return tests_failed ? 1 : 0;
}
