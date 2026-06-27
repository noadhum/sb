#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SB_IMPLEMENTATION
#include "sb.h"

/*
  Assert functions
  Heavily inspired by rexim's sv/test.c - https://github.com/tsoding/sv/blob/master/test.c
 */

typedef const char *cstr;

void sb_assert_equal_cstr(const char *file, int line,
                          const char *expected_expr, cstr expected,
                          const char *actual_expr, cstr actual)
{
     if (strcmp(expected, actual) != 0) {
          fprintf(stderr, "%s:%d: FAILED: %s == %s\n",
                  file, line, expected_expr, actual_expr);
          fprintf(stderr, "  EXPECTED: %s\n", expected);
          fprintf(stderr, "  ACTUAL:   %s\n", actual);
          exit(1);
     }
}

void sb_assert_equal_size_t(const char *file, int line,
                            const char *expected_expr, size_t expected,
                            const char *actual_expr, size_t actual)
{
     if (expected != actual) {
          fprintf(stderr, "%s:%d: FAILED: %s == %s\n",
                  file, line, expected_expr, actual_expr);
          fprintf(stderr, "  EXPECTED: %zu\n", expected);
          fprintf(stderr, "  ACTUAL:   %zu\n", actual);
          exit(1);
     }
}

#define SB_ASSERT_EQUAL(type, expected, actual) sb_assert_equal_##type(__FILE__, __LINE__, #expected, expected, #actual, actual)

void sb_assert_true(const char *file, int line,
                    const char *expr_cstr, bool expr)
{
     if (!expr) {
          fprintf(stderr, "%s:%d: FAILED: %s\n", file, line, expr_cstr);
          exit(1);
     }
}

#define SB_ASSERT_TRUE(expr) sb_assert_true(__FILE__, __LINE__, #expr, expr)

// Test Functions

void test_sb_from_cstr(void)
{
     String_Builder sb = sb_from_cstr("hello");
     SB_ASSERT_EQUAL(size_t, 5, sb.count);
     SB_ASSERT_EQUAL(cstr, "hello", sb.data);
}

void test_sb_append(void)
{
     String_Builder sb = {0};
     sb_append(&sb, "");
     SB_ASSERT_EQUAL(size_t, 0, sb.count);
     SB_ASSERT_EQUAL(cstr, "", sb.data);

     sb_append(&sb, "hello");
     SB_ASSERT_EQUAL(size_t, 5, sb.count);
     SB_ASSERT_EQUAL(cstr, "hello", sb.data);

     sb_append(&sb, "\nworld");
     SB_ASSERT_EQUAL(size_t, 11, sb.count);
     SB_ASSERT_EQUAL(cstr, "hello\nworld", sb.data);
}

void test_sb_append_itself(void)
{
     String_Builder sb = {0};
     sb_append(&sb, sb.data);
     SB_ASSERT_EQUAL(size_t, 0, sb.count);
     SB_ASSERT_TRUE(sb.data == NULL);

     sb_append(&sb, "hello");
     sb_append(&sb, sb.data);
     SB_ASSERT_EQUAL(cstr, "hellohello", sb.data);
}

void test_sb_append_char(void)
{
     String_Builder sb = {0};
     sb_append_char(&sb, 'h');
     SB_ASSERT_EQUAL(cstr, "h", sb.data);
     SB_ASSERT_EQUAL(size_t, 1, sb.count);

     sb_append_char(&sb, 'i');
     SB_ASSERT_EQUAL(cstr, "hi", sb.data);
     SB_ASSERT_EQUAL(size_t, 2, sb.count);
}

void test_sb_append_char_many(void)
{
     String_Builder sb = {0};
     char cstr[] = "hello world";

     for (char *c = cstr; *c != '\0'; c++) {
          sb_append_char(&sb, *c);
     }

     SB_ASSERT_EQUAL(cstr, "hello world", sb.data);
     SB_ASSERT_EQUAL(size_t, 11, sb.count);
}

void test_sb_clear(void)
{
     String_Builder sb = sb_from_cstr("hello");
     sb_clear(&sb);
     SB_ASSERT_EQUAL(size_t, 0, sb.count);
}

void test_sb_clear_and_append(void)
{
     String_Builder sb = sb_from_cstr("hello");
     sb_clear(&sb);

     sb_append(&sb, "world");
     SB_ASSERT_EQUAL(cstr, "world", sb.data);
     SB_ASSERT_EQUAL(size_t, 5, sb.count);
}

void test_sb_equal_ignorecase(void)
{
     String_Builder a = sb_from_cstr("hello");
     String_Builder b = sb_from_cstr("HELLO");
     SB_ASSERT_TRUE(sb_equal_ignorecase(&a, &b));

     sb_append_char(&b, 'W');
     SB_ASSERT_TRUE(!sb_equal_ignorecase(&a, &b));
}

void test_sb_equal_with_cstr(void)
{
     String_Builder sb = sb_from_cstr("hello");
     SB_ASSERT_TRUE(sb_equal_with_cstr(&sb, "hello"));

     sb_append(&sb, "world");
     SB_ASSERT_TRUE(sb_equal_with_cstr(&sb, "helloworld"));
}

void test_sb_equal_with_cstr_ignorecase(void)
{
     String_Builder sb = sb_from_cstr("hello");
     SB_ASSERT_TRUE(sb_equal_with_cstr_ignorecase(&sb, "HELLO"));
     SB_ASSERT_TRUE(!sb_equal_with_cstr_ignorecase(&sb, "HELLOW"));
}

void test_sb_reserve(void)
{
     String_Builder sb = {0};
     sb_reserve(&sb, 128);
     SB_ASSERT_EQUAL(size_t, 128, sb.capacity);

     sb_reserve(&sb, 0);
     SB_ASSERT_EQUAL(size_t, 128, sb.capacity);
}

void test_sb_free(void)
{
     String_Builder sb = sb_from_cstr("hello");
     sb_free(&sb);
     SB_ASSERT_TRUE(sb.data == NULL);
     SB_ASSERT_EQUAL(size_t, 0, sb.count);
     SB_ASSERT_EQUAL(size_t, 0, sb.capacity);

     sb_free(&sb);
     SB_ASSERT_TRUE(sb.data == NULL);
     SB_ASSERT_EQUAL(size_t, 0, sb.count);
     SB_ASSERT_EQUAL(size_t, 0, sb.capacity);
}

int main(void)
{
     test_sb_from_cstr();

     test_sb_append();
     test_sb_append_itself();
     test_sb_append_char();
     test_sb_append_char_many();

     test_sb_clear();
     test_sb_clear_and_append();

     test_sb_equal_ignorecase();

     test_sb_equal_with_cstr();

     test_sb_reserve();

     test_sb_free();

     return 0;
}
