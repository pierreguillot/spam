/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_utils_h
#define spam_utils_h
#include "spam.io.h"

typedef void (*t_spam_bang_method)(t_object* x);
struct _spam_inlet;
struct _spam_outlet;

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


typedef struct _spam_master
{
    t_object            s_object;
    t_canvas*           s_canvas;
    int                 s_n;
    t_object*           s_block;
    t_canvas**          s_subcanvas;
    int                 s_currentn;
    t_spam_iolets       s_iolets;
    t_signal**          s_sigs;
    t_sample*           s_outputs;
    t_sample**          s_routputs;
    int                 s_blksize;
}t_spam_master;


void spam_master_setup(void);
char spam_master_init(t_spam_master* master, t_symbol* name, int n, int argc, t_atom* argv);
char spam_master_free(t_spam_master* master);
char spam_master_visible(t_spam_master* master, int index);
char spam_master_dsp(t_spam_master* master, t_signal **sp);
void spam_master_io_init(t_spam_master* master, t_spam_io* io);
void spam_master_io_dsp(t_spam_master* master, t_spam_io* io);
int spam_master_get_nsignals(t_spam_master* master, int type);









#endif // spam_utils_h



