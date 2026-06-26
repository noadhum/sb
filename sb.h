#ifndef SB_H_
#define SB_H_

#include <stdbool.h>

typedef struct {
     char *data;
     size_t count;
     size_t capacity;
} String_Builder;

#ifndef SBDEF
#define SBDEF
#endif // SBDEF

SBDEF String_Builder sb_from_cstr(const char *cstr);
SBDEF void sb_append(String_Builder *sb, const char *cstr);
SBDEF void sb_append_char(String_Builder *sb, char c);
SBDEF void sb_clear(String_Builder *sb);
SBDEF bool sb_equal(const String_Builder *a, const String_Builder *b);
SBDEF bool sb_equal_ignorecase(const String_Builder *a, const String_Builder *b);
SBDEF void sb_reserve(String_Builder *sb, size_t new_capacity);
SBDEF void sb_free(String_Builder *sb);

#endif // SB_H_

#ifdef SB_IMPLEMENTATION

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SB_REALLOC
#define SB_REALLOC realloc
#endif // SB_REALLOC

#ifndef SB_FREE
#define SB_FREE free
#endif // SB_FREE


#define SB_PANIC(expr, message)                                         \
     do {                                                               \
          if (!(expr)) {                                                \
               fprintf(stderr, "Error in: file %s, function %s, line %d, Message: %s\n", __FILE__, __func__, __LINE__, message); \
               abort();                                                 \
          }                                                             \
     } while (0)

#ifndef SB_INIT_CAP
#define SB_INIT_CAP 64
#endif // SB_INIT_CAP

SBDEF String_Builder sb_from_cstr(const char *cstr)
{
     String_Builder sb = {0};
     sb_append(&sb, cstr);

     return sb;
}

SBDEF void sb_append(String_Builder *sb, const char *cstr)
{
     if (cstr == NULL) return;

     size_t cstr_length = strlen(cstr);
     sb_reserve(sb, sb->count + cstr_length + 1);
     memcpy(sb->data + sb->count, cstr, cstr_length);

     sb->count += cstr_length;
     sb->data[sb->count] = '\0';
}

SBDEF void sb_append_char(String_Builder *sb, char c)
{
     sb_reserve(sb, sb->count + 2);
     sb->data[sb->count] = c;

     sb->count++;
     sb->data[sb->count] = '\0';
}

SBDEF void sb_clear(String_Builder *sb)
{
     if (sb->data != NULL) {
          sb->data[0] = '\0';
     }
     sb->count = 0;
}

SBDEF bool sb_equal(const String_Builder *a, const String_Builder *b)
{
     if (a->count != b->count) return false;
     return memcmp(a->data, b->data, a->count) == 0;
}

SBDEF bool sb_equal_ignorecase(const String_Builder *a, const String_Builder *b)
{
     if (a->count != b->count) return false;

     unsigned char *ua = (unsigned char *)a->data;
     unsigned char *ub = (unsigned char *)b->data;
     for (size_t n = 0; n < a->count; n++) {
          if (tolower(ua[n]) != tolower(ub[n])) {
               return false;
          }
     }
     return true;
}

SBDEF void sb_reserve(String_Builder *sb, size_t new_capacity)
{
     if (new_capacity > sb->capacity) {
          if (sb->capacity == 0) sb->capacity = SB_INIT_CAP;
          while (new_capacity > sb->capacity) {
               sb->capacity *= 2;
          }
          sb->data = SB_REALLOC(sb->data, sb->capacity * sizeof(char));
          SB_PANIC(sb->data != NULL, "Not enough memory :(");
     }
}

SBDEF void sb_free(String_Builder *sb)
{
     SB_FREE(sb->data);
     sb->data = NULL;
     sb->count = 0;
     sb->capacity = 0;
}

#endif // SB_IMPLEMENTATION
