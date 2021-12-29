#line 27 "gestlang.org"
#ifndef GESTLANG_H
#define GESTLANG_H
#line 53 "gestlang.org"
typedef struct gestlang_d gestlang_d;
#line 328 "gestlang.org"
typedef struct gestlang_entry gestlang_entry;
#line 30 "gestlang.org"

#ifdef GESTLANG_PRIV
#line 58 "gestlang.org"
struct gestlang_d {
    char word[64];
    char pool[256];
    char *args[8];
    int argpos;
    int psz;
    int nargs;
    int wsz;
    int (*fun)(gest_d *,int,char **,void*);
    void *ud;
    int err;
#line 348 "gestlang.org"
gestlang_entry *ent[16];
#line 58 "gestlang.org"
#line 774 "gestlang.org"
int comment;
#line 71 "gestlang.org"
};
#line 333 "gestlang.org"
struct gestlang_entry {
    char word[16];
    int sz;
    int (*fun)(gest_d *,int,char **,void*);
    int nargs;
    void *ud;
    void (*del)(void *);
    gestlang_entry *nxt;
};
#line 33 "gestlang.org"
#endif

#line 76 "gestlang.org"
size_t gestlang_sizeof(void);
#line 91 "gestlang.org"
void gestlang_init(gestlang_d *glang);
#line 129 "gestlang.org"
void gestlang_bye(gestlang_d *glang);
#line 148 "gestlang.org"
int gestlang_parse(gestlang_d *glang,
                   gest_d *gest,
                   const char *buf,
                   int sz);

int gestlang_parse_buffer(gestlang_d *glang,
                          gest_d *gest,
                          const char *buf,
                          int sz);
#line 284 "gestlang.org"
int gestlang_load(gestlang_d *glang,
                  gest_d *gest,
                  const char *filename);
#line 386 "gestlang.org"
void gestlang_add(gestlang_d *glang,
                  const char *key,
                  int sz,
                  int nargs,
                  int (*fun)(gest_d *,int,char **,void*),
                  void *ud,
                  void (*del)(void *));
#line 463 "gestlang.org"
int gestlang_find(gestlang_d *glang,
                  const char *key,
                  int sz,
                  gestlang_entry **pent);
#line 498 "gestlang.org"
void gestlang_loader(gestlang_d *g);
#line 36 "gestlang.org"
#endif
