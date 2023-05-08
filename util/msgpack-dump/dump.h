#include <stdlib.h>
#include "buffer.h"
#include "cmp/cmp.h"

#pragma once

typedef struct {
    buffer buffer;
    size_t position;
} msgpack_data;

msgpack_data make_msgpack_data(buffer buffer);

void msgpack_data_read(msgpack_data *backing_data, void* out_data, size_t length);
void msgpack_data_skip(msgpack_data *backing_data, size_t length);

void dump_msgpack(buffer rawdata, bool print_strings);
void dump_msgpack_object(msgpack_data *raw_data, cmp_object_t* object, bool print_strings);
void dump_msgpack_string(msgpack_data *raw_data, cmp_object_t* object, uint32_t length, bool print_strings);
