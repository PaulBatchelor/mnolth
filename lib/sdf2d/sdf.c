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

float sdf_box(struct vec2 p, struct vec2 b)
{
    struct vec2 d;
    float out;

    /* vec2 d = abs(p)-b; */
    d = svec2_abs(p);
    d = svec2_subtract(d, b);

    /* return length(max(d,0.0)) + min(max(d.x,d.y),0.0); */
    out = svec2_length(svec2_max(d, svec2_zero()));
    out += sdf_min(sdf_max(d.x, d.y), 0.0);
    return out;
}

static float ndot(struct vec2 a, struct vec2 b ) {
    return a.x*b.x - a.y*b.y; 
}

float sdf_rhombus(struct vec2 p, struct vec2 b)
{
    float h;
    float d;
    struct vec2 tmp;
    /* p = abs(p) */
    p = svec2_abs(p);
    /* h = clamp(ndot(b-2.0*p,b)/dot(b,b), -1.0, 1.0); */
    tmp = svec2_multiply_f(p, 2.0);
    tmp = svec2_subtract(b, tmp);
    h = ndot(tmp, b) / svec2_dot(b, b);
    h = clampf(h, -1.0, 1.0);
    /* d = length( p-0.5*b*vec2(1.0-h,1.0+h) ); */
    tmp = svec2_multiply_f(b, 0.5);
    tmp = svec2_multiply(tmp, svec2(1.0-h, 1.0+h));
    tmp = svec2_subtract(p, tmp);
    d = svec2_length(tmp);

    /* return d * sign( p.x*b.y + p.y*b.x - b.x*b.y );  */

    return d * sdf_sign(p.x*b.y + p.y*b.x - b.x*b.y);
}

float sdf_equilateral_triangle(struct vec2 p)
{
    const float k = sqrt(3.0);
    p.x = fabs(p.x) - 1.0;
    p.y = p.y + 1.0/k;
    if (p.x + k*p.y > 0.0) {
        p = svec2_multiply_f(svec2(p.x - k*p.y, -k*p.x - p.y), 0.5);
    }

    p.x -= clampf(p.x, -2.0, 0.0);

    return -svec2_length(p) * sdf_sign(p.y);
}

float sdf_pentagon(struct vec2 p, float r)
{
    const struct vec3 k = svec3(0.809016994,0.587785252,0.726542528);
    float tmpf;
    struct vec2 tmp;

    p.x = fabs(p.x);
    /* p -= 2.0*min(dot(vec2(-k.x,k.y),p),0.0)*vec2(-k.x,k.y); */

    tmpf = svec2_dot(svec2(-k.x, k.y), p);
    tmpf = 2.0*sdf_min(tmpf, 0.0);
    tmp = svec2(-k.x, k.y);
    tmp = svec2_multiply_f(tmp, tmpf);
    p = svec2_subtract(p, tmp);

    /* p -= 2.0*min(dot(vec2(+k.x,k.y),p),0.0)*vec2(+k.x,k.y);  */
    tmpf = svec2_dot(svec2(+k.x, k.y), p);
    tmpf = 2.0*sdf_min(tmpf, 0.0);
    tmp = svec2(+k.x, k.y);
    tmp = svec2_multiply_f(tmp, tmpf);
    p = svec2_subtract(p, tmp);

    /* p -= vec2(clamp(p.x,-r*k.z,r*k.z),r); */

    tmp = svec2(clampf(p.x, -r*k.z, r*k.z), r);
    p = svec2_subtract(p, tmp);

    /* return length(p)*sign(p.y); */

    return svec2_length(p) * sdf_sign(p.y);
}

