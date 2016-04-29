/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_utils_h
#define spam_utils_h
#include "../pd/src/m_pd.h"

#define __spam_version__ "0.0.1"

typedef void (*t_spam_bang_method)(t_object* x);
typedef struct _spam_master
{
    t_canvas*   s_cnv;
    t_object*   s_block;
    t_spam_bang_method s_block_tick;
}t_spam_master;

char spam_master_init(t_object* process,t_spam_master* master);
char spam_master_free(t_object* process, t_spam_master* master);
char spam_master_load_canvas(t_spam_master* master, t_symbol* name, int preargc, t_atom* preargv, int argc, t_atom* argv);


typedef struct _spam_signal
{
    t_sample*   s_samples;
    size_t      s_nchannels;
    size_t      s_blocksize;
}t_spam_signal;

void spam_signal_init(t_spam_signal* signal);
char spam_signal_alloc(t_spam_signal* signal, size_t nchannels, size_t blocksize);
void spam_signal_free(t_spam_signal* signal);

typedef struct _spam_io
{
    t_object    s_object;
    t_canvas*   s_canvas;
    int         s_index;
    char        s_type;     //!< 0 for inlet, 1 for outlet
    char        s_signal;   //!< if ~ object
    char        s_static;   //!< if static
    t_symbol*   s_symbol;
    t_sample*   s_samples;
}t_spam_io;

char spam_io_init(t_spam_io* io, int idx, char type, char sig, char stat);
char spam_io_free(t_spam_io* io);
char spam_io_notify(t_spam_io* io);
char spam_io_dsp(t_spam_io* io);



#endif // spam_utils_h



