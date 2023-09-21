#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "sndkit/lil/lil.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#define BUF_SIZE 4096
#define MAXHOST 1024
#define MAXSERV 32

void mnort_loader(lil_t lil);
void mno_clean(lil_t lil);
static volatile int running = 1;
void mno_lua_load(lua_State *L);
void mno_lua_clean(lua_State *L);

static lil_value_t stop(lil_t lil, size_t argc, lil_value_t *argv)
{
    running = 0;
    return NULL;
}

static void loader(lil_t lil)
{
    mnort_loader(lil);
    lil_register(lil, "stop", stop);
}

static void setup_lua_eval(lua_State *L)
{
    int rc;

    rc = luaL_dostring(L,
        "function eval_default(str) load(str)() end "
        "eval = eval_default"
    );

    if (rc != LUA_OK) {
        const char *errstr;

        errstr = lua_tostring(L, 0);
        fprintf(stderr, "%s\n", errstr);
    }
}

static void eval(lua_State *L, const char *str)
{
    int rc;
    lua_getglobal(L, "eval");
    lua_pushstring(L, str);
    rc = lua_pcall(L, 1, 0, 0);

    if (rc != LUA_OK) {
        const char *errstr;

        errstr = lua_tostring(L, -1);
        fprintf(stderr, "error: %s\n", errstr);
    }
}

int mno_rtserver_loader(int argc,
                        char *argv[],
                        void (*loadfun)(lua_State*),
                        void (*cleanfun)(lua_State*))
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char *buf;
    char host[MAXHOST], service[MAXSERV];
    const char *port;
    lua_State *L;
    lil_t lil;

    /*
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    */

    if (argc < 2) {
        port = "6449";
    } else {
        port = argv[1];
    }

    buf = calloc(1, BUF_SIZE);

    L = luaL_newstate();
    luaL_openlibs(L);
    /* mno_lua_load(L); */
    loadfun(L);
    setup_lua_eval(L);

    lua_getglobal(L, "__lil");

    lil = lua_touserdata(L, -1);
    loader(lil);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, port, &hints, &result);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully bind(2).
       If socket(2) (or bind(2)) fails, we (close the socket
       and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    running = 1;

    while (running) {
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd,
                         buf, BUF_SIZE,
                         0,
                         (struct sockaddr *) &peer_addr,
                         &peer_addr_len);

        /* Ignore failed request */
        if (nread == -1) continue;

        s = getnameinfo((struct sockaddr *) &peer_addr,
                        peer_addr_len,
                        host,
                        MAXHOST,
                        service,
                        MAXSERV,
                        NI_NUMERICSERV);
        if (s == 0) {
            buf[nread] = '\0';
            eval(L, buf);
            /*
            int e;
            e = luaL_dostring(L, buf);
            if (e != LUA_OK) {
                fprintf(stderr,
                        "eval error: %s\n",
                        lua_tostring(L, -1));
            }
            */
        } else {
            fprintf(stderr,
                    "getnameinfo: %s\n", gai_strerror(s));
        }
    }

    /* mno_lua_clean(L); */
    cleanfun(L);
    lua_close(L);
    free(buf);
    return 0;
}

int mno_rtserver(int argc, char *argv[])
{
    return mno_rtserver_loader(argc,
                               argv,
                               mno_lua_load,
                               mno_lua_clean);
}
