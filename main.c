#include <string.h>
#include <stdio.h>

/* mnolth + util = mnotil */

int cdb_main(int argc, char *argv[]);
int txt2cdb_main(int argc, char *argv[]);
int sqlite3_main(int argc, char *argv[]);
int sqlar_main(int argc, char *argv[]);
int uxnasm_main(int argc, char *argv[]);
int wavdraw_main(int argc, char *argv[]);
int samu_main(int argc, char *argv[]);
int mno_lua_main_noloader(int argc, char *argv[]);
int mno_lil_main_noloader(int argc, char *argv[]);
int mno_scm_main_noloader(int argc, char *argv[]);
int xmtowav_main(int argc, char** argv);
int msgpack_dump_main(int argc, char *argv[]);
int xmt_main(int argc, char *argv[]);
int run_cfloop(int argc, char *argv[]);
int ft2play_main(int argc, char *argv[]);

#ifdef BUILD_MNORT
int mno_rtserver(int argc, char *argv[]);
int mno_rtclient(int argc, char *argv[]);
#endif

struct mnotil_cmd {
    const char *name;
    int (*main)(int, char**);
};

static int helper(int argc, char *argv[]);

struct mnotil_cmd cmds[] = {
    {"cdb", cdb_main},
    {"txt2cdb", txt2cdb_main},
    {"sqlite", sqlite3_main},
    {"sqlar", sqlar_main},
    {"uxnasm", uxnasm_main},
    {"wavdraw", wavdraw_main},
    {"samu", samu_main},
    {"lua", mno_lua_main_noloader},
    {"lil", mno_lil_main_noloader},
    {"scheme", mno_scm_main_noloader},
#ifdef BUILD_MNORT
    {"server", mno_rtserver},
    {"client", mno_rtclient},
#endif
    {"help", helper},
    {"xmtowav", xmtowav_main},
    {"msgpack-dump", msgpack_dump_main},
    {"xmt", xmt_main},
    {"cfloop", run_cfloop},
    {"ft2play", ft2play_main},
};


static int helper(int argc, char *argv[])
{
    int ncmds;
    int i;

    ncmds = sizeof(cmds) / sizeof(*cmds);

    fprintf(stderr, "Available utilities:\n");

    for (i = 0; i < ncmds; i++) {
        fprintf(stderr, "%s\n", cmds[i].name);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int i;
    int ncmds;
    if (argc < 2) {
        fprintf(stderr, "enter a command, or run 'help'\n");
        return 1;
    }

    ncmds = sizeof(cmds) / sizeof(*cmds);

    for (i = 0; i < ncmds; i++) {
        struct mnotil_cmd *c;

        c = &cmds[i];
        if (!strcmp(argv[1], c->name)) {
            argc--;
            argv++;
            return c->main(argc, argv);
        }
    }

    fprintf(stderr,
            "not sure what '%s' is supposed to be\n",
            argv[1]);

    return 1;
}
