#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pmplay.h"
#include "posix.h"

/* defaults when not overriden by argument switches */
#define DEFAULT_MIX_FREQ 44100
#define DEFAULT_MIX_BUFSIZE 1024
#define DEFAULT_MIX_AMP 10
#define DEFAULT_MASTERVOL 256
#define DEFAULT_INTRP_FLAG true
#define DEFAULT_VRAMP_FLAG true

/* default settings */
static bool interpolation = DEFAULT_INTRP_FLAG;
static bool volumeRamping = DEFAULT_VRAMP_FLAG;
static int32_t mixingAmp = DEFAULT_MIX_AMP;
static int32_t masterVolume = DEFAULT_MASTERVOL;
static int32_t mixingFrequency = DEFAULT_MIX_FREQ;
static int32_t mixingBufferSize = DEFAULT_MIX_BUFSIZE;

int ft2play_main(int argc, char *argv[])
{
    const char *xmFileName;
    const char *wavFileName;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s in.xm out.wav\n", argv[0]);
        return 1;
    }

    xmFileName = argv[1];
    wavFileName = argv[2];
    initMusic(mixingFrequency, mixingBufferSize, interpolation, volumeRamping);

	if (!loadMusic(xmFileName)) {
		fprintf(stderr, "Error: Couldn't load song!\n");
		return 1;
	}

	/* you only need to make a call to these if
     * amp != 4 and/or mastervol != 256, which are the
     * FT2 defaults */
	if (mixingAmp != 4) setAmp(mixingAmp);
	if (masterVolume != 256) setMasterVol(masterVolume);

    WAVDump_Record(wavFileName);

	freeMusic();
    return 0;
}
