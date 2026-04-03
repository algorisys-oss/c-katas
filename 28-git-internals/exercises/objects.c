/*
 * Kata: Git Object Store
 *
 * Build an in-memory simulation of Git's object database:
 *   - Store blobs (file content) identified by their hash
 *   - Retrieve objects by hash (content-addressable lookup)
 *   - Build tree objects that map filenames to blob hashes
 *
 * This teaches the core Git concept: objects are immutable,
 * content-addressed, and form a graph (trees reference blobs).
 *
 * 10 tests total.
 *
 * Hints:
 *   - Reuse simple_hash / hash_to_hex / git_blob_hash from sha1.c
 *     (they are copied here so this file compiles standalone)
 *   - The store is a simple array of objects (no need for a hash table)
 *   - Linear scan to find by hash is fine for small stores
 *   - Trees are objects too — they just contain a list of (name, hash) entries
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define HASH_SIZE 20
#define HEX_SIZE  41
#define MAX_OBJECTS 256
#define MAX_CONTENT 4096
#define MAX_TREE_ENTRIES 32
#define MAX_NAME_LEN 64

/* ── Hash utilities (from sha1.c) ─────────────────────────────── */

void simple_hash(const uint8_t *data, size_t len, uint8_t out_hash[HASH_SIZE])
{
    uint32_t state[5] = {
        0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0
    };
    for (size_t i = 0; i < len; i++) {
        int idx = (int)(i % 5);
        state[idx] = state[idx] * 31 + data[i];
        state[(idx + 1) % 5] ^= (state[idx] << 5) | (state[idx] >> 27);
    }
    for (int r = 0; r < 3; r++) {
        for (int j = 0; j < 5; j++) {
            state[j] += (state[(j + 1) % 5] << 7) | (state[(j + 1) % 5] >> 25);
        }
    }
    for (int i = 0; i < 5; i++) {
        out_hash[i * 4 + 0] = (uint8_t)(state[i] >> 24);
        out_hash[i * 4 + 1] = (uint8_t)(state[i] >> 16);
        out_hash[i * 4 + 2] = (uint8_t)(state[i] >> 8);
        out_hash[i * 4 + 3] = (uint8_t)(state[i]);
    }
}

void hash_to_hex(const uint8_t hash[HASH_SIZE], char out_hex[HEX_SIZE])
{
    for (int i = 0; i < HASH_SIZE; i++) {
        sprintf(out_hex + i * 2, "%02x", hash[i]);
    }
    out_hex[40] = '\0';
}

void git_blob_hash(const char *content, size_t len, char out_hex[HEX_SIZE])
{
    char header[64];
    int hdr_len = sprintf(header, "blob %zu", len);

    size_t total = (size_t)hdr_len + 1 + len;
    uint8_t *buf = malloc(total);
    if (!buf) {
        out_hex[0] = '\0';
        return;
    }
    memcpy(buf, header, (size_t)hdr_len);
    buf[hdr_len] = '\0';
    memcpy(buf + hdr_len + 1, content, len);

    uint8_t hash[HASH_SIZE];
    simple_hash(buf, total, hash);
    hash_to_hex(hash, out_hex);
    free(buf);
}

/* ── Object types ─────────────────────────────────────────────── */

typedef enum {
    OBJ_BLOB,
    OBJ_TREE
} obj_type_t;

/* A single entry in a tree: filename → blob/tree hash */
typedef struct {
    char name[MAX_NAME_LEN];
    char hash[HEX_SIZE];
} tree_entry_t;

/* A stored object */
typedef struct {
    obj_type_t type;
    char hash[HEX_SIZE];           /* hex hash of this object */
    char content[MAX_CONTENT];     /* raw content (for blobs) */
    size_t content_len;
    tree_entry_t entries[MAX_TREE_ENTRIES]; /* for trees */
    int entry_count;
} object_t;

/* The object store */
typedef struct {
    object_t objects[MAX_OBJECTS];
    int count;
} object_store_t;


