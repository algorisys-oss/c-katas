/*
 * Solution: Git Object Store
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
        snprintf(out_hex + i * 2, 3, "%02x", hash[i]);
    }
    out_hex[40] = '\0';
}

void git_blob_hash(const char *content, size_t len, char out_hex[HEX_SIZE])
{
    char header[64];
    int hdr_len = snprintf(header, sizeof(header), "blob %zu", len);

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

typedef struct {
    char name[MAX_NAME_LEN];
    char hash[HEX_SIZE];
} tree_entry_t;

typedef struct {
    obj_type_t type;
    char hash[HEX_SIZE];
    char content[MAX_CONTENT];
    size_t content_len;
    tree_entry_t entries[MAX_TREE_ENTRIES];
    int entry_count;
} object_t;

typedef struct {
    object_t objects[MAX_OBJECTS];
    int count;
} object_store_t;

/* ── store_init / store_destroy ───────────────────────────────── */

void store_init(object_store_t *store)
{
    store->count = 0;
}

void store_destroy(object_store_t *store)
{
    store->count = 0;
}

/* ── store_blob ───────────────────────────────────────────────── */

const char *store_blob(object_store_t *store, const char *content, size_t len)
{
    /* Compute hash */
    char hex[HEX_SIZE];
    git_blob_hash(content, len, hex);

    /* Check for existing object with same hash (dedup) */
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->objects[i].hash, hex) == 0) {
            return store->objects[i].hash;
        }
    }

    /* Store new object */
    if (store->count >= MAX_OBJECTS) {
        return NULL;
    }

    object_t *obj = &store->objects[store->count];
    obj->type = OBJ_BLOB;
    memcpy(obj->hash, hex, HEX_SIZE);

    size_t copy_len = len < MAX_CONTENT ? len : MAX_CONTENT - 1;
    memcpy(obj->content, content, copy_len);
    obj->content[copy_len] = '\0';
    obj->content_len = len;
    obj->entry_count = 0;

    store->count++;
    return obj->hash;
}

/* ── store_get ────────────────────────────────────────────────── */

int store_get(const object_store_t *store, const char *hash,
              char *out_buf, size_t buf_size)
{
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->objects[i].hash, hash) == 0 &&
            store->objects[i].type == OBJ_BLOB) {
            size_t copy_len = store->objects[i].content_len;
            if (copy_len > buf_size - 1) {
                copy_len = buf_size - 1;
            }
            memcpy(out_buf, store->objects[i].content, copy_len);
            out_buf[copy_len] = '\0';
            return (int)store->objects[i].content_len;
        }
    }
    return -1;
}

/* ── store_tree_add ───────────────────────────────────────────── */

int store_tree_add(object_store_t *store, const char *tree_hash,
                   const char *name, const char *blob_hash)
{
    /* Find existing tree */
    for (int i = 0; i < store->count; i++) {
        if (store->objects[i].type == OBJ_TREE &&
            strcmp(store->objects[i].hash, tree_hash) == 0) {
            object_t *tree = &store->objects[i];
            if (tree->entry_count >= MAX_TREE_ENTRIES) {
                return -1;
            }
            tree_entry_t *entry = &tree->entries[tree->entry_count];
            strncpy(entry->name, name, MAX_NAME_LEN - 1);
            entry->name[MAX_NAME_LEN - 1] = '\0';
            strncpy(entry->hash, blob_hash, HEX_SIZE - 1);
            entry->hash[HEX_SIZE - 1] = '\0';
            tree->entry_count++;
            return 0;
        }
    }

    /* Create new tree */
    if (store->count >= MAX_OBJECTS) {
        return -1;
    }

    object_t *tree = &store->objects[store->count];
    tree->type = OBJ_TREE;
    strncpy(tree->hash, tree_hash, HEX_SIZE - 1);
    tree->hash[HEX_SIZE - 1] = '\0';
    tree->content[0] = '\0';
    tree->content_len = 0;
    tree->entry_count = 0;

    tree_entry_t *entry = &tree->entries[tree->entry_count];
    strncpy(entry->name, name, MAX_NAME_LEN - 1);
    entry->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(entry->hash, blob_hash, HEX_SIZE - 1);
    entry->hash[HEX_SIZE - 1] = '\0';
    tree->entry_count++;

    store->count++;
    return 0;
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
