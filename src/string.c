#include <stdlib.h>
#include <string.h>
#include "string.h"

void
string_init(string_t* str)
{
    string_init2(str, 4);
}

void
string_init2(string_t* str, size_t capacity)
{
    if(capacity == 0) {
        capacity = 1;
    }
    str->len = 0;
    str->cap = capacity;
    str->buff = malloc(str->cap);
    str->buff[0] = 0;
}

void
string_init_cstr(string_t* str, const char* cstr)
{
    size_t len = strlen(cstr);
    string_init2(str, len + 1);
    string_puts(str, cstr, len);
}

void
string_free(string_t* str)
{
    free(str->buff);
    str->len = 0;
    str->cap = 0;
    str->buff = NULL;
}

void
string_clear(string_t* str)
{
    string_free(str);
    string_init(str);
}

void
string_putc(string_t* str, char c)
{
    string_puts(str, &c, 1);
}

void
string_puts(string_t* str, const char* buff, size_t len)
{
    int resized = 0;
    while(str->len + len + 1 >= str->cap) {
        str->cap *= 2;
        resized = 1;
    }
    if(resized) {
        str->buff = realloc(str->buff, str->cap);
    }
    memcpy(str->buff + str->len, buff, len);
    str->len += len;
    str->buff[str->len] = 0;
}

void
string_append(string_t* str, string_t* str2)
{
    string_puts(str, str2->buff, str2->len);
}

string_t*
string_dup(string_t* str)
{
    string_t* str2 = malloc(sizeof(*str2));
    str2->buff = malloc(str->cap);
    str2->len = str->len;
    str2->cap = str->cap;
    memcpy(str2->buff, str->buff, str->cap);
    return str2;
}
