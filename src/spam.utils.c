/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"
#include "../pd/src/g_canvas.h"
#include <strings.h>
#include <stdarg.h>

extern t_class* canvas_class;

char spam_master_init(t_spam_master* master)
{
    t_atom av[3];
    master->s_cnv = canvas_new(NULL, gensym(""), 0, NULL);
    if(master->s_cnv)
    {
        pd_popsym((t_pd *)master->s_cnv);
        canvas_vis(master->s_cnv, 0);
        SETFLOAT(av, 10);
        SETFLOAT(av+1, 10);
        SETSYMBOL(av+2, gensym("switch~"));
        pd_typedmess((t_pd *)master->s_cnv, gensym("obj"), 3, av);
        if(!strncmp(class_getname(master->s_cnv->gl_list->g_pd), "block~", 6))
        {
            master->s_block = (t_object *)master->s_cnv->gl_list;
        }
        else
        {
            error("spam: can't allocate master switch~ object.");
            return -1;
        }
    }
    else
    {
        error("spam: can't allocate master canvas.");
        return -1;
    }
    return 0;
}

char spam_master_close(t_spam_master* master)
{
    if(master->s_cnv)
    {
        canvas_free(master->s_cnv);
        return 0;
    }
    return -1;
}

char spam_master_load_canvas(t_spam_master* master, t_symbol* name, int preargc, t_atom* preargv, int argc, t_atom* argv)
{
    int i, n, v;
    t_gobj* z;
    t_atom* vec, *av;
    if(master->s_cnv)
    {
        av = getbytes((preargc + 3) * sizeof(t_atom));
        if(av)
        {
            SETFLOAT(av, 1);
            SETFLOAT(av+1, 1);
            SETSYMBOL(av+2, name);
            memcpy(av+3, preargv, preargc * sizeof(t_atom));
            pd_typedmess((t_pd *)master->s_cnv, gensym("obj"), (preargc + 3), av);
            
            for(z = master->s_cnv->gl_list; z; z = z->g_next)
            {
                if(z->g_pd == canvas_class)
                {
                    n   = binbuf_getnatom(((t_object *)z)->te_binbuf);
                    vec = binbuf_getvec(((t_object *)z)->te_binbuf);
                    if(n && vec && n >= preargc+1 && atom_getsymbol(vec) == name)
                    {
                        v = 1;
                        for(i = 0; i < preargc; ++i)
                        {
                            if((vec[i+1].a_type != preargv[i].a_type) || (vec[i+1].a_type == A_FLOAT && atom_getfloat(vec+i+1) != atom_getfloat(preargv+i)) || (vec[i+1].a_type == A_SYMBOL && atom_getsymbol(vec+i+1) != atom_getsymbol(preargv+i)))
                            {
                                v = 0;
                                break;
                            }
                        }
                        if(v)
                        {
                            canvas_loadbang((t_canvas *)z);
                            return 0;
                        }
                    }
                }
            }
            freebytes(av, (preargc + 3) * sizeof(t_atom));
        }
    }
    return -1;
}


void spam_signal_init(t_spam_signal* signal)
{
    signal->s_samples   = NULL;
    signal->s_nchannels = 0;
    signal->s_blocksize = 0;
}

void spam_signal_alloc(t_spam_signal* signal, size_t nchannels, size_t blocksize)
{
    t_sample* temp;
    const size_t size = signal->s_blocksize * signal->s_nchannels * sizeof(t_sample);
    if(signal->s_samples && size)
    {
        temp = (t_sample *)resizebytes(signal->s_samples, size, nchannels * blocksize * sizeof(t_sample));
        if(temp)
        {
            signal->s_samples = temp;
            signal->s_blocksize = blocksize;
            signal->s_nchannels = nchannels;
        }
        else
        {
            freebytes(signal->s_samples, size);
            signal->s_samples   = NULL;
            signal->s_blocksize = 0;
            signal->s_nchannels = 0;
        }
    }
    else if(nchannels * blocksize)
    {
        signal->s_samples = (t_sample *)getbytes(nchannels * blocksize * sizeof(t_sample));
        if(signal->s_samples)
        {
            signal->s_blocksize = blocksize;
            signal->s_nchannels = nchannels;
        }
    }
    else if(signal->s_samples && size)
    {
        spam_signal_free(signal);
    }
}

void spam_signal_free(t_spam_signal* signal)
{
    const size_t size = signal->s_blocksize * signal->s_nchannels * sizeof(t_sample);
    if(signal->s_samples && size)
    {
        freebytes(signal->s_samples, size);
        signal->s_samples   = NULL;
        signal->s_blocksize = 0;
        signal->s_nchannels = 0;
    }
}

char spam_io_init(t_spam_io* io, int idx, char type, char sig, char stat)
{
    io->s_index     = idx;
    io->s_type      = type ? 1 : 0;
    io->s_signal    = sig  ? 1 : 0;
    io->s_static    = stat ? 1 : 0;
    io->s_symbol    = NULL;
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
            io->s_symbol = spam_gensym("$0-in-%i", io->s_index);
        }
        else
        {
            io->s_symbol = spam_gensym("$0-in-tilde-%i", io->s_index);
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
            io->s_symbol = spam_gensym("$0-out-%i", io->s_index);
        }
        else
        {
            io->s_symbol = spam_gensym("$0-out-tilde-%i", io->s_index);
        }
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

char spam_io_notify(t_spam_io* io)
{
    t_object* process = spam_get_process();
    if(process)
    {
        mess1((t_pd *)process, gensym("setio"), (void *)(io));
        return 0;
    }
    return -1;
}

t_canvas* spam_get_toplevel()
{
    t_canvas* cnv = canvas_getcurrent();
    while(cnv && cnv->gl_owner)
    {
        cnv = cnv->gl_owner;
    }
    return cnv;
}

t_symbol* spam_gensym(char* txt, ...)
{
    va_list ap;
    t_symbol* s = NULL;
    char temp[MAXPDSTRING];
    t_canvas* cnv = spam_get_toplevel();
    if(cnv)
    {
        va_start(ap, txt);
        vsnprintf(temp, MAXPDSTRING-1, txt, ap);
        va_end(ap);
        return canvas_realizedollar(cnv, gensym(temp));
    }    
    return s;
}

t_object* spam_get_process()
{
    t_symbol* s = NULL;
    t_canvas* cnv = spam_get_toplevel();
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





