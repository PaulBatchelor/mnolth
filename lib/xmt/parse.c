#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cmp/cmp.h"
#include "moncmp.h"
#include "obj.h"

typedef struct {
    char id_text[17];
    char module_name[20];
    char magicbyte; /* $1a */
    char tracker_name[20];
    uint16_t version;
    uint32_t header_size;
    uint16_t song_length;
    uint16_t restart_position;
    uint16_t nchannels;
    uint16_t npatterns;
    uint16_t ninstruments;
    uint16_t flags;
    uint16_t tempo;
    uint16_t bpm;
    uint8_t ptable[256];
} xm_header;

typedef struct {
    uint32_t header_length;
    uint8_t packing_type;
    uint16_t nrows;
    uint16_t packed_size;
    uint8_t *data;
} xm_pattern;

typedef struct {
    uint32_t length;
    uint32_t loop_start;
    uint32_t loop_length;
    uint8_t volume;
    int8_t finetune;
    uint8_t type;
    uint8_t panning;
    int8_t rnotenum;
    uint8_t compression;
    char name[22];
    uint8_t *data;
} xm_sample;

typedef struct {
    uint32_t header_size;
    char name[22];
    uint8_t type;
    uint16_t nsamples;
    uint32_t sample_header_size;
    uint8_t keymap[96];
    uint16_t vol[24];
    uint16_t pan[24];
    uint8_t nvol;
    uint8_t npan;
    uint8_t volsus;
    uint8_t volstart;
    uint8_t volend;
    uint8_t pansus;
    uint8_t panstart;
    uint8_t panend;
    uint8_t vtype;
    uint8_t ptype;
    uint8_t vibtype;
    uint8_t vibsweep;
    uint8_t vibdepth;
    uint8_t vibrate;
    uint16_t vol_fadeout;
    uint16_t reserved[11];
    xm_sample *samples;
} xm_instrument;

typedef struct {
    xm_header header;
    xm_pattern *patterns;
    xm_instrument *instruments;
} xm_module;

typedef struct {
    FILE *fp;
} xm_iodat;

static size_t writer(xm_iodat *w,
                     const void *data,
                     size_t sz)
{
    return fwrite(data, 1, sz, w->fp);
}

static size_t reader(xm_iodat *r,
                     void *data,
                     size_t sz)
{
    return fread(data, 1, sz, r->fp);
}

void xm_header_write(xm_header *h, xm_iodat *w)
{
    writer(w, h, sizeof(xm_header));
}

void xm_header_read(xm_header *h, xm_iodat *r)
{
    reader(r, h, sizeof(xm_header));
}

void xm_pattern_write(xm_pattern *pat, xm_iodat *w)
{
    writer(w, &pat->header_length, 4);
    writer(w, &pat->packing_type, 1);
    writer(w, &pat->nrows, 2);
    writer(w, &pat->packed_size, 2);
    writer(w, pat->data, pat->packed_size);
}

void xm_pattern_read(xm_pattern *pat, xm_iodat *r)
{
    reader(r, &pat->header_length, 4);
    reader(r, &pat->packing_type, 1);
    reader(r, &pat->nrows, 2);
    reader(r, &pat->packed_size, 2);
    pat->data = malloc(pat->packed_size);
    reader(r, pat->data, pat->packed_size);
}

void xm_sample_read(xm_sample *smp, xm_iodat *r)
{
    reader(r, &smp->length, 4);
    reader(r, &smp->loop_start, 4);
    reader(r, &smp->loop_length, 4);
    reader(r, &smp->volume, 1);
    reader(r, &smp->finetune, 1);
    reader(r, &smp->type, 1);
    reader(r, &smp->panning, 1);
    reader(r, &smp->rnotenum, 1);
    reader(r, &smp->compression, 1);
    reader(r, &smp->name, 22);
    smp->data = NULL;
}

void xm_sample_write(xm_sample *smp, xm_iodat *w)
{
    writer(w, &smp->length, 4);
    writer(w, &smp->loop_start, 4);
    writer(w, &smp->loop_length, 4);
    writer(w, &smp->volume, 1);
    writer(w, &smp->finetune, 1);
    writer(w, &smp->type, 1);
    writer(w, &smp->panning, 1);
    writer(w, &smp->rnotenum, 1);
    writer(w, &smp->compression, 1);
    writer(w, &smp->name, 22);
}

