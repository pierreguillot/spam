/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.io.h"
#include "../pd/src/g_canvas.h"
#include <string.h>
#include <stdarg.h>

static t_canvas* spam_io_get_toplevel()
{
    t_canvas* cnv = canvas_getcurrent();
    while(cnv && cnv->gl_owner)
    {
        cnv = cnv->gl_owner;
    }
    return cnv;
}

static t_symbol* spam_io_gensym(t_canvas* cnv, char* txt, ...)
{
    va_list ap;
    t_symbol* s = NULL;
    char temp[MAXPDSTRING];
    if(cnv)
    {
        va_start(ap, txt);
        vsnprintf(temp, MAXPDSTRING-1, txt, ap);
        va_end(ap);
        return canvas_realizedollar(cnv, gensym(temp));
    }
    return s;
}

static t_object* spam_get_io_process(t_canvas* cnv)
{
    t_symbol* s = NULL;
    if(cnv)
    {
        s = canvas_realizedollar(cnv, gensym("$0-spam-process"));
        if(s)
        {
            return (t_object *)s->s_thing;
        }
    }
    return NULL;
}


char spam_io_init(t_spam_io* io, int idx, int type, int sig, int stat)
{
    t_object* process;
    io->s_canvas    = spam_io_get_toplevel();
    io->s_index     = idx;
    io->s_type      = ((type != 0) ? 1 : 0);
    io->s_signal    = ((sig != 0) ? 1 : 0);
    io->s_static    = ((stat != 0) ? 1 : 0);
    io->s_symbol    = NULL;
    io->s_samples   = NULL;
    if(io->s_canvas < 0)
    {
        pd_error(io, "spam.io: can't find canvas.");
        return -1;
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
            io->s_symbol = spam_io_gensym(io->s_canvas, "$0-in-%i", io->s_index);
        }
        else
        {
            io->s_symbol = spam_io_gensym(io->s_canvas, "$0-in-tilde-%i", io->s_index);
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
            io->s_symbol = spam_io_gensym(io->s_canvas, "$0-out-%i", io->s_index);
        }
        else
        {
            io->s_symbol = spam_io_gensym(io->s_canvas, "$0-out-tilde-%i", io->s_index);
        }
    }
    process = spam_get_io_process(io->s_canvas);
    if(process)
    {
        mess1((t_pd *)process, gensym("setio"), (void *)(io));
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
    t_object* process = spam_get_io_process(io->s_canvas);
    if(process)
    {
        mess1((t_pd *)process, gensym("getsamples"), (void *)(io));
        return 0;
    }
    return -1;
}



