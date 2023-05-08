#include <stdio.h>
#include <string.h>

#include "dump.h"
#include "buffer.h"

#define BUFFER_SIZE 2048

buffer read_all_stdin() {
    buffer out_data = make_buffer();

    char buffer[BUFFER_SIZE];

    size_t amount_read;
    while ((amount_read = fread(buffer, 1, BUFFER_SIZE, stdin)) > 0) {
        buffer_append(&out_data, buffer, amount_read);
    };

    if (ferror(stdin)) {
        perror("Could not read stdin");
        exit(3);
    }

    return out_data;
}

int msgpack_dump_main(int argc, char *argv[]) {

    int print_strings = 0;

    if (argc > 1) {
        if (!strcmp(argv[1], "--print-strings")) {
            print_strings = 1;
        }
    }

    buffer b = read_all_stdin();
    dump_msgpack(b, print_strings);
    printf("\n");
    return 0;
}
