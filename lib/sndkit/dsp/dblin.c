#line 130 "dblin.org"
#include <math.h>

#define SK_DBLIN_PRIV
#include "dblin.h"

#line 164 "dblin.org"
void sk_dblin_init(sk_dblin *dl)
{
    dl->c = log(10) / 20.0;
    dl->prev = 0;
    dl->out = 1;
}
#line 187 "dblin.org"
SKFLT sk_dblin_tick(sk_dblin *dl, SKFLT db)
{
    SKFLT out;

    out = dl->out;

    if (db != dl->prev) {
        out = exp(db * dl->c);
        dl->out = out;
        dl->prev = db;
    }

    return out;
}
#line 130 "dblin.org"
