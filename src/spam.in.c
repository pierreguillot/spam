/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"

static t_class* spam_in_class;

typedef struct _spam_in
{
    t_spam_io   s_io;
    t_outlet*   s_out;
} t_spam_in;

static void spam_in_bang(t_spam_in *x){
    outlet_bang(x->s_out);
}

static void spam_in_float(t_spam_in *x, float f){
    outlet_float(x->s_out, f);
}

static void spam_in_symbol(t_spam_in *x, t_symbol* s){
    outlet_symbol(x->s_out, s);
}

static void spam_in_list(t_spam_in *x, t_symbol* s, int argc, t_atom* argv){
    outlet_list(x->s_out, s, argc, argv);
}

static void spam_in_anything(t_spam_in *x, t_symbol* s, int argc, t_atom* argv){
    outlet_anything(x->s_out, s, argc, argv);
}

static void spam_in_free(t_spam_in *x){
    spam_io_free((t_spam_io *)x);
}

static void *spam_in_new(t_float f)
{
    t_spam_in *x  = (t_spam_in *)pd_new(spam_in_class);
    if(x)
    {
        if(spam_io_init((t_spam_io *)x, (int)f, 0, 0, 0))
        {
            pd_free((t_pd *)x);
            return NULL;
        }
        x->s_out = outlet_new((t_object *)x, NULL);
        spam_io_notify((t_spam_io *)x);
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



