#include <string.h>
#include <stdio.h>
#include "dump.h"
#include "cmp/cmp.h"

msgpack_data make_msgpack_data(buffer buffer)
{
    msgpack_data data;
    data.buffer = buffer;
    data.position = 0;
    return data;
}

bool msgpack_data_reached_eof(msgpack_data *data)
{
    return data->position == data->buffer.length;
}

void msgpack_data_read(msgpack_data *backing_data, void* out_data, size_t length) {
    size_t new_end_position = backing_data->position + length;
    if (new_end_position > backing_data->buffer.length) {
        perror("Msgpack library tried to read more than the message's size. Message is probably malformed.\n");
        exit(4);
    }
    memcpy(out_data, backing_data->buffer.data + backing_data->position, length);
    backing_data->position = new_end_position;
}

void msgpack_data_skip(msgpack_data *backing_data, size_t length) {
    backing_data->position += length;
    if (backing_data->position > backing_data->buffer.length) {
        perror("Msgpack library tried to read more than the message's size. Message is probably malformed.\n");
        exit(4);
    }
}

static bool msgpack_reader(cmp_ctx_t *ctx, void *data, size_t limit) {
    msgpack_data_read((msgpack_data*)ctx->buf, data, limit);
    return true;
}

static bool msgpack_skipper(cmp_ctx_t *ctx, size_t count) {
    msgpack_data_skip((msgpack_data*)ctx->buf, count);
    return true;
}


void dump_msgpack(buffer rawdata, bool print_strings) {
    msgpack_data data = make_msgpack_data(rawdata);
    cmp_ctx_t ctx;

    cmp_init(&ctx, &data, msgpack_reader, msgpack_skipper, NULL);

    cmp_object_t obj;
    while (!msgpack_data_reached_eof(&data)) {
        if (!cmp_read_object(&ctx, &obj)) {
            perror("Messagepack library error\n");
            perror(cmp_strerror(&ctx));
            exit(5);
        }

        dump_msgpack_object(&data, &obj, print_strings);
    }
    //printf("%s", "done");
}

void dump_msgpack_object(msgpack_data *raw_data, cmp_object_t* object, bool print_strings) {
    switch (object->type) {
        case CMP_TYPE_POSITIVE_FIXNUM:
            printf("Positive fixnum: %d", object->as.u8);
            break;
        case CMP_TYPE_FIXMAP:
            printf("Map length : %d (fixed)", object->as.map_size);
            break;
        case CMP_TYPE_FIXARRAY:
            printf("Array length : %d (fixed)", object->as.array_size);
            break;
        case CMP_TYPE_FIXSTR:
            printf("String length : %d (fixed)", object->as.str_size);
            dump_msgpack_string(raw_data, object, object->as.str_size, print_strings);
            break;
        case CMP_TYPE_NIL:
            printf("Nil");
            break;
        case CMP_TYPE_BOOLEAN:
            printf("Boolean : %d", object->as.boolean);
            break;
        case CMP_TYPE_BIN8:
            printf("Data length : %d (8)", object->as.bin_size);
            msgpack_data_skip(raw_data, object->as.bin_size);
            break;
        case CMP_TYPE_BIN16:
            printf("Data length : %d (16)", object->as.bin_size);
            msgpack_data_skip(raw_data, object->as.bin_size);
            break;
        case CMP_TYPE_BIN32:
            printf("Data length : %d (32)", object->as.bin_size);
            msgpack_data_skip(raw_data, object->as.bin_size);
            break;
        case CMP_TYPE_EXT8:
        case CMP_TYPE_EXT16:
        case CMP_TYPE_EXT32:
            perror("Ext is not supported\n");
            exit(6);
            break;
        case CMP_TYPE_FLOAT:
            printf("Float : %f", object->as.flt);
            break;
        case CMP_TYPE_DOUBLE:
            printf("Double : %f", object->as.dbl);
            break;
        case CMP_TYPE_UINT8:
            printf("Integer : %ju (unsigned, 8)", (uintmax_t)object->as.u8);
            break;
        case CMP_TYPE_UINT16:
            printf("Integer : %ju (unsigned, 16)", (uintmax_t)object->as.u16);
            break;
        case CMP_TYPE_UINT32:
            printf("Integer : %ju (unsigned, 32)", (uintmax_t)object->as.u32);
            break;
        case CMP_TYPE_UINT64:
            printf("Integer : %ju (unsigned, 64)", (uintmax_t)object->as.u8);
            break;
        case CMP_TYPE_SINT8:
            printf("Integer : %jd (signed, 8)", (intmax_t)object->as.s8);
            break;
        case CMP_TYPE_SINT16:
            printf("Integer : %jd (signed, 16)", (intmax_t)object->as.s16);
            break;
        case CMP_TYPE_SINT32:
            printf("Integer : %jd (signed, 32)", (intmax_t)object->as.s32);
            break;
        case CMP_TYPE_SINT64:
            printf("Integer : %jd (signed, 64)", (intmax_t)object->as.s64);
            break;
        case CMP_TYPE_FIXEXT1:
        case CMP_TYPE_FIXEXT2:
        case CMP_TYPE_FIXEXT4:
        case CMP_TYPE_FIXEXT8:
        case CMP_TYPE_FIXEXT16:
            perror("Fixext is not supported\n");
            exit(6);
            break;
        case CMP_TYPE_STR8:
            printf("String length : %d (str8)", object->as.str_size);
            dump_msgpack_string(raw_data, object, object->as.str_size, print_strings);
            break;
        case CMP_TYPE_STR16:
            printf("String length : %d (str16)", object->as.str_size);
            dump_msgpack_string(raw_data, object, object->as.str_size, print_strings);
            break;
        case CMP_TYPE_STR32:
            printf("String length : %d (str32)", object->as.str_size);
            dump_msgpack_string(raw_data, object, object->as.str_size, print_strings);
            break;
        case CMP_TYPE_ARRAY16:
            printf("Array length : %d (array16)", object->as.array_size);
            break;
        case CMP_TYPE_ARRAY32:
            printf("Array length : %d (array32)", object->as.array_size);
            break;
        case CMP_TYPE_MAP16:
            printf("Map length : %d (map16)", object->as.map_size);
            break;
        case CMP_TYPE_MAP32:
            printf("Map length : %d (map32)", object->as.map_size);
            break;
        case CMP_TYPE_NEGATIVE_FIXNUM:
            printf("%s: %d", "Negative fixnum", object->as.s8);
            break;
        default:
            printf("%s", "Error: unknown type");
            break;
    }
    printf("\n");
}

void dump_msgpack_string(msgpack_data *raw_data, cmp_object_t* object, uint32_t length, bool print_strings) {
    char *str = malloc(length);
    msgpack_data_read(raw_data, str, length);

    if (print_strings) {
        fprintf(stdout, "\n\"");
        fwrite(str, 1, length, stdout);
        fprintf(stdout, "\"");
    }

    free(str);
}
