/*
 * Solution: Event System
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

void event_init(EventSystem *es)
{
    es->event_count = 0;
}

static EventEntry *find_event(EventSystem *es, const char *name)
{
    for (int i = 0; i < es->event_count; i++) {
        if (es->events[i].active && strcmp(es->events[i].name, name) == 0) {
            return &es->events[i];
        }
    }
    return NULL;
}

int event_on(EventSystem *es, const char *name, EventHandler handler)
{
    EventEntry *entry = find_event(es, name);
    if (entry == NULL) {
        if (es->event_count >= MAX_EVENTS) return -1;
        entry = &es->events[es->event_count++];
        strncpy(entry->name, name, EVENT_NAME_LEN - 1);
        entry->name[EVENT_NAME_LEN - 1] = '\0';
        entry->handler_count = 0;
        entry->active = 1;
    }
    if (entry->handler_count >= MAX_HANDLERS) return -1;
    entry->handlers[entry->handler_count++] = handler;
    return 0;
}

int event_emit(EventSystem *es, const char *name, void *data)
{
    EventEntry *entry = find_event(es, name);
    if (entry == NULL) return 0;

    for (int i = 0; i < entry->handler_count; i++) {
        entry->handlers[i](name, data);
    }
    return entry->handler_count;
}

int event_clear(EventSystem *es, const char *name)
{
    EventEntry *entry = find_event(es, name);
    if (entry == NULL) return -1;
    entry->handler_count = 0;
    entry->active = 0;
    return 0;
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
    printf("=== Event System (Solution) ===\n\n");

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
