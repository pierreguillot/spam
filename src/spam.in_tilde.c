/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../pd/src/m_pd.h"
#include "../pd/src/g_canvas.h"

static t_class* spam_in_tilde_class;

typedef struct _spam_in_tilde
{
    t_object    s_obj;
    int         s_idx;
    int         s_static;
    t_symbol*   s_sym;
    t_sample*   s_samples;
} t_spam_in_tilde;

static t_int* spam_in_tilde_clear(t_int *w)
{
    t_sample *out = (t_sample *)(w[1]);
    int n = (int)(w[2]);
    while(n--)
    {
        *out++ = 0;
    }
    return (w+3);
}

static t_int* spam_in_tilde_clear8(t_int *w)
{
    t_sample *out = (t_sample *)(w[1]);
    int n = (int)(w[2]);
    while(n--)
    {
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        out[4] = 0;
        out[5] = 0;
        out[6] = 0;
        out[7] = 0;
    }
    return (w+3);
}

static t_int* spam_in_tilde_copy(t_int *w)
{
    t_sample *in  = (t_sample *)(w[1]);
    t_sample *out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    while(n--)
    {
        *out++ = *in++;
    }
    return (w+4);
}

static t_int* spam_in_tilde_copy8(t_int *w)
{
    t_sample *in  = (t_sample *)(w[1]);
    t_sample *out = (t_sample *)(w[2]);
    int n = (int)(w[3]);
    for (; n; n -= 8, in += 8, out += 8)
    {
        out[0] = in[0];
        out[1] = in[1];
        out[2] = in[2];
        out[3] = in[3];
        out[4] = in[4];
        out[5] = in[5];
        out[6] = in[6];
        out[7] = in[7];
    }
    return (w+4);
}

static void spam_in_tilde_dsp(t_spam_in_tilde *x, t_signal **sp)
{
    if(x->s_samples)
    {
        if(sp[0]->s_n&7)
        {
            dsp_add(spam_in_tilde_copy, 3, x->s_samples, sp[0]->s_vec, sp[0]->s_n);
        }
        else
        {
            dsp_add(spam_in_tilde_copy8, 3, x->s_samples, sp[0]->s_vec, sp[0]->s_n);
        }
    }
    else
    {
        if(sp[0]->s_n&7)
        {
            dsp_add(spam_in_tilde_clear, 2, sp[0]->s_vec, sp[0]->s_n);
        }
        else
        {
            dsp_add(spam_in_tilde_clear8, 2, sp[0]->s_vec, sp[0]->s_n);
        }
    }
}

static void spam_in_tilde_set(t_spam_in_tilde *x, t_sample* s)
{
    x->s_samples = s;
    post("get it");
}

static void spam_in_tilde_free(t_spam_in_tilde *x)
{
    if(x->s_sym)
    {
        pd_unbind((t_pd *)x, x->s_sym);
    }
}


static void *spam_in_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    char temp[MAXPDSTRING];
    t_canvas* cnv;
    t_symbol* sym;
    t_spam_in_tilde *x = (t_spam_in_tilde *)pd_new(spam_in_tilde_class);
    if(x)
    {
        x->s_sym        = NULL;
        x->s_samples    = NULL;
        if(argc && argv && argv[0].a_type == A_FLOAT)
        {
            x->s_idx    = atom_getfloatarg(0, argc, argv);
            x->s_static = 0;
            if(x->s_idx < 0)
            {
                pd_free((t_pd *)x);
                error("spam.in~: index must be superior or equal to zero.");
                return NULL;
            }
            if(argc > 1)
            {
                post("spam.in~: shared mode, extra arguments ignored after index.");
            }
        }
        else
        {
            x->s_static = 1;
            x->s_idx    = 0;
            if(argc)
            {
                post("spam.in~: static mode all arguments ignored.");
            }
        }
        outlet_new((t_object *)x, &s_signal);
        
        cnv = canvas_getcurrent();
        while(cnv && cnv->gl_owner)
        {
            cnv = cnv->gl_owner;
        }
        if(cnv)
        {
            sprintf(temp, "$0-in_tilde-%i", x->s_idx);
            x->s_sym = canvas_realizedollar(cnv, gensym(temp));
            pd_bind((t_pd *)x, canvas_realizedollar(cnv, x->s_sym));
            sym = canvas_realizedollar(cnv, gensym("$0-master"));
            if(sym && sym->s_thing)
            {
                mess4(sym->s_thing, gensym("setio"), (t_int)(0), ((t_int)x->s_idx), (t_int)(1), (t_int)(0));
            }
        }
    }
    return x;
}

extern void setup_spam0x2ein_tilde(void)
{
    t_class* c = class_new(gensym("spam.in~"), (t_newmethod)spam_in_tilde_new, (t_method)spam_in_tilde_free, sizeof(t_spam_in_tilde), CLASS_NOINLET, A_GIMME, 0);
    class_addcreator((t_newmethod)spam_in_tilde_new, gensym("spam.in~"), A_NULL, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam.io"));
        class_addmethod((t_class *)c, (t_method)spam_in_tilde_dsp, gensym("dsp"), A_CANT, 0);
        class_addmethod((t_class *)c, (t_method)spam_in_tilde_set, gensym("set"), A_CANT, 0);
    }
    spam_in_tilde_class = c;
}



