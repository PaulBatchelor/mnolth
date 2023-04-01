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
#endif
