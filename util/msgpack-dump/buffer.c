#include <stdio.h>
#include <string.h>
#include "buffer.h"

buffer make_buffer()
{
    buffer b;
    b.data = NULL;
    b.length = 0;
    return b;
}

void buffer_append(buffer* buffer, char* data, size_t length)
{
    size_t offset = buffer->length;
    buffer->length += length;

    char* newdata = realloc(buffer->data, buffer->length);
    if (newdata == NULL) {
        perror("Unable to allocate memory");
        exit(2);
    }
    memcpy(newdata + offset, data, length);
    buffer->data = newdata;
}

