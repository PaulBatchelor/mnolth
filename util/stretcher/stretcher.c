#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "ftbl.h"
#include "paulstretch.h"
#include "../../lib/sndkit/nodes/dr_wav.h"
#include "../../lib/sndkit/graforge/graforge.h"
#include "../../lib/sndkit/core.h"

int sp_ftbl_loadfile_v2(sp_data *sp,
        sp_ftbl **ft,
        const char *filename)
{
    sk_drwav wav;
    *ft = malloc(sizeof(sp_ftbl));
    sp_ftbl *ftp = *ft;

    if (!sk_drwav_init_file(&wav, filename, NULL)) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return 1;
    }

    size_t size = wav.totalPCMFrameCount;

    ftp->tbl = malloc(sizeof(SPFLOAT) * size);

    sp_ftbl_init(sp, ftp, size);

    sk_drwav_read_pcm_frames_f32(&wav, size, ftp->tbl);
    sk_drwav_uninit(&wav);

    return SP_OK;
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

static void write_wav(sp_data *sp,
        sk_drwav *outfile,
        sp_paulstretch *ps)
{
    float *buf;
    uint32_t nsamps;
    buf = calloc(1, sizeof(float) * 2048);

    nsamps = sp->len;

    while (nsamps > 0) {
        int bufsize;
        int i;
        bufsize = 2048;

        if (nsamps < bufsize) {
            bufsize = nsamps;
        }

        for (i = 0; i < bufsize; i++) {
            SPFLOAT out;
            sp_paulstretch_compute(sp, ps, NULL, &out);
            buf[i] = out;
        }

        sk_drwav_write_pcm_frames(outfile, bufsize, buf);
        nsamps -= bufsize;
    }

    free(buf);
}

int stretcher_main(int argc, char *argv[])
{
    sp_data *sp;
    sp_ftbl *ft;
    sp_paulstretch *ps;

    SPFLOAT stretch;
    SPFLOAT window;
    const char *fin;
    const char *fout;
    int rc;
    sk_drwav outfile;
    sk_drwav_data_format format;

    if(argc < 5) {
        fprintf(stderr,
            "Usage: %s window_size stretch in.wav out.wav\n",
            argv[0]
        );
        return 1;
    }

    sp_create(&sp);

    sp_paulstretch_create(&ps);

    fin = argv[3];
    fout = argv[4];
    rc = get_info(fin, &sp->sr, &sp->len);

    if (!rc) {
        printf("There was a problem with the file '%s'\n",
               fin);
        goto cleanup;
    }

    format.container = sk_drwav_container_riff;
    format.format = DR_WAVE_FORMAT_IEEE_FLOAT;
    format.channels = 1;
    format.sampleRate = sp->sr;
    format.bitsPerSample = 32;
    sk_drwav_init_file_write(&outfile, fout, &format, NULL);

    printf("window = %g\n", atof(argv[1]));
    window = atof(argv[1]);
    printf("stretch = %g\n", atof(argv[2]));
    stretch = atof(argv[2]);
    sp->len *= stretch;
    printf("total dur = %gs\n", (SPFLOAT)sp->len / sp->sr);
    printf("input = %s\n", fin);
    sp_ftbl_loadfile_v2(sp, &ft, fin);
    printf("output = %s\n", fout);

    strncpy(sp->filename, fout, 60);

    sp_paulstretch_init(sp, ps, ft, window, stretch);

    ps->wrap = 0;

    write_wav(sp, &outfile, ps);

    cleanup:

    sp_ftbl_destroy(&ft);
    sp_paulstretch_destroy(&ps);
    sp_destroy(&sp);
    sk_drwav_uninit(&outfile);
    return 0;
}

#ifdef STRETCHER_MAIN
int main(int argc, char *argv[])
{
    return stretcher_main(argc, argv);
}
#endif
