/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.io.h"

static t_class* spam_in_tilde_class;

typedef struct _spam_in_tilde
{
    t_spam_io   s_io;
} t_spam_in_tilde;

static void spam_in_tilde_dsp(t_spam_in_tilde *x, t_signal **sp){
    spam_io_dsp((t_spam_io *)x);
    if(x->s_io.s_samples)
    {
        if(x->s_io.s_n != sp[0]->s_n)
        {
            pd_error(x, "spam~: can't use another block size in for the moment.");
        }
        else
        {
            dsp_add_copy(x->s_io.s_samples, sp[0]->s_vec, sp[0]->s_n);
        }
    }
    else
    {
        dsp_add_zero(sp[0]->s_vec, sp[0]->s_n);
    }
}

static void spam_in_tilde_free(t_spam_in_tilde *x){
    spam_io_free((t_spam_io *)x);
}


static void *spam_in_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    t_spam_in_tilde *x = (t_spam_in_tilde *)pd_new(spam_in_tilde_class);
    if(x)
    {
        if(spam_io_init((t_spam_io *)x, (int)atom_getfloatarg(0, argc, argv), 0, 1, (argc == 0)))
        {
            pd_free((t_pd *)x);
            return NULL;
        }
        if(!x->s_io.s_static && argc > 1)
        {
            post("spam.in~: shared mode, extra arguments ignored after index.");
        }
        else if(x->s_io.s_static && argc)
        {
            post("spam.in~: static mode all arguments ignored.");
        }
        outlet_new((t_object *)x, &s_signal);
    }
    return x;
}

extern void setup_spam0x2ein_tilde(void)
{
    t_class* c = class_new(gensym("spam.in~"), (t_newmethod)spam_in_tilde_new, (t_method)spam_in_tilde_free, sizeof(t_spam_in_tilde), CLASS_NOINLET, A_GIMME, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam"));
        class_addmethod((t_class *)c, (t_method)spam_in_tilde_dsp, gensym("dsp"), A_CANT, 0);
    }
    spam_in_tilde_class = c;
}



