/*
 * Debug Kata: Network Programming Bugs
 *
 * This file contains 4 buggy network-related functions.
 * No actual sockets are needed — we use buffers and helper
 * functions to simulate network data.
 *
 * Your job: find and fix each bug.
 *
 * Compile:  gcc -std=c99 -Wall -Wextra -pedantic -o debug_network debug_network.c
 * Run:      ./debug_network
 *
 * 4 bugs to fix.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>  /* htonl, ntohl, htons, ntohs */

/* ── Test harness ─────────────────────────────────────────────── */

#define TEST(name) printf("  %-40s", name)
#define PASS() printf("[PASS]\n")
static int tests_passed = 0, tests_failed = 0;
#define EXPECT(cond) do { \
    if (cond) { PASS(); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); tests_failed++; } \
} while(0)

/* ================================================================
 * Bug 1: Forgot htonl — length prefix in wrong byte order
 * ================================================================
 *
 * Network protocols often prefix messages with a 4-byte length
 * in "network byte order" (big-endian). This function builds a
 * framed message: [4-byte length][payload], but writes the length
 * in host byte order (which is little-endian on x86).
 *
 * A receiver using ntohl() to read the length will get garbage.
 *
 * HINT: Use htonl() to convert the length to network byte order
 *       before writing it into the buffer.
 */

/* Writes a length-prefixed message into out_buf.
 * Format: [4 bytes: payload length in network byte order][payload]
 * Returns total bytes written (4 + payload_len). */
int frame_message(const char *payload, int payload_len,
                  uint8_t *out_buf, int out_buf_size)
{
    int total = 4 + payload_len;
    if (total > out_buf_size) {
        return -1;
    }

    /* BUG: writing length in host byte order instead of network
     * byte order. On a little-endian machine (x86), the bytes
     * are reversed compared to what the receiver expects. */
    uint32_t len = (uint32_t)payload_len;
    memcpy(out_buf, &len, 4);

    memcpy(out_buf + 4, payload, payload_len);
    return total;
}

/* Reads the length prefix from a framed message buffer.
 * Returns the payload length (decoded from network byte order). */
uint32_t read_frame_length(const uint8_t *buf)
{
    uint32_t net_len;
    memcpy(&net_len, buf, 4);
    return ntohl(net_len);
}

/* ================================================================
 * Bug 2: Partial read not handled
 * ================================================================
 *
 * Real network reads (recv/read) can return fewer bytes than
 * requested — you must loop until you have all the data.
 *
 * This function simulates reading from a "socket" that delivers
 * data in small chunks. The simulated_read() function returns
 * at most 3 bytes at a time (like a slow network).
 *
 * The buggy code calls simulated_read() once and assumes it got
 * everything. It should loop until all bytes are received.
 *
 * HINT: Keep calling simulated_read() in a loop, advancing the
 *       buffer pointer and decreasing bytes_remaining, until
 *       you've read everything (or the source is exhausted).
 */

/* Simulated "socket" — a source buffer with a read position. */
typedef struct {
    const uint8_t *data;
    int total_len;
    int read_pos;
} sim_socket_t;

/* Simulates a network read: returns at most max_chunk bytes at a time.
 * Returns number of bytes "received", or 0 if no more data. */
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

/* Read exactly 'wanted' bytes from the simulated socket into dest.
 * Returns number of bytes actually read. */
int read_full(sim_socket_t *sock, uint8_t *dest, int wanted)
{
    /* BUG: only reads once — may get fewer bytes than wanted
     * because simulated_read returns at most 3 bytes at a time. */
    int got = simulated_read(sock, dest, wanted, 3);
    return got;
}

/* ================================================================
 * Bug 3: Buffer overflow in HTTP header parsing
 * ================================================================
 *
 * Parses an HTTP-style header line like "Content-Type: text/html"
 * and copies the value ("text/html") into a fixed-size buffer.
 *
 * The bug: if the value is longer than the buffer, it copies
 * past the end — a classic buffer overflow.
 *
 * HINT: Check the length of the value before copying. If it
 *       doesn't fit, truncate or return an error.
 */

#define HEADER_VALUE_MAX 32

/* Extracts the value from a "Key: Value" header line.
 * Copies the value (after ": ") into out_value.
 * Returns 0 on success, -1 on error. */
