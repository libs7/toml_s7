#include "liblogc.h"
#include "unity.h"
#include "utarray.h"
#include "utstring.h"

#include "libs7.h"

#include "s7plugin_test_config.h"
#include "macros.h"

#include "toml_maps_test.h"

s7_scheme *s7;

extern struct option options[];

s7_pointer b, t, k, a, idx, actual, expected;
s7_pointer  res, res1, res2;
s7_pointer flag, len, m;

char *expected_str;

bool verbose;
bool debug;

#if defined(PROFILE_fastbuild)
#define DEBUG_LEVEL toml_s7_debug
#define TRACE_FLAG toml_s7_trace
extern int  DEBUG_LEVEL;        /* defined in libtoml_s7.c */
extern bool TRACE_FLAG;        /* defined in libtoml_s7.c */

#define S7_DEBUG_LEVEL libs7_debug
extern int libs7_debug;
extern int s7plugin_debug;
#endif

char *cmd;

/* #define TOML_READ(s) \ */
/*     s7_apply_function(s7, s7_name_to_value(s7, "toml:read"),    \ */
/*                       s7_list(s7, 1, s7_eval_c_string(s7, s))); */

#define APPLY_1(f, o) \
 s7_apply_function(s7, s7_name_to_value(s7, f),    \
                       s7_list(s7, 1, o))

#define APPLY_2(f, o, k)                             \
 s7_apply_function(s7, s7_name_to_value(s7, f),    \
                   s7_list(s7, 2, o, k))

    /* s7_apply_function_star(s7, s7_name_to_value(s7, f), \ */
    /*                            s7_list(s7, 1, v)) */

/* WARNING: setUp and tearDown are run once per test. */
void setUp(void) {
    /* log_info("setup"); */
}

void tearDown(void) {
    /* log_info("teardown"); */
}

/*
  (toml:read)
  (toml:read "foo")
  (toml:read "foo")
 */
/* (define tlt (toml:read "v = [0, 1, 2]")) */
/* tlt is (a nameless) table, NOT a kv pair!  */
void root_tables(void) {
    t = TOML_READ("m = true");
    LOG_S7_DEBUG(0, "tt:", t);
    actual = APPLY_1("toml:map?", t);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));
    k = s7_make_string(s7, "m");
    a = APPLY_2("toml:map-ref", t, k);
    LOG_S7_DEBUG(0, "a:", a);
    flag = APPLY_1("boolean?", a);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    t = TOML_READ("m = 123");
    flag = APPLY_1("toml:map?", t);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    k = s7_make_string(s7, "m");
    a = APPLY_2("toml:map-ref", t, k);
    flag = APPLY_1("integer?", a);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    t = TOML_READ("m = 1.23");
    actual = APPLY_1("toml:map?", t);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));
    k = s7_make_string(s7, "m");
    a = APPLY_2("toml:map-ref", t, k);
    actual = APPLY_1("real?", a);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    t = TOML_READ("m = \"Hello\"");
    actual = APPLY_1("toml:map?", t);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));
    k = s7_make_string(s7, "m");
    a = APPLY_2("toml:map-ref", t, k);
    actual = APPLY_1("string?", a);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    t = TOML_READ("m = [1, 2, 3]");
    actual = APPLY_1("toml:map?", t);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));
    k = s7_make_string(s7, "m");
    a = APPLY_2("toml:map-ref", t, k);
    actual = APPLY_1("toml:array?", a);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    t = TOML_READ("m = { a = 1, b = 2}");
    actual = APPLY_1("toml:map?", t);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));
    k = s7_make_string(s7, "m");
    a = APPLY_2("toml:map-ref", t, k);
    actual = APPLY_1("toml:map?", a);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));
}

