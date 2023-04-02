#include "xm_internal.h"
#include "state.h"
#include "state_internal.h"

void xm_state_save(xm_context_t *ctx, xm_state *s)
{
    s->tempo = ctx->tempo;
    s->bpm = ctx->bpm;
    s->global_volume = ctx->global_volume;
    s->amplification = ctx->amplification;

#if XM_RAMPING
    s->volume_ramp = ctx->volume_ramp;
    s->panning_ramp = ctx->panning_ramp; /* Same for panning. */
#endif

    s->current_table_index = ctx->current_table_index;
    s->current_row = ctx->current_row;
    s->current_tick = ctx->current_tick;
    s->remaining_samples_in_tick = ctx->remaining_samples_in_tick;
    s->generated_samples = ctx->generated_samples;

    s->position_jump = ctx->position_jump;
    s->pattern_break = ctx->pattern_break;
    s->jump_dest = ctx->jump_dest;
    s->jump_row = ctx->jump_row;

    s->extra_ticks = ctx->extra_ticks;

    s->loop_count = ctx->loop_count;
    s->max_loop_count = ctx->max_loop_count;
}

void xm_state_restore(xm_context_t *ctx, xm_state *s)
{
    ctx->tempo = s->tempo;
    ctx->bpm = s->bpm;
    ctx->global_volume = s->global_volume;
    ctx->amplification = s->amplification;

#if XM_RAMPING
    ctx->volume_ramp = s->volume_ramp;
    ctx->panning_ramp = s->panning_ramp; /* Same for panning. */
#endif

    ctx->current_table_index = s->current_table_index;
    ctx->current_row = s->current_row;
    ctx->current_tick = s->current_tick;
    ctx->remaining_samples_in_tick = s->remaining_samples_in_tick;
    ctx->generated_samples = s->generated_samples;

    ctx->position_jump = s->position_jump;
    ctx->pattern_break = s->pattern_break;
    ctx->jump_dest = s->jump_dest;
    ctx->jump_row = s->jump_row;

    ctx->extra_ticks = s->extra_ticks;

    ctx->loop_count = s->loop_count;
    ctx->max_loop_count = s->max_loop_count;
}

size_t xm_state_size(void)
{
    return sizeof(xm_state);
}

int xm_state_new(xm_state **s)
{
    *s = calloc(1, xm_state_size());
    return *s == NULL;
}

int xm_state_free(xm_state **s)
{
    free(*s);
    return 1;
}