void xm_instrument_read(xm_instrument *ins, xm_iodat *r)
{
    reader(r, &ins->header_size, 4);
    reader(r, &ins->name, 22);
    reader(r, &ins->type, 1);
    reader(r, &ins->nsamples, 2);
    ins->samples = NULL;
    if (ins->nsamples > 0) {
        uint16_t s;
        reader(r, &ins->sample_header_size, 4);
        reader(r, &ins->keymap, 96);
        reader(r, &ins->vol, 48);
        reader(r, &ins->pan, 48);
        reader(r, &ins->nvol, 1);
        reader(r, &ins->npan, 1);

        reader(r, &ins->volsus, 1);
        reader(r, &ins->volstart, 1);
        reader(r, &ins->volend, 1);

        reader(r, &ins->pansus, 1);
        reader(r, &ins->panstart, 1);
        reader(r, &ins->panend, 1);

        reader(r, &ins->vtype, 1);
        reader(r, &ins->ptype, 1);

        reader(r, &ins->vibtype, 1);
        reader(r, &ins->vibsweep, 1);
        reader(r, &ins->vibdepth, 1);
        reader(r, &ins->vibrate, 1);

        reader(r, &ins->vol_fadeout, 2);
        reader(r, &ins->reserved, 22);

        ins->samples = malloc(sizeof(xm_sample) * ins->nsamples);

        for (s = 0; s < ins->nsamples; s++) {
            xm_sample_read(&ins->samples[s], r);
        }
        for (s = 0; s < ins->nsamples; s++) {
            xm_sample *smp;
            smp = &ins->samples[s];
            smp->data = malloc(smp->length);
            reader(r, smp->data, smp->length);
        }
    }
}

void xm_instrument_write(xm_instrument *ins, xm_iodat *w)
{
    writer(w, &ins->header_size, 4);
    writer(w, &ins->name, 22);
    writer(w, &ins->type, 1);
    writer(w, &ins->nsamples, 2);

    if (ins->nsamples > 0) {
        uint16_t s;

        writer(w, &ins->sample_header_size, 4);
        writer(w, &ins->keymap, 96);
        writer(w, &ins->vol, 48);
        writer(w, &ins->pan, 48);
        writer(w, &ins->nvol, 1);
        writer(w, &ins->npan, 1);

        writer(w, &ins->volsus, 1);
        writer(w, &ins->volstart, 1);
        writer(w, &ins->volend, 1);

        writer(w, &ins->pansus, 1);
        writer(w, &ins->panstart, 1);
        writer(w, &ins->panend, 1);

        writer(w, &ins->vtype, 1);
        writer(w, &ins->ptype, 1);

        writer(w, &ins->vibtype, 1);
        writer(w, &ins->vibsweep, 1);
        writer(w, &ins->vibdepth, 1);
        writer(w, &ins->vibrate, 1);

        writer(w, &ins->vol_fadeout, 2);
        writer(w, &ins->reserved, 22);

        for (s = 0; s < ins->nsamples; s++) {
            xm_sample_write(&ins->samples[s], w);
        }

        for (s = 0; s < ins->nsamples; s++) {
            xm_sample *smp;
            smp = &ins->samples[s];
            writer(w, smp->data, smp->length);
        }
    }
}

void xm_instrument_free(xm_instrument *ins)
{
    if (ins->samples != NULL) {
        int s;

        for (s = 0; s < ins->nsamples; s++) {
            xm_sample *smp;
            smp = &ins->samples[s];
            if (smp->data != NULL) free(smp->data);
        }

        free(ins->samples);
    }
}

xm_module * xm_module_read(xm_iodat *r)
{
    xm_header *header;
    int i;
    xm_pattern *pats;
    xm_instrument *ins;
    xm_module *xm;

    xm = malloc(sizeof(xm_module));
    header = &xm->header;
    xm_header_read(header, r);

    xm->patterns = malloc(sizeof(xm_pattern) * header->npatterns);
    xm->instruments = malloc(sizeof(xm_instrument) * header->ninstruments);
    pats = xm->patterns;
    ins = xm->instruments;

    for (i = 0; i < header->npatterns; i++) {
        xm_pattern_read(&pats[i], r);
    }

    for (i = 0; i < header->ninstruments; i++) {
        xm_instrument_read(&ins[i], r);
    }

    return xm;
}

void xm_module_write(xm_module *xm, xm_iodat *w)
{
    xm_pattern *pats;
    xm_instrument *ins;
    xm_header *header;
    int i;

    header = &xm->header;
    pats = xm->patterns;
    ins = xm->instruments;

    xm_header_write(header, w);

    pats = xm->patterns;
    ins = xm->instruments;
    header = &xm->header;

    for (i = 0; i < header->npatterns; i++) {
        xm_pattern_write(&pats[i], w);
    }
    for (i = 0; i < header->ninstruments; i++) {
        xm_instrument_write(&ins[i], w);
    }
}

void xm_module_free(xm_module **xmp)
{
    int i;
    xm_header *header;
    xm_instrument *ins;
    xm_pattern *pats;
    xm_module *xm;

    xm = *xmp;
    ins = xm->instruments;
    pats = xm->patterns;
    header = &xm->header;

    if (pats != NULL) {
        for (i = 0; i < header->npatterns; i++) {
            if (pats[i].data != NULL) free(pats[i].data);
        }
        free(pats);
    }

    if (ins != NULL) {
        for (i = 0; i < header->ninstruments; i++) {
            xm_instrument_free(&ins[i]);
        }
        free(ins);
    }

    free(xm);
}

