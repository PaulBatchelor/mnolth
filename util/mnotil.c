#include <string.h>
#include <stdio.h>

/* mnolth + util = mnotil */

int cdb_main(int argc, char *argv[]);
int txt2cdb_main(int argc, char *argv[]);
int sqlite3_main(int argc, char *argv[]);
int sqlar_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "enter a command\n");
        return 1;
    } else if (!strcmp(argv[1], "cdb")) {
        argc--;
        argv++;
        return cdb_main(argc, argv);
    } else if (!strcmp(argv[1], "txt2cdb")) {
        argc--;
        argv++;
        return txt2cdb_main(argc, argv);
    } else if (!strcmp(argv[1], "sqlite")) {
        argc--;
        argv++;
        return sqlite3_main(argc, argv);
    } else if (!strcmp(argv[1], "sqlar")) {
        argc--;
        argv++;
        return sqlar_main(argc, argv);
    }

    fprintf(stderr,
            "not sure what '%s' is supposed to be\n",
            argv[1]);

    return 1;
}
