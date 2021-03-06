/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.io.h"

static t_class* spam_out_tilde_class;

typedef struct _spam_out_tilde
{
    t_spam_io   s_io;
    t_float     s_f;
} t_spam_out_tilde;

static void spam_out_tilde_dsp(t_spam_out_tilde *x, t_signal **sp){
    spam_io_dsp((t_spam_io *)x);
    if(x->s_io.s_samples)
    {
        if(x->s_io.s_n != sp[0]->s_n)
        {
            pd_error(x, "spam~: can't use another block size in for the moment.");
        }
        else
        {
            dsp_add_plus(x->s_io.s_samples, sp[0]->s_vec, x->s_io.s_samples, sp[0]->s_n);
        }
    }
}

static void *spam_out_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    t_spam_out_tilde *x = (t_spam_out_tilde *)pd_new(spam_out_tilde_class);
    if(x)
    {
        if(spam_io_init((t_spam_io *)x, (int)(atom_getfloatarg(0, argc, argv)), 1, 1, (argc == 0)))
        {
            pd_free((t_pd *)x);
            return NULL;
        }
        if(!x->s_io.s_static && argc > 1)
        {
            post("spam.out~: shared mode, extra arguments ignored after index.");
        }
        else if(x->s_io.s_static && argc)
        {
            post("spam.out~: static mode all arguments ignored.");
        }
    }
    return x;
}

extern void setup_spam0x2eout_tilde(void)
{
    t_class* c = class_new(gensym("spam.out~"), (t_newmethod)spam_out_tilde_new, (t_method)spam_io_free, sizeof(t_spam_out_tilde), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        CLASS_MAINSIGNALIN((t_class *)c, t_spam_out_tilde, s_f);
        class_sethelpsymbol((t_class *)c, gensym("spam"));
        class_addmethod((t_class *)c, (t_method)spam_out_tilde_dsp, gensym("dsp"), A_CANT, 0);
    }
    spam_out_tilde_class = c;
}