void dump(xm_module *xm, cmp_ctx_t *cmp)
{
    xm_header *header;
    int i;

    cmp_write_fixmap(cmp, 3);

    /* Header */
    header = &xm->header;
    cmp_write_str8(cmp, "header", 6);
    cmp_write_fixmap(cmp, 15);

    cmp_write_str8(cmp, "id_text", 7);
    cmp_write_str8(cmp, header->id_text, 17);

    cmp_write_str8(cmp, "module_name", 11);
    cmp_write_str8(cmp, header->module_name, 20);

    cmp_write_str8(cmp, "magic", 5);
    cmp_write_u8(cmp, header->magicbyte);

    cmp_write_str8(cmp, "tracker_name", 12);
    cmp_write_str8(cmp, header->tracker_name, 20);

    cmp_write_str8(cmp, "version", 7);
    cmp_write_u16(cmp, header->version);

    cmp_write_str8(cmp, "header_size", 11);
    cmp_write_u32(cmp, header->header_size);

    cmp_write_str8(cmp, "song_length", 11);
    cmp_write_u16(cmp, header->song_length);

    cmp_write_str8(cmp, "restart_position", 16);
    cmp_write_u16(cmp, header->restart_position);

    cmp_write_str8(cmp, "nchannels", 9);
    cmp_write_u16(cmp, header->nchannels);

    cmp_write_str8(cmp, "npatterns", 9);
    cmp_write_u16(cmp, header->npatterns);

    cmp_write_str8(cmp, "ninstruments", 12);
    cmp_write_u16(cmp, header->ninstruments);

    cmp_write_str8(cmp, "flags", 5);
    cmp_write_u16(cmp, header->flags);

    cmp_write_str8(cmp, "tempo", 5);
    cmp_write_u16(cmp, header->tempo);

    cmp_write_str8(cmp, "bpm", 3);
    cmp_write_u16(cmp, header->bpm);

    cmp_write_str8(cmp, "ptable", 6);
    cmp_write_array16(cmp, 256);

    for (i = 0; i < 256; i++) {
        cmp_write_u8(cmp, header->ptable[i]);
    }

    /* Patterns */

    cmp_write_str8(cmp, "patterns", 8);
    cmp_write_array(cmp, header->npatterns);

    for (i = 0; i < header->npatterns; i++) {
        xm_pattern *pat;
        uint16_t n;

        pat = &xm->patterns[i];
        cmp_write_fixmap(cmp, 5);

        cmp_write_str8(cmp, "header_length", 13);
        cmp_write_u32(cmp, pat->header_length);

        cmp_write_str8(cmp, "packing_type", 12);
        cmp_write_u8(cmp, pat->packing_type);

        cmp_write_str8(cmp, "nrows", 5);
        cmp_write_u16(cmp, pat->nrows);

        cmp_write_str8(cmp, "packed_size", 11);
        cmp_write_u16(cmp, pat->packed_size);

        cmp_write_str8(cmp, "data", 4);
        cmp_write_array16(cmp, pat->packed_size);
        for (n = 0; n < pat->packed_size; n++) {
            cmp_write_u8(cmp, pat->data[n]);
        }
    }

    /* Instruments */
    cmp_write_str8(cmp, "instruments", 11);
    cmp_write_array16(cmp, header->ninstruments);

    for (i = 0; i < header->ninstruments; i++) {
        xm_instrument *ins;

        ins = &xm->instruments[i];

        if (ins->nsamples > 0) {
            cmp_write_map16(cmp, 25);
        } else {
            cmp_write_fixmap(cmp, 4);
        }
        cmp_write_str8(cmp, "header_size", 11);
        cmp_write_u32(cmp, ins->header_size);

        cmp_write_str8(cmp, "name", 4);
        cmp_write_str8(cmp, ins->name, 22);

        cmp_write_str8(cmp, "type", 4);
        cmp_write_u8(cmp, ins->type);

        cmp_write_str8(cmp, "nsamples", 8);
        cmp_write_u16(cmp, ins->nsamples);

        if (ins->nsamples > 0) {
            int k;
            cmp_write_str8(cmp, "sample_header_size", 18);
            cmp_write_u32(cmp, ins->sample_header_size);

            cmp_write_str8(cmp, "keymap", 6);
            cmp_write_array16(cmp, 96);
            for (k = 0; k < 96; k++) {
                cmp_write_u8(cmp, ins->keymap[k]);
            }

            cmp_write_str8(cmp, "vol", 3);
            cmp_write_array16(cmp, 24);
            for(k = 0; k < 24; k++) {
                cmp_write_u16(cmp, ins->vol[k]);
            }

            cmp_write_str8(cmp, "pan", 3);
            cmp_write_array16(cmp, 24);
            for(k = 0; k < 24; k++) {
                cmp_write_u16(cmp, ins->pan[k]);
            }

            cmp_write_str8(cmp, "nvol", 4);
            cmp_write_u8(cmp, ins->nvol);
            cmp_write_str8(cmp, "npan", 4);
            cmp_write_u8(cmp, ins->npan);

            cmp_write_str8(cmp, "volsus", 6);
            cmp_write_u8(cmp, ins->volsus);
            cmp_write_str8(cmp, "volstart", 8);
            cmp_write_u8(cmp, ins->volstart);
            cmp_write_str8(cmp, "volend", 6);
            cmp_write_u8(cmp, ins->volend);

            cmp_write_str8(cmp, "pansus", 6);
            cmp_write_u8(cmp, ins->pansus);
            cmp_write_str8(cmp, "panstart", 8);
            cmp_write_u8(cmp, ins->panstart);
            cmp_write_str8(cmp, "panend", 6);
            cmp_write_u8(cmp, ins->panend);

            cmp_write_str8(cmp, "vtype", 5);
            cmp_write_u8(cmp, ins->vtype);
            cmp_write_str8(cmp, "ptype", 5);
            cmp_write_u8(cmp, ins->ptype);

            cmp_write_str8(cmp, "vibtype", 7);
            cmp_write_u8(cmp, ins->vibtype);
            cmp_write_str8(cmp, "vibsweep", 8);
            cmp_write_u8(cmp, ins->vibsweep);
            cmp_write_str8(cmp, "vibdepth", 8);
            cmp_write_u8(cmp, ins->vibdepth);
            cmp_write_str8(cmp, "vibrate", 7);
            cmp_write_u8(cmp, ins->vibrate);

            cmp_write_str8(cmp, "vol_fadeout", 11);
            cmp_write_u16(cmp, ins->vol_fadeout);

            cmp_write_str8(cmp, "reserved", 8);
            cmp_write_fixarray(cmp, 11);
            for (k = 0; k < 11; k++) {
                cmp_write_u16(cmp, ins->reserved[k]);
            }


            cmp_write_str8(cmp, "samples", 7);
            cmp_write_array16(cmp, ins->nsamples);

            for (k = 0; k < ins->nsamples; k++) {
                xm_sample *s;
                uint32_t n;
                s = &ins->samples[k];
                cmp_write_fixmap(cmp, 11);

                cmp_write_str8(cmp, "length", 6);
                cmp_write_u32(cmp, s->length);

                cmp_write_str8(cmp, "loop_start", 10);
                cmp_write_u32(cmp, s->loop_start);

                cmp_write_str8(cmp, "loop_length", 11);
                cmp_write_u32(cmp, s->loop_length);

                cmp_write_str8(cmp, "volume", 6);
                cmp_write_u8(cmp, s->volume);

                cmp_write_str8(cmp, "finetune", 8);
                cmp_write_s8(cmp, s->finetune);

                cmp_write_str8(cmp, "type", 4);
                cmp_write_u8(cmp, s->type);

                cmp_write_str8(cmp, "panning", 7);
                cmp_write_u8(cmp, s->panning);

                cmp_write_str8(cmp, "rnotenum", 8);
                cmp_write_s8(cmp, s->rnotenum);

                cmp_write_str8(cmp, "compression", 11);
                cmp_write_u8(cmp, s->compression);

                cmp_write_str8(cmp, "name", 4);
                cmp_write_str8(cmp, s->name, 22);

                cmp_write_str8(cmp, "data", 4);
                cmp_write_array32(cmp, s->length);

                for(n = 0; n < s->length; n++) {
                    cmp_write_u8(cmp, s->data[n]);
                }
            }
        }
    }

}

