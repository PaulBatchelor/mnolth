#include <string.h>
#include <stdio.h>

int mno_rtclient(int argc, char *argv[]);
int mno_rtserver(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [server|client]\n", argv[0]);
        return 1;
    }

    argc--;
    argv++;

    if (!strcmp(argv[0], "server")) {
        return mno_rtserver(argc, argv);
    } else if (!strcmp(argv[0], "client")) {
        return mno_rtclient(argc, argv);
    }

    fprintf(stderr, "Invalid command: %s\n", argv[0]);

    return 1;
}
