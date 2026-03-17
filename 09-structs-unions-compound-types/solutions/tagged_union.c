/*
 * Solution: Tagged Union (Variant Type)
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

Value value_from_int(int n)
{
    Value v;
    v.type = VAL_INT;
    v.data.i = n;
    return v;
}

Value value_from_float(float f)
{
    Value v;
    v.type = VAL_FLOAT;
    v.data.f = f;
    return v;
}

Value value_from_string(const char *s)
{
    Value v;
    v.type = VAL_STRING;
    strncpy(v.data.s, s, VAL_STR_MAX - 1);
    v.data.s[VAL_STR_MAX - 1] = '\0';
    return v;
}

const char *value_type_name(const Value *v)
{
    switch (v->type) {
        case VAL_INT:    return "int";
        case VAL_FLOAT:  return "float";
        case VAL_STRING: return "string";
    }
    return "unknown";
}

void value_to_string(const Value *v, char *buf, int buf_size)
{
    switch (v->type) {
        case VAL_INT:
            snprintf(buf, buf_size, "%d", v->data.i);
            break;
        case VAL_FLOAT:
            snprintf(buf, buf_size, "%.2f", v->data.f);
            break;
        case VAL_STRING:
            snprintf(buf, buf_size, "%s", v->data.s);
            break;
    }
}

Value value_add(const Value *a, const Value *b)
{
    if (a->type != b->type) {
        Value err;
        err.type = VAL_INT;
        err.data.i = 0;
        return err;
    }

    switch (a->type) {
        case VAL_INT:
            return value_from_int(a->data.i + b->data.i);
        case VAL_FLOAT:
            return value_from_float(a->data.f + b->data.f);
        case VAL_STRING: {
            Value result;
            result.type = VAL_STRING;
            strncpy(result.data.s, a->data.s, VAL_STR_MAX - 1);
            result.data.s[VAL_STR_MAX - 1] = '\0';
            {
                size_t len = strlen(result.data.s);
                if (len < VAL_STR_MAX - 1) {
                    strncpy(result.data.s + len, b->data.s, VAL_STR_MAX - 1 - len);
                    result.data.s[VAL_STR_MAX - 1] = '\0';
                }
            }
            return result;
        }
    }

    /* Fallback */
    Value err;
    err.type = VAL_INT;
    err.data.i = 0;
    return err;
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
    printf("=== Tagged Union (Solution) ===\n\n");

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