static xmt_obj * read_object_v2(cmp_ctx_t *cmp)
{
    cmp_object_t obj;
    xmt_obj *o;

    o = NULL;

    if (!cmp_read_object(cmp, &obj)) {
        return NULL;
    }

    switch(obj.type) {
        case CMP_TYPE_FIXMAP:
        case CMP_TYPE_MAP16:
        case CMP_TYPE_MAP32: {
            size_t i;
            o = xmt_obj_mkmap(obj.as.map_size);
            for (i = 0; i < obj.as.map_size; i++) {
                xmt_obj *key, *val;
                key = read_object_v2(cmp);
                val = read_object_v2(cmp);
                xmt_obj_map_insert_v2(o, key, val, i);
            }
            break;
        }
        case CMP_TYPE_FIXSTR:
        case CMP_TYPE_STR8:
        case CMP_TYPE_STR16:
        case CMP_TYPE_STR32: {
            char *sbuf;
            sbuf = malloc(obj.as.str_size + 1);
            memset(sbuf, 0, obj.as.str_size + 1);
            moncmp_read(cmp, sbuf, obj.as.str_size);
            o = xmt_obj_mkstr(sbuf, obj.as.str_size);
            free(sbuf);
            break;
        }
        case CMP_TYPE_POSITIVE_FIXNUM:
        case CMP_TYPE_UINT8:
            o = xmt_obj_mkint(obj.as.u8);
            break;
        case CMP_TYPE_SINT8:
            o = xmt_obj_mkint(obj.as.s8);
            break;
        case CMP_TYPE_UINT16:
            o = xmt_obj_mkint(obj.as.u16);
            break;
        case CMP_TYPE_UINT32:
            o = xmt_obj_mkint(obj.as.u32);
            break;
        case CMP_TYPE_FIXARRAY:
        case CMP_TYPE_ARRAY16:
        case CMP_TYPE_ARRAY32: {
            size_t i;
            xmt_obj **vals;
            o = xmt_obj_mkarray(obj.as.array_size);
            vals = xmt_obj_array_values(o);
            for (i = 0; i < obj.as.array_size; i++) {
                vals[i] = read_object_v2(cmp);
            }
       }
            break;
        default:
            printf("not sure: %d\n", obj.type);
            break;
    }
    return o;
}

