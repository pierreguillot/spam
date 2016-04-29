/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../pd/src/m_pd.h"
#include "../pd/src/g_canvas.h"

static t_class* spam_out_tilde_class;

typedef struct _spam_out_tilde
{
    t_object    s_obj;
    int         s_idx;
    t_symbol*   s_sym;
} t_spam_out_tilde;

static void spam_out_tilde_free(t_spam_out_tilde *x)
{
    ;
}

static void *spam_out_tilde_new(t_float idx)
{
    char temp[MAXPDSTRING];
    t_canvas* cnv;
    t_symbol* sym;
    t_spam_out_tilde *x = (t_spam_out_tilde *)pd_new(spam_out_tilde_class);
    if(x)
    {
        x->s_idx = idx;
        if(x->s_idx < 0)
        {
            pd_free((t_pd *)x);
            error("spam.out: index must be superior or equal to zero.");
            return NULL;
        }
        cnv = canvas_getcurrent();
        while(cnv && cnv->gl_owner)
        {
            cnv = cnv->gl_owner;
        }
        if(cnv)
        {
            sprintf(temp, "$0-out_tilde-%i", x->s_idx);
            x->s_sym = canvas_realizedollar(cnv, gensym(temp));
            sym = canvas_realizedollar(cnv, gensym("$0-master"));
            if(sym && sym->s_thing)
            {
                mess4(sym->s_thing, gensym("setio"), (t_int)(1), ((t_int)x->s_idx), (t_int)(0), (t_int)(0));
            }
        }
    }
    return x;
}

extern void setup_spam0x2eout_tilde(void)
{
    t_class* c = class_new(gensym("spam.out"), (t_newmethod)spam_out_tilde_new, (t_method)spam_out_tilde_free, sizeof(t_spam_out_tilde), CLASS_DEFAULT, A_FLOAT, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam.io"));
    }
    spam_out_tilde_class = c;
}



