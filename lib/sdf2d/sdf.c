#include <math.h>
#include "mathc/mathc.h"

float sdf_sign(float x)
{
    if (x == 0) return 0;
    return x < 0 ? -1 : 1;
}

float sdf_min(float x, float y)
{
    return x < y ? x : y;
}

float sdf_max(float x, float y)
{
    return x > y ? x : y;
}

float sdf_circle(struct vec2 p, float r)
{
    return svec2_length(p) - r;
}

static float dot2(struct vec2 p)
{
    return svec2_dot(p, p);
}

float sdf_heart(struct vec2 p)
{
    p.x = fabs(p.x);
    /* p.y = 1 - p.y; */
    /* p.y = p.y - 0.5; */

    if (p.y + p.x > 1.0) {
        return sqrt(dot2(svec2_subtract(p, svec2(0.25, 0.75)))) -
            sqrt(2.0)/4.0;
    }
    return sqrt(sdf_min(dot2(svec2_subtract(p, svec2(0.0, 1.00))),
                    dot2(svec2_subtract_f(p, 0.5*sdf_max(p.x+p.y, 0.0))))) *
                    sdf_sign(p.x - p.y);
}

struct vec2 sdf_heart_center(struct vec2 pos, struct vec2 res)
{
    struct vec2 p;
    p.x = (2.0 * pos.x - res.x) / res.y;
    p.y = (2.0 * (res.y - pos.y) - res.y) / res.y;
    p.y += 0.5;
    return p;
}

float sdf_smoothstep(float e0, float e1, float x)
{
    float t;
    t = clampf((x - e0) / (e1 - e0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

struct vec2 sdf_normalize(struct vec2 pos, struct vec2 res)
{
    struct vec2 p;
    p = svec2_multiply_f(pos, 2.0);
    p = svec2_subtract(p, res);
    p = svec2_divide_f(p, res.y);
    return p;
}

float sdf_rounded_box(struct vec2 pos, struct vec2 b, struct vec4 r)
{
    struct vec2 q;
    float out;

    /* r.xy = (p.x>0.0)?r.xy : r.zw; */

    if (pos.x <= 0.0) {
        r.x = r.z;
        r.y = r.w;
    }

    /* r.x  = (p.y>0.0)?r.x  : r.y; */

    if (pos.y <= 0.0) {
        r.x = r.y;
    }
    /* vec2 q = abs(p)-b+r.x; */

    /* abs(p) */
    q = pos;
    q.x = fabs(q.x);
    q.y = fabs(q.y);

    /* q - b */
    q = svec2_subtract(q, b);

    /* q + r.x */
    q = svec2_add_f(q, r.x);

    /* return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x; */

    /* min(max(q.x, q.y), 0.0) */

    out = sdf_min(sdf_max(q.x, q.y), 0.0);

    /* + length(max(q, 0.0)) */

    out += svec2_length(svec2_max(q, svec2_zero()));

    /* - r.x */
    out -= r.x;

    return out;
}