void read_u32(xmt_obj *o, uint32_t *val)
{
    /* TODO: type checking */
    xmt_obj_int *ival;
    ival = o->data;
    *val = ival->val;
}

void read_u16(xmt_obj *o, uint16_t *val)
{
    /* TODO: type checking */
    xmt_obj_int *ival;
    ival = o->data;
    *val = ival->val;
}

void read_u8(xmt_obj *o, uint8_t *val)
{
    /* TODO: type checking */
    xmt_obj_int *ival;
    ival = o->data;
    *val = ival->val;
}

void read_s8(xmt_obj *o, int8_t *val)
{
    /* TODO: type checking */
    xmt_obj_int *ival;
    ival = o->data;
    *val = (int8_t)ival->val;
}

void read_str(xmt_obj *o, char *str)
{
    /* TODO: type checking */
    xmt_obj_str *sval;
    int slen;
    sval = o->data;
    slen = strlen(sval->val);
    /* no need for null terminator (no +1) */
    memcpy(str, sval->val, slen);
}

int param_u16(xmt_obj *map, const char *key, uint16_t *val)
{
    xmt_obj *tmp;

    tmp = xmt_obj_map_lookup(map, key);

    if (tmp != NULL) {
        read_u16(tmp, val);
        return 0;
    }

    return 1;
}

int param_u32(xmt_obj *map, const char *key, uint32_t *val)
{
    xmt_obj *tmp;

    tmp = xmt_obj_map_lookup(map, key);

    if (tmp != NULL) {
        read_u32(tmp, val);
        return 0;
    }

    return 1;
}

int param_u8(xmt_obj *map, const char *key, uint8_t *val)
{
    xmt_obj *tmp;

    tmp = xmt_obj_map_lookup(map, key);

    if (tmp != NULL) {
        read_u8(tmp, val);
        return 0;
    }

    return 1;
}

int param_s8(xmt_obj *map, const char *key, int8_t *val)
{
    xmt_obj *tmp;

    tmp = xmt_obj_map_lookup(map, key);

    if (tmp != NULL) {
        read_s8(tmp, val);
        return 0;
    }

    return 1;
}

void param_str(xmt_obj *map, const char *key, char *val)
{
    xmt_obj *tmp;
    tmp = xmt_obj_map_lookup(map, key);
    if (tmp != NULL) {
        read_str(tmp, val);
    }
}

