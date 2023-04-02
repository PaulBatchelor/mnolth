#ifndef XM_STATE_INTERNAL_H
#define XM_STATE_INTERNAL_H
struct xm_state {
	 uint16_t tempo;
	 uint16_t bpm;
	 float global_volume;
	 float amplification;

#if XM_RAMPING
	 float volume_ramp;
	 float panning_ramp; /* Same for panning. */
#endif

	 uint8_t current_table_index;
	 uint8_t current_row;
	 uint16_t current_tick;
	 float remaining_samples_in_tick;
	 uint64_t generated_samples;

	 bool position_jump;
	 bool pattern_break;
	 uint8_t jump_dest;
	 uint8_t jump_row;

	 uint16_t extra_ticks;

	 uint8_t loop_count;
	 uint8_t max_loop_count;
};
#endif