float sdf_hexagon(struct vec2 p, float r)
{
    const struct vec3 k = svec3(-0.866025404,0.5,0.577350269);
    float tmpf;
    struct vec2 tmp;

    p = svec2_abs(p);

    /* p -= 2.0*min(dot(k.xy,p),0.0)*k.xy; */

    tmp = svec2(k.x, k.y);
    tmpf = svec2_dot(tmp, p);
    tmpf = 2.0*sdf_min(tmpf, 0.0);
    tmp = svec2_multiply_f(tmp, tmpf);
    p = svec2_subtract(p, tmp);

    /* p -= vec2(clamp(p.x, -k.z*r, k.z*r), r); */

    tmp = svec2(clampf(p.x, -k.z*r, k.z*r), r);
    p = svec2_subtract(p, tmp);

    return svec2_length(p) * sdf_sign(p.y);
}

float sdf_octogon(struct vec2 p, float r)
{
    const struct vec3 k = svec3(-0.9238795325, 0.3826834323, 0.4142135623 );
    float tmpf;
    struct vec2 tmp;

    p = svec2_abs(p);

    /* p -= 2.0*min(dot(vec2(+k.x,k.y),p),0.0)*vec2(+k.x,k.y);  */
    tmpf = svec2_dot(svec2(k.x, k.y), p);
    tmpf = 2.0*sdf_min(tmpf, 0.0);
    tmp = svec2(+k.x, k.y);
    tmp = svec2_multiply_f(tmp, tmpf);
    p = svec2_subtract(p, tmp);

    /* p -= 2.0*min(dot(vec2(-k.x,k.y),p),0.0)*vec2(-k.x,k.y); */
    tmpf = svec2_dot(svec2(-k.x, k.y), p);
    tmpf = 2.0*sdf_min(tmpf, 0.0);
    tmp = svec2(-k.x, k.y);
    tmp = svec2_multiply_f(tmp, tmpf);
    p = svec2_subtract(p, tmp);


    /* p -= vec2(clamp(p.x,-r*k.z,r*k.z),r); */

    tmp = svec2(clampf(p.x, -r*k.z, r*k.z), r);
    p = svec2_subtract(p, tmp);

    /* return length(p)*sign(p.y); */

    return svec2_length(p) * sdf_sign(p.y);
}

float sdf_hexagram(struct vec2 p, float r)
{
    const struct vec4 k = svec4(-0.5,0.8660254038,0.5773502692,1.7320508076);
    struct vec2 tmp;
    float tmpf;

    p = svec2_abs(p);

    /* p -= 2.0*min(dot(k.xy,p),0.0)*k.xy; */

    tmp = svec2(k.x, k.y);
    tmpf = svec2_dot(tmp, p);
    tmpf = 2.0*sdf_min(tmpf, 0.0);
    tmp = svec2_multiply_f(tmp, tmpf);
    p = svec2_subtract(p, tmp);

    /* note the change from xy -> yx */
    /* p -= 2.0*min(dot(k.xy,p),0.0)*k.yx; */

    tmp = svec2(k.y, k.x);
    tmpf = svec2_dot(tmp, p);
    tmpf = 2.0*sdf_min(tmpf, 0.0);
    tmp = svec2_multiply_f(tmp, tmpf);
    p = svec2_subtract(p, tmp);

    /* p -= vec2(clamp(p.x,r*k.z,r*k.w),r); */
    tmp = svec2(clampf(p.x, r*k.z,r*k.w),r);
    p = svec2_subtract(p, tmp);

    return svec2_length(p) - sdf_sign(p.y);
}