void load_v2(xm_module *xm, cmp_ctx_t *cmp)
{
    xmt_obj *o;
    xmt_obj *ho;
    xmt_obj *po;
    xmt_obj *io;

    o = read_object_v2(cmp);

    ho = xmt_obj_map_lookup(o, "header");
    po = xmt_obj_map_lookup(o, "patterns");
    io = xmt_obj_map_lookup(o, "instruments");

    if (ho != NULL) {
        xmt_obj *tmp;
        param_str(ho, "id_text", xm->header.id_text);
        param_str(ho, "module_name", xm->header.module_name);
        param_u8(ho, "magic", (uint8_t *)&xm->header.magicbyte);
        param_str(ho, "tracker_name", xm->header.tracker_name);
        param_u16(ho, "version", &xm->header.version);
        param_u32(ho, "header_size", &xm->header.header_size);
        param_u16(ho, "song_length", &xm->header.song_length);
        param_u16(ho, "restart_position", &xm->header.restart_position);
        param_u16(ho, "nchannels", &xm->header.nchannels);
        param_u16(ho, "npatterns", &xm->header.npatterns);
        param_u16(ho, "ninstruments", &xm->header.ninstruments);
        param_u16(ho, "flags", &xm->header.flags);
        param_u16(ho, "tempo", &xm->header.tempo);
        param_u16(ho, "bpm", &xm->header.bpm);

        tmp = xmt_obj_map_lookup(ho, "ptable");

        if (tmp != NULL) {
            int i;
            xmt_obj_array *arr;
            arr = tmp->data;
            for (i = 0; i < arr->length; i++) {
                read_u8(arr->val[i], &xm->header.ptable[i]);
            }
        }

    }

    if (po != NULL) {
        uint16_t p;
        xmt_obj_array *plist;
        xm_pattern *patterns;

        xm->patterns = calloc(1, sizeof(xm_pattern) * xm->header.npatterns);
        patterns = xm->patterns;
        plist = po->data;
        for (p = 0; p < plist->length; p++) {
            xmt_obj *pat;
            xmt_obj *tmp;
            pat = plist->val[p];
            patterns[p].data = NULL;
            param_u32(pat, "header_length", &patterns[p].header_length);
            param_u8(pat, "packing_type", &patterns[p].packing_type);
            param_u16(pat, "nrows", &patterns[p].nrows);
            param_u16(pat, "packed_size", &patterns[p].packed_size);

            patterns[p].data = calloc(1, patterns[p].packed_size);

            tmp = xmt_obj_map_lookup(pat, "data");

            if (tmp != NULL) {
                uint16_t d;
                xmt_obj_array *pdat;
                pdat = tmp->data;
                for (d = 0; d < patterns[p].packed_size; d++) {
                    read_u8(pdat->val[d], &patterns[p].data[d]);
                }
            }
        }
    }

    if (io != NULL) {
        xmt_obj_array *ilist;
        xm_instrument *instruments;
        int i;
        xm->instruments =
            calloc(1, sizeof(xm_instrument) * xm->header.ninstruments);
        instruments = xm->instruments;
        ilist = io->data;

        for (i = 0; i < ilist->length; i++) {
            xmt_obj *ins;
            ins = ilist->val[i];
            param_u32(ins, "header_size", &instruments[i].header_size);
            param_str(ins, "name", instruments[i].name);
            param_u8(ins, "type", &instruments[i].type);
            param_u16(ins, "nsamples", &instruments[i].nsamples);

            if (instruments[i].nsamples > 0) {
                xmt_obj *tmp;
                instruments[i].samples =
                    malloc(sizeof(xm_sample) * instruments[i].nsamples);
                param_u32(ins, "sample_header_size", &instruments[i].sample_header_size);

                tmp = xmt_obj_map_lookup(ins, "keymap");
                if (tmp != NULL) {
                    xmt_obj_array *arr;
                    int k;
                    arr = tmp->data;
                    for (k = 0; k < arr->length; k++) {
                        read_u8(arr->val[k], &instruments[i].keymap[k]);
                    }
                }

                tmp = xmt_obj_map_lookup(ins, "vol");
                if (tmp != NULL) {
                    xmt_obj_array *arr;
                    int k;
                    arr = tmp->data;
                    for (k = 0; k < arr->length; k++) {
                        read_u16(arr->val[k], &instruments[i].vol[k]);
                    }
                }

                tmp = xmt_obj_map_lookup(ins, "pan");
                if (tmp != NULL) {
                    xmt_obj_array *arr;
                    int k;
                    arr = tmp->data;
                    for (k = 0; k < arr->length; k++) {
                        read_u16(arr->val[k], &instruments[i].vol[k]);
                    }
                }

                param_u8(ins, "nvol", &instruments[i].nvol);
                param_u8(ins, "npan", &instruments[i].npan);

                param_u8(ins, "volsus", &instruments[i].volsus);
                param_u8(ins, "volstart", &instruments[i].volstart);
                param_u8(ins, "volend", &instruments[i].volend);

                param_u8(ins, "pansus", &instruments[i].pansus);
                param_u8(ins, "panstart", &instruments[i].panstart);
                param_u8(ins, "panend", &instruments[i].panend);

                param_u8(ins, "vtype", &instruments[i].vtype);
                param_u8(ins, "ptype", &instruments[i].ptype);

                param_u8(ins, "vibtype", &instruments[i].vibtype);
                param_u8(ins, "vibsweep", &instruments[i].vibsweep);
                param_u8(ins, "vibdepth", &instruments[i].vibdepth);
                param_u8(ins, "vibrate", &instruments[i].vibrate);

                param_u16(ins, "vol_fadeout", &instruments[i].vol_fadeout);

                tmp = xmt_obj_map_lookup(ins, "reserved");

                if (tmp != NULL) {
                    xmt_obj_array *arr;
                    int k;
                    arr = tmp->data;
                    for (k = 0; k < arr->length; k++) {
                        read_u16(arr->val[k], &instruments[i].reserved[k]);
                    }
                }

                tmp = xmt_obj_map_lookup(ins, "samples");
                if (tmp != NULL) {
                    xmt_obj_array *slist;
                    int s;

                    slist = tmp->data;
                    for (s = 0; s < instruments[i].nsamples; s++) {
                        xm_sample *smp;
                        smp = &instruments[i].samples[s];
                        param_u32(slist->val[s], "length", &smp->length);
                        param_u32(slist->val[s], "loop_start", &smp->loop_start);
                        param_u32(slist->val[s], "loop_length", &smp->loop_length);
                        param_u8(slist->val[s], "volume", &smp->volume);
                        param_s8(slist->val[s], "finetune", &smp->finetune);
                        param_u8(slist->val[s], "type", &smp->type);
                        param_u8(slist->val[s], "panning", &smp->panning);
                        param_s8(slist->val[s], "rnotenum", &smp->rnotenum);
                        param_u8(slist->val[s], "compression", &smp->compression);
                        param_str(slist->val[s], "name", smp->name);

                        if (smp->length > 0) {
                            tmp = xmt_obj_map_lookup(slist->val[s], "data");
                            smp->data = calloc(1, smp->length);
                            if (tmp != NULL) {
                                uint32_t n;
                                xmt_obj_array *dat;
                                dat = tmp->data;
                                for (n = 0; n < dat->length; n++) {
                                    read_u8(dat->val[n], &smp->data[n]);
                                }
                            }
                        } else {
                            smp->data = NULL;
                        }
                    }
                }
            } else {
                instruments[i].samples = NULL;
            }
        }

    }

    xmt_obj_del(&o);
}