void table_refs(void) {
    t = TOML_READ("m = \"Hello\"");
    flag = APPLY_1("toml:map?", t);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    // string key
    k = s7_make_string(s7, "m");
    res1 = APPLY_2("toml:map-ref", t, k);
    flag = APPLY_1("string?", res1);
    TEST_ASSERT_TRUE(s7_boolean(s7, res1));

    // obj application
    res2 = APPLY_OBJ(t, k);
    flag = APPLY_1("string?", res2);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, res1, res2));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "eqv?"),
                             s7_list(s7, 2, res1, res2));
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "eq?"),
                             s7_list(s7, 2, res1, res2));
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    // symbol key
    k = s7_make_symbol(s7, "m");
    res1 = APPLY_2("toml:map-ref", t, k);
    flag = APPLY_1("string?", res1);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    // obj application
    res2 = APPLY_OBJ(t, k);
    flag = APPLY_1("string?", res2);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, res1, res2));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "eqv?"),
                             s7_list(s7, 2, res1, res2));
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "eq?"),
                             s7_list(s7, 2, res1, res2));
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    // keyword key
    k = s7_make_keyword(s7, "m");
    res1 = APPLY_2("toml:map-ref", t, k);
    flag = APPLY_1("string?", res1);
    TEST_ASSERT_TRUE(s7_boolean(s7, res1));

    // obj application
    res2 = APPLY_OBJ(t, k);
    flag = APPLY_1("string?", res2);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, res1, res2));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "eqv?"),
                             s7_list(s7, 2, res1, res2));
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));
    flag = s7_apply_function(s7, s7_name_to_value(s7, "eq?"),
                             s7_list(s7, 2, res1, res2));
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));
}

/*
 * WARNING: tomlc99 table count ops are typed:
 * ntab for table-valued entries, narr for array-valued entries,
 * and nkv for "key-values", i.e. entries with 'atomic' values
 * so toml:map-length must sum the three: ntab + narr + nkv
 */
void table_length_ops(void) {
    t = TOML_READ(""
        "m = { b = true, s = \"Hello!\", "
        "        i = 0, f = 1.2, "
                  "        t = { t1 = 1 }, a = [0, 1, 2] }");
    actual = APPLY_1("toml:map?", t);
    /* TEST_ASSERT_EQUAL(actual, s7_t(s7)); */

    k = s7_make_string(s7, "m");
    s7_pointer m = APPLY_2("toml:map-ref", t, k);
    actual = APPLY_1("toml:map?", m);
    /* TEST_ASSERT_EQUAL(actual, s7_t(s7)); */

    actual = APPLY_1("toml:map-length", m);
    TEST_ASSERT_EQUAL(6, s7_integer(actual));

    actual = APPLY_1("toml:map-nkval", m);
    TEST_ASSERT_EQUAL(4, s7_integer(actual));
    /* aliases nkval */
    actual = APPLY_1("toml:map-atomic-count", m);
    TEST_ASSERT_EQUAL(4, s7_integer(actual));

    actual = APPLY_1("toml:map-ntab", m);
    TEST_ASSERT_EQUAL(1, s7_integer(actual));
    /* aliases ntab */
    actual = APPLY_1("toml:map-subtable-count", m);
    TEST_ASSERT_EQUAL(1, s7_integer(actual));

    actual = APPLY_1("toml:map-narr", m);
    TEST_ASSERT_EQUAL(1, s7_integer(actual));
    /* aliases narr */
    actual = APPLY_1("toml:map-array-count", m);
    TEST_ASSERT_EQUAL(1, s7_integer(actual));
}

void table_ops(void) {
    t = TOML_READ(""
        "m = { b = true, s = \"Hello!\", "
        "        i = 0, f = 1.2, "
                  "        t = { t1 = 1 }, a = [0, 1, 2] }");
    actual = APPLY_1("toml:map?", t);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    /* root tables have empty key  */
    actual = APPLY_1("toml:map-key", t);
    TEST_ASSERT_EQUAL_STRING("", s7_string(actual));

    k = s7_make_string(s7, "m");
    s7_pointer m = APPLY_2("toml:map-ref", t, k);
    actual = APPLY_1("toml:map?", m);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    actual = APPLY_1("toml:map-key", m);
    TEST_ASSERT_EQUAL_STRING("m", s7_string(actual));
    actual = APPLY_1("toml:map-keys", m);
    s7_pointer v = APPLY_1("list?", actual);
    TEST_ASSERT_EQUAL(s7_t(s7), v);

    //FIXME: test toml:map-key-for-index ('key_in')

}

