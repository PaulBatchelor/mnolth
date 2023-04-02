#ifndef XM_STATE_H
#define XM_STATE_H
typedef struct xm_state xm_state;
void xm_state_save(xm_context_t *ctx, xm_state *s);
void xm_state_restore(xm_context_t *ctx, xm_state *s);
size_t xm_state_size(void);
int xm_state_new(xm_state **s);
int xm_state_free(xm_state **s);
#endif