void load_from_file_v2(xm_module *xm, const char *filename)
{
    uint8_t *buf;
    size_t sz;
    FILE *fp;
    moncmp_d m;
    cmp_ctx_t cmp;

    fp = fopen(filename, "rb");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);

    buf = malloc(sz);
    fseek(fp, 0L, SEEK_SET);
    fread(buf, 1, sz, fp);
    fclose(fp);
    moncmp_init_read(&m, &cmp, buf, sz);
    load_v2(xm, &cmp);
    free(buf);
}

int streq(const char *str1, size_t sz1,
          const char *str2, size_t sz2)
{
    size_t i;
    if (sz1 != sz2) return 0;

    for (i = 0; i < sz1; i++) {
        if (str1[i] != str2[i]) return 0;
    }

    return 1;
}

#define STRCMP(NAME) \
if (strcmp(xm->NAME, ref->NAME)) { \
    printf(#NAME ": %s != %s\n", xm->NAME, ref->NAME); \
    nerr++; \
}

/* STRCMP2 works with non-null-terminated strings */
#define STRCMP2(NAME, SZ) \
if (!streq(xm->NAME, SZ, ref->NAME, SZ)) { \
    printf(#NAME ": '"); \
    fwrite(xm->NAME, 1, SZ, stdout); \
    printf("' != '"); \
    fwrite(ref->NAME, 1, SZ, stdout); \
    printf("'\n"); \
    nerr++; \
}

#define INTCMP(NAME) \
if (xm->NAME != ref->NAME) { \
    printf(#NAME ": %d != %d\n", xm->NAME, ref->NAME); \
    nerr++; \
}

void verify(xm_module *ref, const char *filename)
{
    xm_module *xm;
    int nerr;
    int i;
    FILE *fp;
    xm_iodat w;
    xm = calloc(1, sizeof(xm_module));

    nerr = 0;
    load_from_file_v2(xm, filename);

    STRCMP2(header.id_text, 17);
    STRCMP2(header.module_name, 20);
    INTCMP(header.magicbyte);
    STRCMP2(header.tracker_name, 20);
    INTCMP(header.version);
    INTCMP(header.header_size);
    INTCMP(header.song_length);
    INTCMP(header.restart_position);
    INTCMP(header.nchannels);
    INTCMP(header.npatterns);
    INTCMP(header.ninstruments);
    INTCMP(header.flags);
    INTCMP(header.tempo);
    INTCMP(header.bpm);

    for (i = 0; i < 256; i++) {
        int last;
        last = nerr;
        INTCMP(header.ptable[i]);
        if (last != nerr) break;
    }

    if (xm->patterns != NULL) {
        for (i = 0; i < ref->header.npatterns; i++) {
            int last;
            uint16_t k;
            last = nerr;

            INTCMP(patterns[i].header_length);
            INTCMP(patterns[i].packing_type);
            INTCMP(patterns[i].nrows);
            INTCMP(patterns[i].packed_size);

            if (xm->patterns[i].data == NULL && ref->patterns[i].packed_size > 0) {
                printf("data is NULL for non-empty pattern\n");
                nerr++;
                break;
            }

            for (k = 0; k < ref->patterns[i].packed_size; k++) {
                int last;
                last = nerr;
                INTCMP(patterns[i].data[k]);

                if (last != nerr) break;
            }

            if (last != nerr) break;
        }
    } else {
        printf("patterns is NULL\n");
        nerr++;
    }

    if (xm->instruments != NULL) {
        int i;

        for (i = 0; i < xm->header.ninstruments; i++) {
            /* NOTE: 'last' has nested scope, redefined in inner loops */
            int last;
            last = nerr;
            INTCMP(instruments[i].header_size);
            STRCMP2(instruments[i].name, 22);
            INTCMP(instruments[i].type);
            INTCMP(instruments[i].nsamples);

            if (last != nerr) break;

            if (ref->instruments[i].nsamples > 0 &&
                xm->instruments[i].samples == NULL) {
                printf("Non-empty sample data is NULL.\n");
                nerr++;
                break;
            }


            if (ref->instruments[i].nsamples > 0) {
                int k;
                int s;
                int last;

                INTCMP(instruments[i].sample_header_size);

                for (k = 0; k < 96; k++) {
                    int last;
                    last = nerr;
                    INTCMP(instruments[i].keymap[k]);
                    if (last != nerr) break;
                }

                for (k = 0; k < 24; k++) {
                    int last;
                    last = nerr;
                    INTCMP(instruments[i].vol[k]);
                    INTCMP(instruments[i].pan[k]);
                    if (last != nerr) break;
                }

                INTCMP(instruments[i].nvol);
                INTCMP(instruments[i].npan);

                INTCMP(instruments[i].volsus);
                INTCMP(instruments[i].volstart);
                INTCMP(instruments[i].volend);

                INTCMP(instruments[i].pansus);
                INTCMP(instruments[i].panstart);
                INTCMP(instruments[i].panend);

                INTCMP(instruments[i].vtype);
                INTCMP(instruments[i].ptype);

                INTCMP(instruments[i].vibtype);
                INTCMP(instruments[i].vibsweep);
                INTCMP(instruments[i].vibdepth);
                INTCMP(instruments[i].vibrate);

                INTCMP(instruments[i].vol_fadeout);

                for (k = 0; k < 11; k++) {
                    INTCMP(instruments[i].reserved[k]);
                }

                last = nerr;

                for (s = 0; s < xm->instruments[i].nsamples; s++) {
                    int last;
                    int null_and_nonempty;
                    uint32_t n;

                    last = nerr;
                    INTCMP(instruments[i].samples[s].length);
                    INTCMP(instruments[i].samples[s].loop_start);
                    INTCMP(instruments[i].samples[s].loop_length);
                    INTCMP(instruments[i].samples[s].volume);
                    INTCMP(instruments[i].samples[s].finetune);
                    INTCMP(instruments[i].samples[s].type);
                    INTCMP(instruments[i].samples[s].panning);
                    INTCMP(instruments[i].samples[s].rnotenum);
                    INTCMP(instruments[i].samples[s].compression);
                    STRCMP2(instruments[i].samples[s].name, 22);
                    if (last != nerr) break;

                    null_and_nonempty =
                        ref->instruments[i].samples[s].length > 0 &&
                        xm->instruments[i].samples[s].data == NULL;

                    if (null_and_nonempty) {
                        printf("sample data is null and nonempty\n");
                        nerr++;
                        break;
                    }

                    /* TODO: data */
                    for (n = 0; n < ref->instruments[i].samples[s].length; n++) {
                        int last;

                        last = nerr;
                        INTCMP(instruments[i].samples[s].data[n]);
                        if (last != nerr) break;
                    }
                }

                if (last != nerr) break;

            }

        }

    } else {
        printf("instruments is NULL\n");
        nerr++;
    }

    if (nerr == 0) {
        printf("all okay!\n");
    }

    /* used to verify byte-for-byte identical modules */

    fp = fopen("acidout2.xm", "wb");
    w.fp = fp;
    xm_module_write(xm, &w);
    fclose(fp);

    xm_module_free(&xm);
}

void acidjazz(void)
{
    FILE *fp;
    const char *filename;
    FILE *out;
    xm_iodat w, r;
    xm_module *xm;

    filename = "tempest-acidjazz.xm";

    fp = fopen(filename, "rb");
    r.fp = fp;
    xm = xm_module_read(&r);
    fclose(fp);

    {
        size_t msgsz;
        moncmp_d mc;
        cmp_ctx_t cmp;
        uint8_t *msg;
        FILE *fp;
        msgsz = 0;
        moncmp_init_getsize(&cmp, &msgsz);
        dump(xm, &cmp);
        msg = malloc(msgsz);
        moncmp_init_write(&mc, &cmp, msg);
        dump(xm, &cmp);
        fp = fopen("acidout.bin", "wb");
        fwrite(msg, 1, msgsz, fp);
        fclose(fp);
        free(msg);
    }

    verify(xm, "acidout.bin");

    out = fopen("acidout.xm", "wb");
    w.fp = out;
    xm_module_write(xm, &w);
    fclose(out);

    xm_module_free(&xm);
}

xm_module * xm_module_new(void)
{
    xm_module *xm;
    xm = calloc(1, sizeof(xm_module));
    return xm;
}

int xmt_main(int argc, char *argv[])
{
    const char *ifname;
    const char *ofname;
    int argpos;
    int i;
    int decode;
    if (argc < 3) {
        fprintf(stderr, "Usage %s in.bin out.xm\n", argv[0]);
        return 1;
    }

    argpos = 1;

    decode = 0;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'd') {
                decode = 1;
            }
            argpos = i + 1;
        } else {
            break;
        }
    }


    ifname = argv[argpos];
    ofname = argv[argpos + 1];

    if (decode == 0) {
        xm_module *xm;
        FILE *fp;
        xm_iodat w;

        xm = xm_module_new();

        load_from_file_v2(xm, ifname);

        fp = fopen(ofname, "wb");
        w.fp = fp;
        xm_module_write(xm, &w);
        fclose(fp);
        xm_module_free(&xm);
    } else {
        size_t msgsz;
        moncmp_d mc;
        cmp_ctx_t cmp;
        uint8_t *msg;
        FILE *fp;
        xm_iodat r;
        xm_module *xm;

        fp = fopen(ifname, "rb");

        r.fp = fp;
        xm = xm_module_read(&r);
        fclose(fp);
        msgsz = 0;
        moncmp_init_getsize(&cmp, &msgsz);
        dump(xm, &cmp);
        msg = malloc(msgsz);
        moncmp_init_write(&mc, &cmp, msg);
        dump(xm, &cmp);
        fp = fopen(ofname, "wb");
        fwrite(msg, 1, msgsz, fp);
        fclose(fp);
        free(msg);
        xm_module_free(&xm);
    }

    return 0;
}

#ifdef XMT_MAIN
int main(int argc, char *argv[])
{
    return xmt_main(argc, argv);
}
#endif
