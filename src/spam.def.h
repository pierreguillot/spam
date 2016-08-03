/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_def_h
#define spam_def_h
#include "../pd/src/m_pd.h"

#define __spam_version_major__ 0
#define __spam_version_minor__ 0
#define __spam_version_debug__ 1
#define __spam_version_string__ "0.0.1"

#define __spam_in_string__ "in"
#define __spam_in_tilde_string__ "in_tilde"
#define __spam_out_string__ "out"
#define __spam_out_tilde_string__ "out_tilde"

#define __spam_io_init__ "ioinit"
#define __spam_io_dsp__ "iodsp"

#define spam_string_owned       "owned"
#define spam_string_borrowed    "borrowed"
#define spam_string_shared      "shared"
#define spam_string_in          "in"


typedef struct _spam_tie
{
    t_object  s_object;
    int       s_index;
    t_object* s_process;
} t_spam_tie;

typedef struct _spam_io
{
    t_object            s_object;
    struct _spam_tie*   s_tie;
    int         s_index;
    int         s_type;     //!< 0 for inlet, 1 for outlet
    int         s_signal;   //!< if ~ object
    int         s_static;   //!< if static
    t_symbol*   s_symbol;
    t_sample*   s_samples;
    int         s_n;
}t_spam_io;

// Internal inlet & outlet
typedef struct _spam_inlet
{
    t_class*    s_pd;
    t_symbol*   s_sym;
} t_spam_inlet;

typedef struct _spam_outlet
{
    t_class*    s_pd;
    t_outlet*   s_outlet;
    t_symbol*   s_sym;
} t_spam_outlet;

#endif // spam_def_h