void table_serialization(void) {
    /* t = TOML_READ("m = [1, 2, 3]"); */
    /* t = TOML_READ("k1 = 1\nk2 = true\nk3='Hello'"); */

    t = TOML_READ("k1 = 7\nk2 = 8");

    /* actual = APPLY_FORMAT("~A", t); */
    res = s7_apply_function(s7, s7_name_to_value(s7, "object->string"),
                            s7_list(s7, 1, t));
    LOG_S7_DEBUG(0, "obj->s:", res);

    /* res = s7_apply_function(s7, s7_name_to_value(s7, "format"), */
    /*                         s7_list(s7, 3, */
    /*                                 s7_t(s7), */
    /*                                 s7_make_string(s7, "~A"), */
    /*                                 t)); */
    /* log_debug("xxxxxxxxxxxxxxxx"); */
    /* LOG_S7_DEBUG(0, "fmt:", res); */
    /* TEST_ASSERT_EQUAL(actual, s7_t(s7)); */

    /* /\* root tables have empty key  *\/ */
    /* actual = APPLY_1("toml:map-key", t); */
    /* TEST_ASSERT_EQUAL_STRING("", s7_string(actual)); */

    /* k = s7_make_string(s7, "m"); */
    /* s7_pointer m = APPLY_2("toml:map-ref", t, k); */
    /* actual = APPLY_1("toml:map?", m); */
    /* TEST_ASSERT_EQUAL(actual, s7_t(s7)); */

    /* actual = APPLY_1("toml:map-key", m); */
    /* TEST_ASSERT_EQUAL_STRING("m", s7_string(actual)); */
    /* actual = APPLY_1("toml:map-keys", m); */
    /* s7_pointer v = APPLY_1("list?", actual); */
    /* TEST_ASSERT_EQUAL(s7_t(s7), v); */

    /* //FIXME: test toml:map-key-for-index ('key_in') */
}

void to_string_atoms(void) {
    // bools
    t = TOML_READ("k1 = true\nk2 = false");
    res = APPLY_1("object->string", t);
    TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = true, k2 = false>",
                             s7_string(res));
    // doubles
    t = TOML_READ("k1 = 1.2\nk2 = 3.4");
    res = APPLY_1("object->string", t);
    TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = 1.2, k2 = 3.4>",
                             s7_string(res));
    // ints
    t = TOML_READ("k1 = 7\nk2 = 8");
    res = APPLY_1("object->string", t);
    TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = 7, k2 = 8>",
                             s7_string(res));

    // strings
    t = TOML_READ("k1 = 'Hi there'\nk2 = ', World'");
    res = APPLY_1("object->string", t);
    TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = 'Hi there', k2 = ', World'>",
                             s7_string(res));

    /* // timestamps (not yet) */
    /* t = TOML_READ("k1 = 'Hi there'\nk2 = ', World'"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = 'Hi there', k2 = ', World'>", */
    /*                          s7_string(res)); */
}

void to_string_arrays(void) {
    // bool arrays
    t = TOML_READ("ba = [true, false]");
    res = APPLY_1("object->string", t);
    LOG_S7_DEBUG(0, "obj->s:", res);
    TEST_ASSERT_EQUAL_STRING("#<toml-table ba = #<toml-array true, false>>",
                             s7_string(res));
    // int arrays
    t = TOML_READ("ia = [0, 1, 2]");
    res = APPLY_1("object->string", t);
    TEST_ASSERT_EQUAL_STRING("#<toml-table ia = #<toml-array 0, 1, 2>>",
                             s7_string(res));
    // double arrays
    t = TOML_READ("da = [1.2, 3.4]");
    res = APPLY_1("object->string", t);
    LOG_S7_DEBUG(0, "obj->s:", res);
    TEST_ASSERT_EQUAL_STRING("#<toml-table da = #<toml-array 1.2, 3.4>>",
                             s7_string(res));

    // string arrays
    t = TOML_READ("sa = ['Hey there', 'you old world']");
    res = APPLY_1("object->string", t);
    LOG_S7_DEBUG(0, "obj->s:", res);
    TEST_ASSERT_EQUAL_STRING("#<toml-table sa = #<toml-array \"Hey there\", \"you old world\">>",
                             s7_string(res));

    /* // timestamp arrays (not yet) */
    /* t = TOML_READ("k1 = 'Hi there'\nk2 = ', World'"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = \"Hi there\", k2 = \", World\">", */
    /*                          s7_string(res)); */
}

void to_string_subtables(void) {
    // bool values
    t = TOML_READ("a1 = { a1b1 = true }\na2 = 9");
    res = APPLY_1("object->string", t);
    LOG_S7_DEBUG(0, "obj->s:", res);
    TEST_ASSERT_EQUAL_STRING("#<toml-table a2 = 9, a1 = #<toml-table a1b1 = true>>",
                             s7_string(res));
    /* // int arrays */
    /* t = TOML_READ("ia = [0, 1, 2]"); */
    /* res = APPLY_1("object->string", t); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table ia = [0, 1, 2]>", */
    /*                          s7_string(res)); */
    /* // double arrays */
    /* t = TOML_READ("da = [1.2, 3.4]"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table da = [1.2, 3.4]>", */
    /*                          s7_string(res)); */

    /* // string arrays */
    /* t = TOML_READ("sa = ['Hey there', 'you old world']"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table sa = ['Hey there', 'you old world']>", */
    /*                          s7_string(res)); */

    /* // timestamp arrays (not yet) */
    /* t = TOML_READ("k1 = 'Hi there'\nk2 = ', World'"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = 'Hi there', k2 = ', World'>", */
    /*                          s7_string(res)); */
}

