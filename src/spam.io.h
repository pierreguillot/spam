/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_io_h
#define spam_io_h
#include "../pd/src/m_pd.h"

#define __spam_version_major__ 0
#define __spam_version_minor__ 0
#define __spam_version_debug__ 1
#define __spam_version_string__ "0.0.1"

typedef struct _spam_io
{
    t_object    s_object;
    t_canvas*   s_canvas;
    int         s_index;
    int         s_type;     //!< 0 for inlet, 1 for outlet
    int         s_signal;   //!< if ~ object
    int         s_static;   //!< if static
    t_symbol*   s_symbol;
    t_sample*   s_samples;
}t_spam_io;

char spam_io_init(t_spam_io* io, int idx, int type, int sig, int stat);
char spam_io_free(t_spam_io* io);
char spam_io_dsp(t_spam_io* io);

#endif // spam_io_h



