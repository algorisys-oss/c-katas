/*
 * Kata: Event System
 *
 * Build a simple event system using callbacks:
 *   - event_on: register a callback for a named event
 *   - event_emit: fire all callbacks registered for an event
 *   - event_clear: remove all callbacks for an event
 *
 * 10 tests total.
 *
 * Hints:
 *   - Use a fixed-size array of EventEntry structs
 *   - Each entry stores: event name, array of callback function pointers, count
 *   - event_on finds (or creates) the entry and adds the callback
 *   - event_emit finds the entry and calls all registered callbacks
 *   - Use strcmp to match event names
 */

#include <stdio.h>
#include <string.h>

#define MAX_EVENTS 16
#define MAX_HANDLERS 8
#define EVENT_NAME_LEN 32

typedef void (*EventHandler)(const char *event, void *data);

typedef struct {
    char name[EVENT_NAME_LEN];
    EventHandler handlers[MAX_HANDLERS];
    int handler_count;
    int active;
} EventEntry;

typedef struct {
    EventEntry events[MAX_EVENTS];
    int event_count;
} EventSystem;

/* ── Exercise 1: event_init ───────────────────────────────────────────
 * Initialize the event system.
 */
void event_init(EventSystem *es)
{
    /* TODO: set event_count to 0 */
    (void)es;
}

/* Helper: find an event entry by name, or NULL if not found */
static EventEntry *find_event(EventSystem *es, const char *name)
{
    /* TODO: loop through events, return matching active entry */
    (void)es; (void)name;
    return NULL; /* placeholder */
}

/* ── Exercise 2: event_on ─────────────────────────────────────────────
 * Register a callback for a named event.
 *
 * - If the event exists, add the handler to its handler array
 * - If the event doesn't exist, create a new entry
 * - Return 0 on success, -1 if full
 */
int event_on(EventSystem *es, const char *name, EventHandler handler)
{
    /* TODO: find or create event entry, add handler */
    (void)es; (void)name; (void)handler;
    return -1; /* placeholder */
}

/* ── Exercise 3: event_emit ───────────────────────────────────────────
 * Fire all callbacks registered for the named event.
 *
 * - Find the event entry
 * - Call each registered handler with the event name and data pointer
 * - Return number of handlers called, or 0 if event not found
 */
int event_emit(EventSystem *es, const char *name, void *data)
{
    /* TODO: find event, call each handler */
    (void)es; (void)name; (void)data;
    return 0; /* placeholder */
}

/* ── Exercise 4: event_clear ──────────────────────────────────────────
 * Remove all handlers for a named event.
 * Return 0 on success, -1 if event not found.
 */
int event_clear(EventSystem *es, const char *name)
{
    /* TODO: find event, set handler_count to 0, mark inactive */
    (void)es; (void)name;
    return -1; /* placeholder */
}

/* ── Test Helpers ─────────────────────────────────────────────────── */

static int call_count_a = 0;
static int call_count_b = 0;
static int received_value = 0;

void handler_a(const char *event, void *data)
{
    (void)event; (void)data;
    call_count_a++;
}

void handler_b(const char *event, void *data)
{
    (void)event; (void)data;
    call_count_b++;
}

void handler_with_data(const char *event, void *data)
{
    (void)event;
    if (data != NULL) {
        received_value = *(int *)data;
    }
}

/* ── Test Harness ─────────────────────────────────────────────────── */

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
    printf("=== Event System ===\n\n");

    EventSystem es;
    event_init(&es);

    printf("event_on:\n");
    {
        int rc = event_on(&es, "click", handler_a);
        TEST("register handler_a for 'click'") EXPECT(rc == 0);
        rc = event_on(&es, "click", handler_b);
        TEST("register handler_b for 'click'") EXPECT(rc == 0);
        rc = event_on(&es, "keydown", handler_a);
        TEST("register handler_a for 'keydown'") EXPECT(rc == 0);
    }

    printf("\nevent_emit:\n");
    {
        call_count_a = 0; call_count_b = 0;
        int count = event_emit(&es, "click", NULL);
        TEST("emit 'click' calls 2 handlers") EXPECT(count == 2);
        TEST("handler_a was called") EXPECT(call_count_a == 1);
        TEST("handler_b was called") EXPECT(call_count_b == 1);

        call_count_a = 0;
        event_emit(&es, "keydown", NULL);
        TEST("emit 'keydown' calls handler_a") EXPECT(call_count_a == 1);

        count = event_emit(&es, "nonexistent", NULL);
        TEST("emit unknown event returns 0") EXPECT(count == 0);
    }

    printf("\nevent_emit with data:\n");
    {
        event_on(&es, "data_event", handler_with_data);
        int value = 42;
        event_emit(&es, "data_event", &value);
        TEST("handler received data value 42") EXPECT(received_value == 42);
    }

    printf("\nevent_clear:\n");
    {
        call_count_a = 0; call_count_b = 0;
        event_clear(&es, "click");
        event_emit(&es, "click", NULL);
        TEST("after clear, emit 'click' calls 0") EXPECT(call_count_a == 0 && call_count_b == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
