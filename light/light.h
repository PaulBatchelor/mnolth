#ifndef MNOLTH_LIGHT_H
#define MNOLTH_LIGHT_H
typedef struct {
    int fd;
} mno_light;

int mno_light_init(mno_light *ml, const char *portname);
void mno_light_close(mno_light *ml);
void mno_light_set(mno_light *ml, float amt);
#endif