float sdf_star5(struct vec2 p, float r, float rf)
{
    const struct vec2 k1 = svec2(0.809016994375, -0.587785252292);
    const struct vec2 k2 = svec2(-k1.x,k1.y);
    float tmpf;
    struct vec2 tmp;
    struct vec2 ba;
    float h;

    p.x = fabs(p.x);

    /* p -= 2.0*max(dot(k1,p),0.0)*k1; */

    tmpf = svec2_dot(k1, p);
    tmpf = 2.0 * sdf_max(tmpf, 0.0);
    tmp = svec2_multiply_f(k1, tmpf);
    p = svec2_subtract(p, tmp);

    /* p -= 2.0*max(dot(k2,p),0.0)*k2; */
    tmpf = svec2_dot(k2, p);
    tmpf = 2.0 * sdf_max(tmpf, 0.0);
    tmp = svec2_multiply_f(k2, tmpf);
    p = svec2_subtract(p, tmp);

    p.x = fabs(p.x);
    p.y -= r;

    /* vec2 ba = rf*vec2(-k1.y,k1.x) - vec2(0,1); */
    ba = svec2_multiply_f(svec2(-k1.y, k1.x), rf);
    ba = svec2_subtract(ba, svec2(0, 1));

    /* float h = clamp( dot(p,ba)/dot(ba,ba), 0.0, r ); */
    tmpf = svec2_dot(p, ba);
    tmpf = tmpf / svec2_dot(ba, ba);
    h = clampf(tmpf, 0.0, r);

    /* return length(p-ba*h) * sign(p.y*ba.x-p.x*ba.y); */
    tmp = svec2_multiply_f(ba, h);
    tmp = svec2_subtract(p, tmp);
    return svec2_length(tmp) * sdf_sign(p.y*ba.x-p.x*ba.y);
}

float sdf_rounded_x(struct vec2 p, float w, float r)
{
    p = svec2_abs(p);

    p = svec2_subtract_f(p, sdf_min(p.x + p.y,w) * 0.5);

    return svec2_length(p) - r;
}

float sdf_vesica(struct vec2 p, float r, float d)
{
    float b;
    float out;

    out = 0;

    p = svec2_abs(p);

    b = sqrt(r*r - d*d);

    if (((p.y - b) * d) > p.x*b) {
        p = svec2_subtract(p, svec2(0.0, b));
        out = svec2_length(p);
    } else {
        p = svec2_subtract(p, svec2(-d, 0.0));
        out = svec2_length(p) - r;
    }
    return out;
}

float sdf_egg(struct vec2 p, float ra, float rb)
{
    const float k = sqrt(3.0);
    float r;
    float out;

    out = 0;

    p.x = fabs(p.x);

    r = ra - rb;
/*
    return ((p.y<0.0)       ? length(vec2(p.x,  p.y    )) - r :
            (k*(p.x+r)<p.y) ? length(vec2(p.x,  p.y-k*r)) :
                              length(vec2(p.x+r,p.y    )) - 2.0*r) - rb;
*/
    if (p.y < 0.0) {
        out = svec2_length(svec2(p.x, p.y)) - r;
    } else {
        if (k * (p.x + r) < p.y) {
            out = svec2_length(svec2(p.x, p.y-k*r));
        } else {
            out = svec2_length(svec2(p.x + r, p.y)) - 2.0*r;
        }
    }

    return out - rb;
}

float sdf_ellipse(struct vec2 p, struct vec2 ab)
{
    float l;
    float m, m2;
    float n, n2;
    float c, c3;
    float q;
    float d;
    float g;
    float co;
    struct vec2 r;

    p = svec2_abs(p);

    if (p.x > p.y) {
        p = svec2(p.y, p.x);
        ab = svec2(ab.y, ab.x);
    }

    l = ab.y*ab.y - ab.x*ab.x;
    m = ab.x*p.x / l;
    m2 = m*m;
    n = ab.y*p.y / l;
    n2 = n*n;
    c = (m2 + n2 - 1.0) / 3.0;
    c3 = c*c*c;
    q = c3 + m2*n2*2.0;
    d = c3 + m2*n2;
    g = m + m*n2;

    if (d < 0.0) {
        float h;
        float s;
        float t;
        float rx;
        float ry;

        h = acos(q/c3)/3.0;
        s = cos(h);
        t = sin(h)*sqrt(3.0);
        rx = sqrt(-c*(s + t + 2.0) + m2);
        ry = sqrt(-c*(s - t + 2.0) + m2);
        co = (ry + sdf_sign(l)*rx + fabs(g)/(rx*ry) - m)*0.5;
    } else {
        float h;
        float s;
        float u;
        float rx;
        float ry;
        float rm;

        h = 2.0*m*n*sqrt(d);
        s = sdf_sign(q+h)*pow(fabs(q+h), 1.0/3.0);
        u = sdf_sign(q-h)*pow(fabs(q-h), 1.0/3.0);
        rx = -s - u - c*4.0 + 2.0*m2;
        ry = (s - u)*sqrt(3.0);
        rm = sqrt(rx*rx + ry*ry);
        co = (ry/sqrt(rm - rx) + 2.0*g/rm - m)*0.5;
    }

    r = svec2_multiply(ab, svec2(co, sqrt(1.0-co*co)));
    return svec2_length(svec2_subtract(r, p)) * sdf_sign(p.y - r.y);
}

