/*
 * Kata: Command Dispatcher
 *
 * Build a command dispatcher that maps string names to handler functions.
 *
 *   - dispatch_register: associate a command name with a function
 *   - dispatch_run: look up a command by name and call its handler
 *   - dispatch_list: print all registered commands
 *
 * 9 tests total.
 *
 * Hints:
 *   - Use an array of {name, handler} pairs
 *   - Use strcmp to match command names
 *   - Handler signature: void (*)(int argc, const char *argv[])
 *     (like a mini main() for each command)
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

/* ── Exercise 1: dispatch_init ────────────────────────────────────────
 * Initialize the dispatcher.
 */
void dispatch_init(Dispatcher *d)
{
    /* TODO: set count to 0 */
    (void)d;
}

/* ── Exercise 2: dispatch_register ────────────────────────────────────
 * Register a command with a name, description, and handler function.
 * Return 0 on success, -1 if full or name already exists.
 */
int dispatch_register(Dispatcher *d, const char *name, const char *desc,
                      CommandHandler handler)
{
    /* TODO: check for duplicate, add to commands array */
    (void)d; (void)name; (void)desc; (void)handler;
    return -1; /* placeholder */
}

/* ── Exercise 3: dispatch_run ─────────────────────────────────────────
 * Look up a command by name and call its handler.
 *
 * Parameters:
 *   d    — the dispatcher
 *   name — command name to look up
 *   argc — number of arguments to pass to handler
 *   argv — argument strings to pass to handler
 *
 * Returns: 0 if command found and called, -1 if not found
 */
int dispatch_run(Dispatcher *d, const char *name, int argc, const char *argv[])
{
    /* TODO: find command, call handler */
    (void)d; (void)name; (void)argc; (void)argv;
    return -1; /* placeholder */
}

/* ── Exercise 4: dispatch_count ───────────────────────────────────────
 * Return the number of registered commands.
 */
int dispatch_count(const Dispatcher *d)
{
    /* TODO: count active commands */
    (void)d;
    return 0; /* placeholder */
}

/* ── Test Helpers ─────────────────────────────────────────────────── */

static int greet_called = 0;
static int add_result = 0;
static int help_called = 0;

void cmd_greet(int argc, const char *argv[])
{
    greet_called = 1;
    if (argc > 0) {
        /* Just verify we received the argument */
        (void)argv[0];
    }
}

void cmd_add(int argc, const char *argv[])
{
    if (argc >= 2) {
        int a = 0, b = 0;
        /* Simple atoi-like parsing */
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
    printf("=== Command Dispatcher ===\n\n");

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