int parse_header_value(const char *header_line,
                       char *out_value, int out_value_size)
{
    const char *colon = strstr(header_line, ": ");
    if (!colon) {
        return -1;  /* malformed header */
    }

    const char *value = colon + 2;  /* skip ": " */

    /* BUG: copies the entire value regardless of buffer size.
     * If value is longer than out_value_size, this writes past
     * the end of out_value — buffer overflow!
     * Note: out_value_size is available but never used! */
    (void)out_value_size;
    strcpy(out_value, value);

    return 0;
}

/* ================================================================
 * Bug 4: Off-by-one in Content-Length body read
 * ================================================================
 *
 * Parses a simplified HTTP response: finds "Content-Length: N",
 * then locates the body (after "\r\n\r\n") and copies N bytes.
 *
 * The bug: copies (content_length - 1) bytes instead of
 * content_length bytes, because it uses < instead of <=
 * in the wrong place (or subtracts 1 where it shouldn't).
 *
 * HINT: The body starts right after "\r\n\r\n". Copy exactly
 *       content_length bytes from that point.
 */

/* Extracts the body from a simplified HTTP response.
 * Writes the body into out_body (must be big enough).
 * Returns the number of body bytes copied, or -1 on error. */
int extract_http_body(const char *response, char *out_body, int out_size)
{
    /* Find Content-Length */
    const char *cl = strstr(response, "Content-Length: ");
    if (!cl) return -1;

    int content_length = atoi(cl + strlen("Content-Length: "));
    if (content_length <= 0 || content_length > out_size) return -1;

    /* Find the body: starts after the first "\r\n\r\n" */
    const char *body = strstr(response, "\r\n\r\n");
    if (!body) return -1;
    body += 4;  /* skip past "\r\n\r\n" */

    /* BUG: copies one byte too few.
     * The subtraction of 1 is wrong — we need exactly
     * content_length bytes. */
    memcpy(out_body, body, content_length - 1);
    out_body[content_length - 1] = '\0';

    return content_length - 1;
}

/* ── main ─────────────────────────────────────────────────────── */

int main(void)
{
    printf("\n=== Debug Kata: Network Programming Bugs ===\n\n");

    /* --- Bug 1: Byte order --- */
    printf("Bug 1: Forgot htonl\n");
    {
        uint8_t buf[64];
        const char *msg = "Hello";
        int msg_len = 5;

        int written = frame_message(msg, msg_len, buf, sizeof(buf));
        TEST("frame writes correct total length");
        EXPECT(written == 9);  /* 4 + 5 */

        /* The receiver decodes the length using ntohl().
         * If frame_message wrote in host byte order, ntohl
         * will produce garbage. */
        uint32_t decoded_len = read_frame_length(buf);
        TEST("receiver decodes correct payload length");
        EXPECT(decoded_len == 5);

        /* Also verify the payload itself is intact */
        TEST("payload data is correct");
        EXPECT(memcmp(buf + 4, "Hello", 5) == 0);
    }

    /* --- Bug 2: Partial read --- */
    printf("Bug 2: Partial read not handled\n");
    {
        /* Source data: 10 bytes. simulated_read gives at most 3 at a time. */
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

    /* --- Bug 3: Buffer overflow --- */
    printf("Bug 3: Buffer overflow in header parsing\n");
    {
        /* Short value — should work fine */
        char value[HEADER_VALUE_MAX];
        int rc = parse_header_value("Content-Type: text/html",
                                    value, HEADER_VALUE_MAX);
        TEST("parses short header value");
        EXPECT(rc == 0 && strcmp(value, "text/html") == 0);

        /* Long value — longer than HEADER_VALUE_MAX.
         * With the bug, this overflows. When fixed, it should
         * either truncate or return an error. */
        const char *long_header =
            "X-Custom: "
            "this-value-is-way-too-long-for-the-buffer-and-will-overflow";
        char small_buf[HEADER_VALUE_MAX];
        memset(small_buf, 0, sizeof(small_buf));

        rc = parse_header_value(long_header, small_buf, HEADER_VALUE_MAX);
        TEST("rejects or truncates oversized value");
        /* When fixed: returns -1 (error) because value doesn't fit */
        EXPECT(rc == -1);
    }

    /* --- Bug 4: Off-by-one in Content-Length --- */
    printf("Bug 4: Off-by-one in Content-Length\n");
    {
        /* Build a fake HTTP response */
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

    /* ── Summary ─────────────────────────────────────────────── */
    printf("\n  %d passed, %d failed\n\n", tests_passed, tests_failed);
    return tests_failed ? 1 : 0;
}
