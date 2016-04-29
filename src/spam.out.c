/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.io.h"

static t_class* spam_out_class;

typedef t_spam_io t_spam_out;

static void spam_out_bang(t_spam_out *x){
    if(x->s_symbol && x->s_symbol->s_thing){
        pd_bang(x->s_symbol->s_thing);
    }
}

static void spam_out_float(t_spam_out *x, float f){
    if(x->s_symbol && x->s_symbol->s_thing){
        pd_float(x->s_symbol->s_thing, f);
    }
}

static void spam_out_symbol(t_spam_out *x, t_symbol* s){
    if(x->s_symbol && x->s_symbol->s_thing){
        pd_symbol(x->s_symbol->s_thing, s);
    }
}

static void spam_out_list(t_spam_out *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_symbol && x->s_symbol->s_thing){
        pd_list(x->s_symbol->s_thing, s, argc, argv);
    }
}

static void spam_out_anything(t_spam_out *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_symbol && x->s_symbol->s_thing){
        pd_typedmess(x->s_symbol->s_thing, s, argc, argv);
    }
}

static void spam_out_free(t_spam_out *x) {
    spam_io_free((t_spam_io *)x);
}

static void *spam_out_new(t_float idx)
{
    t_spam_out *x = (t_spam_out *)pd_new(spam_out_class);
    if(x)
    {
        if(spam_io_init((t_spam_io *)x, idx, 1, 0, 0))
        {
            pd_free((t_pd *)x);
            return NULL;
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



