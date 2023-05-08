#include <stdlib.h>

#pragma once

typedef struct {
    size_t length;
    char *data;
} buffer;

buffer make_buffer();
void buffer_append(buffer* buffer, char* data, size_t length);
