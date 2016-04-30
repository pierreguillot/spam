/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.io.h"
#include "spam.tie.h"

char spam_io_init(t_spam_io* io, int idx, int type, int sig, int stat)
{
    t_object* process;
    io->s_tie       = spam_tie_get(canvas_getcurrent());
    io->s_index     = idx;
    io->s_type      = ((type != 0) ? 1 : 0);
    io->s_signal    = ((sig != 0) ? 1 : 0);
    io->s_static    = ((stat != 0) ? 1 : 0);
    io->s_symbol    = NULL;
    io->s_samples   = NULL;
    if(!io->s_tie)
    {
        return 0;
    }
    if(io->s_index < 0)
    {
        pd_error(io, "spam.io: index must be superior or equal to zero.");
        return -1;
    }
    if(io->s_index && io->s_static)
    {
        pd_error(io, "spam.io: can't be static and have an index.");
        return -1;
    }
    if(!io->s_type)
    {
        if(!io->s_signal)
        {
            io->s_symbol = spam_tie_gensym(io->s_tie, __spam_in_string__, io->s_index);
        }
        else
        {
            io->s_symbol = spam_tie_gensym(io->s_tie, __spam_in_tilde_string__, io->s_index);
        }
        if(io->s_symbol)
        {
            pd_bind((t_pd *)io, io->s_symbol);
        }
        else
        {
            pd_error(io, "spam.io: can't bind the object.");
            return -1;
        }
    }
    else
    {
        if(!io->s_signal)
        {
            io->s_symbol = spam_tie_gensym(io->s_tie, __spam_out_string__, io->s_index);
        }
        else
        {
            io->s_symbol = spam_tie_gensym(io->s_tie, __spam_out_tilde_string__, io->s_index);
        }
    }
    process = spam_tie_get_process(io->s_tie);
    if(process)
    {
        mess1((t_pd *)process, gensym(__spam_io_init__), (void *)(io));
    }
    return 0;
}

char spam_io_free(t_spam_io* io)
{
    if(!io->s_type && io->s_symbol)
    {
        pd_unbind((t_pd *)io, io->s_symbol);
    }
    return 0;
}

char spam_io_dsp(t_spam_io* io)
{
    t_object* process = spam_tie_get_process(io->s_tie);
    if(process)
    {
        mess1((t_pd *)process, gensym(__spam_io_dsp__), (void *)(io));
        return 0;
    }
    return -1;
}



