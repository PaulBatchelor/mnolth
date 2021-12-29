#line 16 "softclip.org"
#include "softclip.h"
#line 44 "softclip.org"
static SKFLT rational_tanh(SKFLT x)
{
    if( x < -3 )
        return -1;
    else if( x > 3 )
        return 1;
    else
        return x * ( 27 + x * x ) / ( 27 + 9 * x * x );
}

SKFLT sk_softclip_tick(SKFLT in, SKFLT drive)
{
    return rational_tanh(in * drive);
}
#line 16 "softclip.org"
