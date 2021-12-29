#line 12 "bezier.org"
#include <math.h>
#include "bezier.h"
#line 96 "bezier.org"
static SKFLT find_t(SKFLT x0, SKFLT x1, SKFLT x2, SKFLT x);
#line 119 "bezier.org"
static SKFLT quadratic_equation(SKFLT a, SKFLT b, SKFLT c);
#line 12 "bezier.org"
#line 101 "bezier.org"
static SKFLT find_t(SKFLT x0, SKFLT x1, SKFLT x2, SKFLT x)
{
    SKFLT a, b, c;

    a = (x0 - 2.0 * x1 + x2);
    b = 2.0 * (-x0 + x1);
    c = x0 - x;

    if (a) {
        return quadratic_equation(a, b, c);
    } else {
        return (x - x0) / b;
    }
}
#line 124 "bezier.org"
static SKFLT quadratic_equation(SKFLT a, SKFLT b, SKFLT c)
{
    SKFLT det; /* determinant */

    det = b*b - 4*a*c;

    if (det >= 0) {
        return ((-b + sqrt(det))/(2.0*a));
    } else {
        return 0;
    }
}
#line 154 "bezier.org"
SKFLT sk_bezier_tick(SKFLT xpos, SKFLT cx, SKFLT cy)
{
    SKFLT x[3];
    SKFLT y[3];
    SKFLT t;
    SKFLT val;

    x[0] = 0;
    x[1] = cx;
    x[2] = 1;

    y[0] = 0;
    y[1] = cy;
    y[2] = 1;

    t = find_t(x[0], x[1], x[2], xpos);

    val = (1.0-t)*(1.0-t)*y[0] + 2.0*(1 - t)*t*y[1] + t*t*y[2];
    return val;
}
#line 12 "bezier.org"
