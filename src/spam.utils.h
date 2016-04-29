/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_utils_h
#define spam_utils_h
#include "spam.io.h"

typedef void (*t_spam_bang_method)(t_object* x);

typedef struct _spam_sub_canvas
{
    t_canvas*   s_canvas;
    int         s_index;
}t_spam_sub_canvas;

typedef struct _spam_master
{
    t_object            s_object;
    t_canvas*           s_canvas;
    int                 s_n;
    t_spam_sub_canvas*  s_subcanvas;
    t_object*           s_block;
    t_spam_bang_method  s_block_tick;
}t_spam_master;

char spam_master_init(t_spam_master* master, t_symbol* name, int n, int argc, t_atom* argv);
char spam_master_free(t_spam_master* master);





typedef struct _spam_signal
{
    t_sample*   s_samples;
    size_t      s_nchannels;
    size_t      s_blocksize;
}t_spam_signal;

void spam_signal_init(t_spam_signal* signal);
char spam_signal_alloc(t_spam_signal* signal, size_t nchannels, size_t blocksize);
void spam_signal_free(t_spam_signal* signal);







struct _spam_inlet;
struct _spam_outlet;

void setup_iolet_setup(void);

typedef struct _spam_iolets
{
    int             staticins;
    int             nins_message;
    int             nins_signal;
    int             staticout;
    int             nout_message;
    int             nouts_signal;
    struct _spam_inlet*  ins;
    struct _spam_outlet* outs;
} t_spam_iolets;

void spam_iolets_init(t_spam_iolets* iolets);
void spam_iolets_free(t_spam_iolets* iolets);
void spam_iolets_set(t_spam_iolets* iolets, t_spam_io* io);
void spam_iolets_set_samples(t_spam_iolets* iolets, t_spam_io* io, t_spam_signal* signal, int nstatic);
int spam_iolets_has_insig(t_spam_iolets* iolets);
int spam_iolets_has_outsig(t_spam_iolets* iolets);
int spam_iolets_get_ninsig(t_spam_iolets* iolets, int nstatics);
int spam_iolets_get_noutsig(t_spam_iolets* iolets, int nstatics);
void spam_iolets_newio(t_spam_iolets* iolets, t_object* x, t_canvas* cnv, int nstatics);

#endif // spam_utils_h



