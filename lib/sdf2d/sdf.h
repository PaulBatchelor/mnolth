#ifndef SDF2D_H
#define SDF2D_H
float sdf_sign(float x);
float sdf_min(float x, float y);
float sdf_max(float x, float y);
float sdf_circle(struct vec2 p, float r);
float sdf_heart(struct vec2 p);
struct vec2 sdf_heart_center(struct vec2 pos, struct vec2 res);
float sdf_smoothstep(float e0, float e1, float x);
struct vec2 sdf_normalize(struct vec2 pos, struct vec2 res);
float sdf_rounded_box(struct vec2 pos, struct vec2 b, struct vec4 r);
float sdf_box(struct vec2 p, struct vec2 b);
float sdf_rhombus(struct vec2 p, struct vec2 b);
float sdf_equilateral_triangle(struct vec2 p);
float sdf_pentagon(struct vec2 p, float r);
float sdf_hexagon(struct vec2 p, float r);
float sdf_octogon(struct vec2 p, float r);
float sdf_hexagram(struct vec2 p, float r);
float sdf_star5(struct vec2 p, float r, float rf);
float sdf_rounded_x(struct vec2 p, float w, float r);
float sdf_vesica(struct vec2 p, float r, float d);
float sdf_egg(struct vec2 p, float ra, float rb);
float sdf_ellipse(struct vec2 p, struct vec2 ab);
float sdf_moon(struct vec2 p, float d, float ra, float rb);
#endif
