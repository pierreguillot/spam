/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"
#include "../pd/src/g_canvas.h"
#include <strings.h>
//#include "../pd/src/m_imp.h"
//

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

t_canvas* spam_get_toplevel()
{
    t_canvas* cnv = canvas_getcurrent();
    while(cnv && cnv->gl_owner)
    {
        cnv = cnv->gl_owner;
    }
    return cnv;
}





