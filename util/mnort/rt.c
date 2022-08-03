#include <jack/jack.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/lil/lil.h"
#include "sndkit/nodes/sklil.h"
#include "hotswap.h"

typedef struct {
    mno_hotswap *hs;
    jack_port_t *out[2];
    jack_client_t *client;
} mno_rt;

static int jack_process(jack_nframes_t nframes, void *arg)
{
    jack_default_audio_sample_t *out[2];
    mno_rt *rt;

    rt = (mno_rt *)arg;
    out[0] = (jack_default_audio_sample_t*)
        jack_port_get_buffer(rt->out[0], nframes);
    out[1] = (jack_default_audio_sample_t*)
        jack_port_get_buffer(rt->out[1], nframes);

    mno_hotswap_render(rt->hs, nframes, out);
    return 0;
}

static void delrt(void *ptr)
{
    mno_rt *rt;

    rt = ptr;

    if (rt->client != NULL) {
        jack_client_close(rt->client);
        rt->client = NULL;
    }
}

int mno_rt_new(sk_core *core,
               mno_hotswap *hs,
               const char *key, int len)
{
    mno_rt *rt;
    int rc;
    const char **ports;
    const char *client_name;
    const char *server_name;
    jack_options_t options;
    jack_status_t status;
    gf_patch *patch;


    rt = malloc(sizeof(mno_rt));
    rc = sk_core_append(core, key, len, rt, delrt);

    if (rc) {
        return rc;
    }

    rt->hs = hs;
    rt->out[0] = NULL;
    rt->out[1] = NULL;
    rt->client = NULL;

    options = JackNullOption;

    server_name=NULL;
    client_name= "mnolth";

    rt->client =
        jack_client_open(client_name, options, &status, server_name);

    if (status & JackServerStarted) {
        /* error? */
    }

    if (status & JackNameNotUnique) {
        client_name = jack_get_client_name(rt->client);
        fprintf(stderr, "unique name `%s' assigned\n", client_name);
    }

    jack_set_process_callback(rt->client, jack_process, rt);


    rt->out[0] = jack_port_register(rt->client, "output1",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput, 0);

    rt->out[1] = jack_port_register(rt->client, "output2",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput, 0);

    if((rt->out[0] == NULL) || (rt->out[1] == NULL)) {
        fprintf(stderr, "no more JACK ports available\n");
        return 1;
    }

    if(jack_activate(rt->client)) {
        fprintf(stderr, "cannot activate client\n");
        return 1;
    }

    ports = jack_get_ports(rt->client, NULL, NULL,
                           JackPortIsPhysical|JackPortIsInput);

    if (jack_connect(rt->client, jack_port_name(rt->out[0]), ports[0])) {
        fprintf (stderr, "cannot connect output ports\n");
        return 1;
    }

    if (jack_connect(rt->client, jack_port_name(rt->out[1]), ports[1])) {
        fprintf (stderr, "cannot connect output ports\n");
        return 1;
    }

    patch = mno_hotswap_patch(rt->hs);
    gf_patch_srate_set(patch, jack_get_sample_rate(rt->client));

    jack_free(ports);

    return 0;
}

static lil_value_t l_rtnew(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    sk_core *core;
    int rc;
    void *ud;
    mno_hotswap *hs;
    const char *key;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "rtnew", argc, 2);

    key = lil_to_string(argv[1]);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    mno_rt_new(core, hs, key, strlen(key));
    return NULL;
}

static lil_value_t l_keypress(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    getchar();
    return NULL;
}

void lil_load_rt(lil_t lil)
{
    lil_register(lil, "rtnew", l_rtnew);
    lil_register(lil, "keypress", l_keypress);
}

void lil_load_hotswap(lil_t lil);
void mno_load(lil_t lil);

void mnort_loader(lil_t lil)
{
    mno_load(lil);
    lil_load_hotswap(lil);
    lil_load_rt(lil);
}
