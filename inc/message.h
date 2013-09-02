#ifndef MESSAGE_H
#define MESSAGE_H

#include "string.h"

typedef enum {
    COMMAND_NUMBER,
    COMMAND_STRING,
}
command_type_t;

typedef struct {
    command_type_t type;
    union {
        int number;
        string_t string;
    } as;
}
command_t;

typedef struct {
    string_t* params;
    size_t param_count;
}
params_t;

typedef struct {
    prefix_t* prefix;
    command_t command;
    params_t params;
}
message_t;

#endif