void to_string_mixed(void) {
    t = TOML_READ("a1 = [ {a1b1 = [1, 2] } ]");
    res = APPLY_1("object->string", t);
    LOG_S7_DEBUG(0, "obj->s:", res);
    expected_str = "#<toml-table a1 = #<toml-array #<toml-table a1b1 = #<toml-array 1, 2>>>>";
    TEST_ASSERT_EQUAL_STRING(expected_str, s7_string(res));

    /* t = TOML_READ("a1 = { a1b1 = [1, 2] }\na2 = [{a2b1=true},{a2b2=99}]"); */


    /* // int arrays */
    /* t = TOML_READ("ia = [0, 1, 2]"); */
    /* res = APPLY_1("object->string", t); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table ia = [0, 1, 2]>", */
    /*                          s7_string(res)); */
    /* // double arrays */
    /* t = TOML_READ("da = [1.2, 3.4]"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table da = [1.2, 3.4]>", */
    /*                          s7_string(res)); */

    /* // string arrays */
    /* t = TOML_READ("sa = ['Hey there', 'you old world']"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table sa = ['Hey there', 'you old world']>", */
    /*                          s7_string(res)); */

    /* // timestamp arrays (not yet) */
    /* t = TOML_READ("k1 = 'Hi there'\nk2 = ', World'"); */
    /* res = APPLY_1("object->string", t); */
    /* LOG_S7_DEBUG(0, "obj->s:", res); */
    /* TEST_ASSERT_EQUAL_STRING("#<toml-table k1 = 'Hi there', k2 = ', World'>", */
    /*                          s7_string(res)); */
}

void dotted_keys(void) {
    t = TOML_READ("fruit.apple.color = \"red\"");
    actual = APPLY_1("toml:map?", t);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    k = s7_make_string(s7, "fruit");
    s7_pointer m = APPLY_2("toml:map-ref", t, k);
    actual = APPLY_1("toml:map?", m);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));
    actual = APPLY_1("toml:map-key", m);
    TEST_ASSERT_EQUAL_STRING("fruit", s7_string(actual));

    k = s7_make_string(s7, "apple");
    actual = APPLY_2("toml:map-ref", m, k);
    actual = APPLY_1("toml:map?", actual);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    len = APPLY_1("toml:map-length", m);
    TEST_ASSERT_EQUAL(1, s7_integer(len));
    len = APPLY_1("toml:map-array-count", m);
    TEST_ASSERT_EQUAL(0, s7_integer(len));
    len = APPLY_1("toml:map-subtable-count", m);
    TEST_ASSERT_EQUAL(1, s7_integer(len));
    len = APPLY_1("toml:map-atomic-count", m);
    TEST_ASSERT_EQUAL(0, s7_integer(len));

    k = s7_make_string(s7, "apple");
    b = APPLY_2("toml:key-exists?", m, k);
    TEST_ASSERT_EQUAL(s7_t(s7), b);
    /* alias */
    b = APPLY_2("toml:map-contains?", m, k);
    TEST_ASSERT_EQUAL(s7_t(s7), b);


    m = APPLY_2("toml:map-ref", m, k);
    actual = APPLY_1("toml:map?", m);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    k = s7_make_string(s7, "color");
    m = APPLY_2("toml:map-ref", m, k);
    actual = APPLY_1("string?", m);
    TEST_ASSERT_EQUAL(actual, s7_t(s7));

    /* actual = APPLY_1("toml:map-length", m); */
    /* TEST_ASSERT_EQUAL_STRING(1, s7_string(actual)); */
}

int main(int argc, char **argv)
{
    s7 = s7_plugin_initialize("maps", argc, argv);

    libs7_load_plugin(s7, "toml");

    UNITY_BEGIN();

    RUN_TEST(root_tables);
    RUN_TEST(table_refs);
    RUN_TEST(table_length_ops);
    RUN_TEST(table_ops);
    RUN_TEST(table_serialization);

    RUN_TEST(to_string_atoms);
    RUN_TEST(to_string_arrays);
    RUN_TEST(to_string_subtables);
    RUN_TEST(to_string_mixed);
    RUN_TEST(dotted_keys);

    return UNITY_END();
    s7_quit(s7);
    s7_free(s7);
}
