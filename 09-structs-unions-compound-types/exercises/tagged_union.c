/*
 * Kata: Tagged Union (Variant Type)
 *
 * Build a Value type that can hold an int, a float, or a string.
 * Use a tagged union (enum tag + union data) for type safety.
 *
 *   - value_from_int, value_from_float, value_from_string: constructors
 *   - value_print: print the value based on its type
 *   - value_to_string: convert any value to a string representation
 *   - value_add: add two values (int+int=int, float+float=float, string+string=concat)
 *
 * 13 tests total.
 *
 * Hints:
 *   - Always check the type tag before accessing union fields
 *   - For string concatenation in value_add, use a static buffer
 *   - snprintf is your friend for safe string formatting
 */

#include <stdio.h>
#include <string.h>

#define VAL_STR_MAX 64

typedef enum {
    VAL_INT,
    VAL_FLOAT,
    VAL_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        int i;
        float f;
        char s[VAL_STR_MAX];
    } data;
} Value;

/* ── Exercise 1: Constructors ─────────────────────────────────────────
 * Create Value instances from different types.
 */
Value value_from_int(int n)
{
    Value v;
    /* TODO: set type to VAL_INT, set data.i */
    v.type = VAL_INT;
    v.data.i = 0;
    (void)n;
    return v;
}

Value value_from_float(float f)
{
    Value v;
    /* TODO: set type to VAL_FLOAT, set data.f */
    v.type = VAL_FLOAT;
    v.data.f = 0.0f;
    (void)f;
    return v;
}

Value value_from_string(const char *s)
{
    Value v;
    /* TODO: set type to VAL_STRING, copy s into data.s (safely!) */
    v.type = VAL_STRING;
    v.data.s[0] = '\0';
    (void)s;
    return v;
}

/* ── Exercise 2: value_type_name ──────────────────────────────────────
 * Return the type name as a string: "int", "float", or "string".
 */
const char *value_type_name(const Value *v)
{
    /* TODO: switch on v->type, return the name */
    (void)v;
    return "unknown"; /* placeholder */
}

/* ── Exercise 3: value_to_string ──────────────────────────────────────
 * Convert any Value to its string representation.
 * Write into the provided buffer (max buf_size chars).
 *
 * Formats:
 *   VAL_INT:    "%d"
 *   VAL_FLOAT:  "%.2f"
 *   VAL_STRING: "%s"
 */
void value_to_string(const Value *v, char *buf, int buf_size)
{
    /* TODO: switch on type, use snprintf to write into buf */
    (void)v; (void)buf; (void)buf_size;
}

/* ── Exercise 4: value_add ────────────────────────────────────────────
 * Add two values of the same type.
 *
 * - INT + INT = INT (a.i + b.i)
 * - FLOAT + FLOAT = FLOAT (a.f + b.f)
 * - STRING + STRING = STRING (concatenation)
 * - Mismatched types: return a Value with type VAL_INT and data.i = 0
 *   (a simple error indicator)
 *
 * For string concatenation, use snprintf to concatenate safely.
 *
 * Returns: the result Value
 */
Value value_add(const Value *a, const Value *b)
{
    Value result;
    /* TODO: check types match, perform appropriate addition */
    result.type = VAL_INT;
    result.data.i = 0;
    (void)a; (void)b;
    return result;
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

static int fequal(float a, float b) { return (a - b) < 0.01f && (b - a) < 0.01f; }

int main(void)
{
    printf("=== Tagged Union ===\n\n");

    printf("constructors:\n");
    {
        Value vi = value_from_int(42);
        TEST("int value type is VAL_INT") EXPECT(vi.type == VAL_INT);
        TEST("int value data is 42") EXPECT(vi.data.i == 42);

        Value vf = value_from_float(3.14f);
        TEST("float value type is VAL_FLOAT") EXPECT(vf.type == VAL_FLOAT);
        TEST("float value data is 3.14") EXPECT(fequal(vf.data.f, 3.14f));

        Value vs = value_from_string("hello");
        TEST("string value type is VAL_STRING") EXPECT(vs.type == VAL_STRING);
        TEST("string value data is 'hello'") EXPECT(strcmp(vs.data.s, "hello") == 0);
    }

    printf("\nvalue_type_name:\n");
    {
        Value vi = value_from_int(0);
        Value vf = value_from_float(0);
        Value vs = value_from_string("");
        TEST("int type name is 'int'") EXPECT(strcmp(value_type_name(&vi), "int") == 0);
        TEST("float type name is 'float'") EXPECT(strcmp(value_type_name(&vf), "float") == 0);
        TEST("string type name is 'string'") EXPECT(strcmp(value_type_name(&vs), "string") == 0);
    }

    printf("\nvalue_to_string:\n");
    {
        char buf[64];
        Value vi = value_from_int(42);
        value_to_string(&vi, buf, sizeof(buf));
        TEST("int 42 → '42'") EXPECT(strcmp(buf, "42") == 0);

        Value vs = value_from_string("world");
        value_to_string(&vs, buf, sizeof(buf));
        TEST("string → 'world'") EXPECT(strcmp(buf, "world") == 0);
    }

    printf("\nvalue_add:\n");
    {
        Value a = value_from_int(10);
        Value b = value_from_int(20);
        Value r = value_add(&a, &b);
        TEST("10 + 20 = 30 (int)") EXPECT(r.type == VAL_INT && r.data.i == 30);

        Value s1 = value_from_string("hello ");
        Value s2 = value_from_string("world");
        Value sr = value_add(&s1, &s2);
        TEST("'hello ' + 'world' = 'hello world'")
            EXPECT(sr.type == VAL_STRING && strcmp(sr.data.s, "hello world") == 0);
    }

    printf("\n=== Results: %d / %d passed ===\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
