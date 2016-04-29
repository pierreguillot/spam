/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"

static t_class* spam_out_tilde_class;

typedef struct _spam_out_tilde
{
    t_spam_io   s_io;
    t_sample*   s_samples;
} t_spam_out_tilde;

static void spam_out_tilde_dsp(t_spam_out_tilde *x, t_signal **sp){
    if(x->s_samples){
        dsp_add_plus(x->s_samples, sp[0]->s_vec, x->s_samples, sp[0]->s_n);
    }
    else{
        dsp_add_zero(sp[0]->s_vec, sp[0]->s_n);
    }
}

static void spam_out_tilde_free(t_spam_out_tilde *x){
    spam_io_free((t_spam_io *)x);
}

static void *spam_out_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    t_spam_out_tilde *x = (t_spam_out_tilde *)pd_new(spam_out_tilde_class);
    if(x)
    {
        x->s_samples    = NULL;
        if(spam_io_init((t_spam_io *)x, atom_getfloatarg(0, argc, argv), 1, 1, (argc == 0)))
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
        spam_io_notify((t_spam_io *)x);
    }
    return x;
}

extern void setup_spam0x2eout_tilde(void)
{
    t_class* c = class_new(gensym("spam.out~"), (t_newmethod)spam_out_tilde_new, (t_method)spam_out_tilde_free, sizeof(t_spam_out_tilde), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam.io"));
        class_addmethod((t_class *)c, (t_method)spam_out_tilde_dsp, gensym("dsp"), A_CANT, 0);
    }
    spam_out_tilde_class = c;
}



