/*
 * Solution: Command Dispatcher
 */

#include <stdio.h>
#include <string.h>

#define MAX_COMMANDS 16
#define CMD_NAME_LEN 32

typedef void (*CommandHandler)(int argc, const char *argv[]);

typedef struct {
    char name[CMD_NAME_LEN];
    char description[64];
    CommandHandler handler;
    int active;
} Command;

typedef struct {
    Command commands[MAX_COMMANDS];
    int count;
} Dispatcher;

void dispatch_init(Dispatcher *d)
{
    d->count = 0;
}

int dispatch_register(Dispatcher *d, const char *name, const char *desc,
                      CommandHandler handler)
{
    /* Check for duplicate */
    for (int i = 0; i < d->count; i++) {
        if (d->commands[i].active && strcmp(d->commands[i].name, name) == 0) {
            return -1;
        }
    }
    if (d->count >= MAX_COMMANDS) return -1;

    Command *cmd = &d->commands[d->count++];
    strncpy(cmd->name, name, CMD_NAME_LEN - 1);
    cmd->name[CMD_NAME_LEN - 1] = '\0';
    strncpy(cmd->description, desc, 63);
    cmd->description[63] = '\0';
    cmd->handler = handler;
    cmd->active = 1;
    return 0;
}

int dispatch_run(Dispatcher *d, const char *name, int argc, const char *argv[])
{
    for (int i = 0; i < d->count; i++) {
        if (d->commands[i].active && strcmp(d->commands[i].name, name) == 0) {
            d->commands[i].handler(argc, argv);
            return 0;
        }
    }
    return -1;
}

int dispatch_count(const Dispatcher *d)
{
    int count = 0;
    for (int i = 0; i < d->count; i++) {
        if (d->commands[i].active) count++;
    }
    return count;
}

/* ── Test Helpers ─────────────────────────────────────────────────── */

static int greet_called = 0;
static int add_result = 0;
static int help_called = 0;

void cmd_greet(int argc, const char *argv[])
{
    greet_called = 1;
    if (argc > 0) {
        (void)argv[0];
    }
}

void cmd_add(int argc, const char *argv[])
{
    if (argc >= 2) {
        int a = 0, b = 0;
        for (const char *p = argv[0]; *p >= '0' && *p <= '9'; p++)
            a = a * 10 + (*p - '0');
        for (const char *p = argv[1]; *p >= '0' && *p <= '9'; p++)
            b = b * 10 + (*p - '0');
        add_result = a + b;
    }
}

void cmd_help(int argc, const char *argv[])
{
    (void)argc; (void)argv;
    help_called = 1;
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
    printf("=== Command Dispatcher (Solution) ===\n\n");

    Dispatcher d;
    dispatch_init(&d);

    printf("dispatch_register:\n");
    {
        int rc = dispatch_register(&d, "greet", "Say hello", cmd_greet);
        TEST("register 'greet' returns 0") EXPECT(rc == 0);
        dispatch_register(&d, "add", "Add two numbers", cmd_add);
        dispatch_register(&d, "help", "Show help", cmd_help);
        TEST("count is 3") EXPECT(dispatch_count(&d) == 3);

        rc = dispatch_register(&d, "greet", "Duplicate", cmd_greet);
        TEST("duplicate name returns -1") EXPECT(rc == -1);
    }

    printf("\ndispatch_run:\n");
    {
        greet_called = 0;
        const char *args1[] = {"World"};
        int rc = dispatch_run(&d, "greet", 1, args1);
        TEST("run 'greet' returns 0") EXPECT(rc == 0);
        TEST("greet handler was called") EXPECT(greet_called == 1);

        add_result = 0;
        const char *args2[] = {"3", "7"};
        dispatch_run(&d, "add", 2, args2);
        TEST("add 3 + 7 = 10") EXPECT(add_result == 10);

        rc = dispatch_run(&d, "unknown", 0, NULL);
        TEST("run unknown returns -1") EXPECT(rc == -1);

        help_called = 0;
        dispatch_run(&d, "help", 0, NULL);
        TEST("help handler was called") EXPECT(help_called == 1);
    }

    printf("\nedge cases:\n");
    {
        Dispatcher empty;
        dispatch_init(&empty);
        int rc = dispatch_run(&empty, "anything", 0, NULL);
        TEST("run on empty dispatcher returns -1") EXPECT(rc == -1);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