/* ── Exercise 1: store_init / store_destroy ───────────────────────
 * Initialize the store (set count to 0).
 * Destroy frees any resources (nothing dynamic here, just reset).
 */
void store_init(object_store_t *store)
{
    /* TODO: Initialize the store */
    (void)store;
}

void store_destroy(object_store_t *store)
{
    /* TODO: Clean up the store (reset count) */
    (void)store;
}

/* ── Exercise 2: store_blob ───────────────────────────────────────
 * Store a blob in the object store. Return pointer to its hex hash.
 *
 * Steps:
 *   1. Compute the git blob hash with git_blob_hash()
 *   2. Check if an object with this hash already exists (dedup!)
 *      - If it does, return the existing hash (don't store twice)
 *   3. Add a new object to store->objects[store->count]:
 *      - Set type to OBJ_BLOB
 *      - Copy the hash
 *      - Copy the content and content_len
 *      - Set entry_count to 0
 *   4. Increment store->count
 *   5. Return pointer to the new object's hash string
 *
 * Parameters:
 *   store   — the object store
 *   content — blob content
 *   len     — content length
 *
 * Returns: pointer to the hex hash string (stored in the object)
 */
const char *store_blob(object_store_t *store, const char *content, size_t len)
{
    /* TODO: Hash content, dedup, store, return hash */
    (void)store; (void)content; (void)len;
    return NULL;
}

/* ── Exercise 3: store_get ────────────────────────────────────────
 * Retrieve an object's content by its hash.
 *
 * Linear scan through store->objects. If found and type is OBJ_BLOB,
 * copy content to out_buf (up to buf_size - 1 bytes), null-terminate,
 * and return the number of bytes copied.
 *
 * Return -1 if not found.
 *
 * Parameters:
 *   store    — the object store
 *   hash     — hex hash string to look up
 *   out_buf  — buffer to copy content into
 *   buf_size — size of out_buf
 *
 * Returns: number of bytes copied, or -1 if not found
 */
int store_get(const object_store_t *store, const char *hash,
              char *out_buf, size_t buf_size)
{
    /* TODO: Find object by hash, copy content to out_buf */
    (void)store; (void)hash; (void)out_buf; (void)buf_size;
    return -1;
}

/* ── Exercise 4: store_tree_add ───────────────────────────────────
 * Add a named entry (filename → blob hash) to a tree object.
 *
 * If no tree with tree_hash exists yet, create one:
 *   - type = OBJ_TREE, entry_count = 0
 *   - The tree_hash is provided by the caller (or use "" for a new tree)
 * Add the (name, blob_hash) pair to the tree's entries array.
 *
 * For simplicity, the tree_hash is caller-managed. In real Git, the
 * tree hash would be computed from its entries, but here we let the
 * caller assign it.
 *
 * Parameters:
 *   store     — the object store
 *   tree_hash — hash to identify this tree (caller-assigned)
 *   name      — filename for this entry
 *   blob_hash — hash of the blob this entry points to
 *
 * Returns: 0 on success, -1 on failure
 */
int store_tree_add(object_store_t *store, const char *tree_hash,
                   const char *name, const char *blob_hash)
{
    /* TODO: Find or create tree, add entry */
    (void)store; (void)tree_hash; (void)name; (void)blob_hash;
    return -1;
}


/* ══════════════════════════════════════════════════════════════════
 *  Tests
 * ══════════════════════════════════════════════════════════════════ */

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { \
    tests_total++; \
    printf("  Test %d: %-50s ", tests_total, name);

#define EXPECT(cond) \
    if (cond) { printf("[PASS]\n"); tests_passed++; } \
    else { printf("[FAIL] line %d\n", __LINE__); } \
    } while(0)

