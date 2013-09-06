#ifndef CHANNEL_H
#define CHANNEL_H

#include "string.h"

typedef struct {
    string_t* name;
    string_t* topic;
}
channel_t;

#endif
