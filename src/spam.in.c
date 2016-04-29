/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../pd/src/m_pd.h"
#include "../pd/src/g_canvas.h"

static t_class* spam_in_class;

typedef struct _spam_in
{
    t_object    s_obj;
    int         s_idx;
    t_symbol*   s_sym;
    t_outlet*   s_out;
} t_spam_in;

static void spam_in_bang(t_spam_in *x)
{
    outlet_bang(x->s_out);
}

static void spam_in_float(t_spam_in *x, float f)
{
    outlet_float(x->s_out, f);
}

static void spam_in_symbol(t_spam_in *x, t_symbol* s)
{
    outlet_symbol(x->s_out, s);
}

static void spam_in_list(t_spam_in *x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_list(x->s_out, s, argc, argv);
}

static void spam_in_anything(t_spam_in *x, t_symbol* s, int argc, t_atom* argv)
{
    outlet_anything(x->s_out, s, argc, argv);
}

static void spam_in_free(t_spam_in *x)
{
    if(x->s_sym)
    {
        pd_unbind((t_pd *)x, x->s_sym);
    }
}

static void *spam_in_new(t_float f)
{
    char temp[MAXPDSTRING];
    t_canvas* cnv;
    t_symbol* sym;
    t_spam_in *x = (t_spam_in *)pd_new(spam_in_class);
    if(x)
    {
        x->s_sym = NULL;
        x->s_idx = f;
        x->s_out = outlet_new((t_object *)x, NULL);
        if(x->s_idx < 0)
        {
            pd_free((t_pd *)x);
            error("spam.in: index must be superior or equal to zero.");
            return NULL;
        }
        cnv = canvas_getcurrent();
        while(cnv && cnv->gl_owner)
        {
            cnv = cnv->gl_owner;
        }
        if(cnv)
        {
            sprintf(temp, "$0-in-%i", x->s_idx);
            x->s_sym = canvas_realizedollar(cnv, gensym(temp));
            pd_bind((t_pd *)x, canvas_realizedollar(cnv, x->s_sym));
            sym = canvas_realizedollar(cnv, gensym("$0-master"));
            if(sym && sym->s_thing)
            {
                mess4(sym->s_thing, gensym("setio"), (t_int)(0), ((t_int)x->s_idx), (t_int)(0), (t_int)(0));
            }
        }
    }
    return x;
}

extern void setup_spam0x2ein(void)
{
    t_class* c = class_new(gensym("spam.in"), (t_newmethod)spam_in_new, (t_method)spam_in_free, sizeof(t_spam_in), CLASS_NOINLET, A_FLOAT, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam.io"));
        class_addmethod(c, (t_method)spam_in_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_in_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_in_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_in_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_in_anything,   gensym("anything"),A_GIMME, 0);
    }
    spam_in_class = c;
}