int main(void)
{
    printf("=== Git Object Store ===\n\n");

    /* --- store_init / store_blob --- */
    printf("store_blob:\n");
    {
        static object_store_t store;
        store_init(&store);

        const char *h = store_blob(&store, "hello", 5);
        TEST("store_blob returns non-NULL hash")
            EXPECT(h != NULL && strlen(h) == 40);

        store_destroy(&store);
    }

    {
        static object_store_t store;
        store_init(&store);

        const char *h1 = store_blob(&store, "hello", 5);
        const char *h2 = store_blob(&store, "hello", 5);
        TEST("same content → same hash (dedup)")
            EXPECT(h1 != NULL && h2 != NULL && strcmp(h1, h2) == 0);

        TEST("dedup: object count stays 1")
            EXPECT(store.count == 1);

        store_destroy(&store);
    }

    {
        static object_store_t store;
        store_init(&store);

        const char *h1 = store_blob(&store, "hello", 5);
        const char *h2 = store_blob(&store, "world", 5);
        TEST("different content → different hash")
            EXPECT(h1 != NULL && h2 != NULL && strcmp(h1, h2) != 0);

        TEST("two different blobs → count is 2")
            EXPECT(store.count == 2);

        store_destroy(&store);
    }

    /* --- store_get --- */
    printf("\nstore_get:\n");
    {
        static object_store_t store;
        store_init(&store);

        const char *h = store_blob(&store, "hello", 5);
        char buf[256];
        int n = store_get(&store, h, buf, sizeof(buf));
        TEST("retrieve blob by hash")
            EXPECT(n == 5 && memcmp(buf, "hello", 5) == 0);

        store_destroy(&store);
    }

    {
        static object_store_t store;
        store_init(&store);

        char buf[256];
        int n = store_get(&store, "0000000000000000000000000000000000000000",
                          buf, sizeof(buf));
        TEST("get non-existent hash → returns -1")
            EXPECT(n == -1);

        store_destroy(&store);
    }

    /* --- store_tree_add --- */
    printf("\nstore_tree_add:\n");
    {
        static object_store_t store;
        store_init(&store);

        const char *blob_h = store_blob(&store, "file content", 12);
        int rc = store_tree_add(&store, "mytree01", "readme.txt", blob_h);
        TEST("add entry to tree succeeds")
            EXPECT(rc == 0);

        store_destroy(&store);
    }

    {
        static object_store_t store;
        store_init(&store);

        const char *h1 = store_blob(&store, "aaa", 3);
        const char *h2 = store_blob(&store, "bbb", 3);
        store_tree_add(&store, "mytree01", "a.txt", h1);
        store_tree_add(&store, "mytree01", "b.txt", h2);

        /* Find the tree and check entry count */
        int found = 0;
        for (int i = 0; i < store.count; i++) {
            if (store.objects[i].type == OBJ_TREE &&
                strcmp(store.objects[i].hash, "mytree01") == 0) {
                found = store.objects[i].entry_count;
                break;
            }
        }
        TEST("tree with 2 entries has entry_count == 2")
            EXPECT(found == 2);

        store_destroy(&store);
    }

    {
        static object_store_t store;
        store_init(&store);

        const char *h1 = store_blob(&store, "content_a", 9);
        store_tree_add(&store, "mytree01", "file.txt", h1);

        /* Verify the tree entry has correct name and hash */
        int match = 0;
        for (int i = 0; i < store.count; i++) {
            if (store.objects[i].type == OBJ_TREE &&
                strcmp(store.objects[i].hash, "mytree01") == 0) {
                if (store.objects[i].entry_count >= 1 &&
                    strcmp(store.objects[i].entries[0].name, "file.txt") == 0 &&
                    strcmp(store.objects[i].entries[0].hash, h1) == 0) {
                    match = 1;
                }
                break;
            }
        }
        TEST("tree entry stores correct name and hash")
            EXPECT(match);

        store_destroy(&store);
    }

    /* --- Summary --- */
    printf("\n%d / %d tests passed.\n", tests_passed, tests_total);
    if (tests_passed == tests_total) {
        printf("All tests passed!\n");
    }
    return tests_passed == tests_total ? 0 : 1;
}
