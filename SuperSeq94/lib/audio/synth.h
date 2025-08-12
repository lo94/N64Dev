#ifndef H_SYNTH
#define H_SYNTH

#include <stdbool.h>
#include <stdint.h>
#include <libdragon.h>

typedef struct fx_s {

} fx_t;

/*
Maybe allow for complex envelope design?
*/
typedef struct env_s {
	// 0 - AD, 1 - ADSR
	int type;
	double a, d, s, r;
	bool sustain;
} env_t;

typedef struct lfo_s {
	// 0 - tri, 1 - sin, 2 - sqr, 3 - saw, 4 - rvs saw, 5 - rand
	int type;
	double freq;
	bool enabled;
} lfo_t;

typedef struct sample_s {
	bool loop;
	double startPoint;
	double endPoint;
	// Crossfade will require 2 channels, so each voice uses 2 channels
	double crossFade;
	// 1 - base, 2 - half sample rate, 4 - 1/4 sample rate, 8 - 1/8 sample rate
	int sampleRate;
} sample_t;

typedef struct voice_s {
	sample_t sample;
	env_t env;
	// output channel
	int output;
} voice_t;

#endif