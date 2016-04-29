/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_utils_h
#define spam_utils_h
#include "../pd/src/m_pd.h"

typedef struct _spam_master
{
    t_canvas*   s_cnv;
    t_object*   s_block;
}t_spam_master;

char spam_master_init(t_spam_master* master);
char spam_master_close(t_spam_master* master);
char spam_master_load_canvas(t_spam_master* master, t_symbol* name, int preargc, t_atom* preargv, int argc, t_atom* argv);


typedef struct _spam_signal
{
    t_sample*   s_samples;
    size_t      s_nchannels;
    size_t      s_blocksize;
}t_spam_signal;

void spam_signal_init(t_spam_signal* signal);
void spam_signal_alloc(t_spam_signal* signal, size_t nchannels, size_t blocksize);
void spam_signal_free(t_spam_signal* signal);

t_canvas* spam_get_toplevel();


#endif // spam_utils_h



