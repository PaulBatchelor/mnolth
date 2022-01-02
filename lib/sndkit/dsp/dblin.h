#line 112 "dblin.org"
#ifndef SK_DBLIN_H
#define SK_DBLIN_H

#ifndef SKFLT
#define SKFLT float
#endif

#ifdef SK_DBLIN_PRIV
#line 150 "dblin.org"
struct sk_dblin {
    SKFLT c;
    SKFLT prev;
    SKFLT out;
};
#line 121 "dblin.org"
#endif

#line 142 "dblin.org"
typedef struct sk_dblin sk_dblin;
#line 121 "dblin.org"
#line 159 "dblin.org"
void sk_dblin_init(sk_dblin *dl);
#line 178 "dblin.org"
SKFLT sk_dblin_tick(sk_dblin *dl, SKFLT db);
#line 125 "dblin.org"
#endif
