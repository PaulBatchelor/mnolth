#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "ftbl.h"
#include "paulstretch.h"
#include "../../lib/sndkit/nodes/dr_wav.h"

static void process(sp_data *sp, void *ud)
{
    SPFLOAT out;
    sp_paulstretch *ps;
    ps = ud;
    sp_paulstretch_compute(sp, ps, NULL, &out);
    sp_out(sp, 0, out);
}

/* a little kluge to get file info */
static int get_info(const char *fname,
                    int *sr,
                    unsigned long *len)
{
    sk_drwav wav;
    int rc;

    rc = sk_drwav_init_file(&wav, fname, NULL);

    *sr = wav.sampleRate;
    *len = wav.totalPCMFrameCount;

    if (!rc) {
        return 0;
    }

    sk_drwav_uninit(&wav);
    return 1;
}

int main(int argc, char *argv[])
{
    sp_data *sp;
    sp_ftbl *ft;
    sp_paulstretch *ps;

    SPFLOAT stretch;
    SPFLOAT window;
    const char *fin;
    const char *fout;
    int rc;
    FILE *fp;

    if(argc < 5) {
        fprintf(stderr,
            "Usage: %s window_size stretch in.wav out.wav\n",
            argv[0]
        );
        return 1;
    }

    sp_create(&sp);

    printf("samplerate = %d\n", atoi(argv[1]));

    sp_paulstretch_create(&ps);

    fin = argv[3];
    fout = argv[4];
    rc = get_info(fin, &sp->sr, &sp->len);

    if (!rc) {
        printf("There was a problem with the file '%s'\n",
               fin);
        goto cleanup;
    }

    fp = fopen(fout, "w");

    if (fp == NULL) {
       fprintf(stderr, "Could not open %s\n", fout);
       return 1;
    }

    printf("window = %g\n", atof(argv[1]));
    window = atof(argv[1]);
    printf("stretch = %g\n", atof(argv[2]));
    stretch = atof(argv[2]);
    sp->len *= stretch;
    printf("total dur = %gs\n", (SPFLOAT)sp->len / sp->sr);
    printf("input = %s\n", fin);
    sp_ftbl_loadfile(sp, &ft, fin);
    printf("output = %s\n", fout);

    strncpy(sp->filename, fout, 60);

    sp_paulstretch_init(sp, ps, ft, window, stretch);

    ps->wrap = 0;

    sp_process_raw(sp, ps, process, fp);

    cleanup:

    sp_ftbl_destroy(&ft);
    sp_paulstretch_destroy(&ps);
    sp_destroy(&sp);
    return 0;
}
