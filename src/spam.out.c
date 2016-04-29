/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../pd/src/m_pd.h"
#include "../pd/src/g_canvas.h"

static t_class* spam_out_class;

typedef struct _spam_out
{
    t_object    s_obj;
    int         s_idx;
    t_symbol*   s_sym;
} t_spam_out;

static void spam_out_bang(t_spam_out *x)
{
    if(x->s_sym && x->s_sym->s_thing)
    {
        pd_bang(x->s_sym->s_thing);
    }
}

static void spam_out_float(t_spam_out *x, float f)
{
    if(x->s_sym && x->s_sym->s_thing)
    {
        pd_float(x->s_sym->s_thing, f);
    }
}

static void spam_out_symbol(t_spam_out *x, t_symbol* s)
{
    if(x->s_sym && x->s_sym->s_thing)
    {
        pd_symbol(x->s_sym->s_thing, s);
    }
}

static void spam_out_list(t_spam_out *x, t_symbol* s, int argc, t_atom* argv)
{
    if(x->s_sym && x->s_sym->s_thing)
    {
        pd_list(x->s_sym->s_thing, s, argc, argv);
    }
}

static void spam_out_anything(t_spam_out *x, t_symbol* s, int argc, t_atom* argv)
{
    if(x->s_sym && x->s_sym->s_thing)
    {
        pd_typedmess(x->s_sym->s_thing, s, argc, argv);
    }
}

static void spam_out_free(t_spam_out *x)
{
    ;
}

static void *spam_out_new(t_float idx)
{
    char temp[MAXPDSTRING];
    t_canvas* cnv;
    t_symbol* sym;
    t_spam_out *x = (t_spam_out *)pd_new(spam_out_class);
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
            sprintf(temp, "$0-out-%i", x->s_idx);
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

extern void setup_spam0x2eout(void)
{
    t_class* c = class_new(gensym("spam.out"), (t_newmethod)spam_out_new, (t_method)spam_out_free, sizeof(t_spam_out), CLASS_DEFAULT, A_FLOAT, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam.io"));
        class_addmethod(c, (t_method)spam_out_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_out_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_out_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_out_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_out_anything,   gensym("anything"),A_GIMME, 0);
    }
    spam_out_class = c;
}