float sdf_moon(struct vec2 p, float d, float ra, float rb)
{
    float a;
    float b;
    float out;

    p.y = fabs(p.y);

    a = (ra*ra - rb*rb + d*d)/(2.0 * d);
    b = sqrt(sdf_max(ra*ra - a*a, 0.0));

    out = 0;

    if (d*(p.x*b - p.y*a) > d*d*sdf_max(b-p.y, 0.0)) {
        out = svec2_length(svec2_subtract(p, svec2(a, b)));
    } else {
        out = sdf_max(svec2_length(p) - ra,
                      -(svec2_length(svec2_subtract(p, svec2(d, 0))) - rb));
    }

    return out;
}

float sdf_polygon(struct vec2 *v, int N, struct vec2 p)
{
    float d;
    float s;
    int i;
    int j;

    /* d = dot(p - v[0], p - v[0]) */
    d = dot2(svec2_subtract(p, v[0]));
    s = 1.0;

    for (i=0, j=N-1; i < N; j=i, i++) {
        struct vec2 e;
        struct vec2 w;
        struct vec2 b;
        struct vec3 c;
        int all, none;
        float tmpf;

        /* vec2 e = v[j] - v[i] */
        e = svec2_subtract(v[j], v[i]);

        /* vec2 w = p - v[i] */
        w = svec2_subtract(p, v[i]);

        /* vec2 b = w - e*clamp( dot(w, e) / dot(e, e), 0.0, 1.0) */
        tmpf = svec2_dot(e, e);
        if (tmpf != 0) {
            tmpf = svec2_dot(w, e) / tmpf;
        }
        if (tmpf < 0.0) tmpf = 0.0;
        else if (tmpf > 1.0) tmpf = 1.0;
        b = svec2_multiply_f(e, tmpf);
        b = svec2_subtract(w, b);

        /* d = min(d, dot(b,b)) */
        tmpf = dot2(b);
        if (tmpf < d) d = tmpf;

        /* bvec3 c = bvec3(p.y>=v[i].y,p.y<v[j].y,e.x*w.y>e.y*w.x); */
        c = svec3(p.y>=v[i].y,
                  p.y<v[j].y,
                  e.x*w.y>e.y*w.x);

        /* if ( all(c) || all(not(c))) s *= -1.0; */

        all = (c.x > 0) && (c.y > 0) && (c.z > 0);
        none = (c.x < 1) && (c.y < 1) && (c.z < 1);
        if (all || none) s = -s;
    }

    return s * sqrt(d);
}

float sdf_onion(float d, float r)
{
    return fabs(d) - r;
}

float sdf_union(float d1, float d2)
{
    return sdf_min(d1, d2);
}

float sdf_union_smooth(float d1, float d2, float k)
{
    float h;
    float mix;

    if (k == 0) return 0;

    h = clampf(0.5 + 0.5*(d2-d1)/k, 0.0, 1.0);

    mix = d2*(1.0-h) + d1*h;

    mix -= k*h*(1.0 - h);
    return mix;
}

float sdf_subtract(float d1, float d2)
{
    return sdf_max(-d1, d2);
}
