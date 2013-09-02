#ifndef STRING_H
#define STRING_H

#include <stdint.h>

typedef struct string {
    char* buff;
    size_t len, cap;
}
string_t;

void
string_init(string_t* str);

void
string_init2(string_t* str, size_t capacity);

void
string_init_cstr(string_t* str, const char* cstr);

void
string_free(string_t* str);

void
string_clear(string_t* str);

void
string_putc(string_t* str, char c);

void
string_puts(string_t* str, const char* c, size_t len);

void
string_append(string_t* str, string_t* str2);

string_t*
string_dup(string_t* str);

#endif
