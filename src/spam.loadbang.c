/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../pd/src/m_pd.h"

static t_class* spam_loadbang;

static void spam_loadbang_bang(t_object *x){
    outlet_bang(x->te_outlet);
}

static void *spam_loadbang_new()
{
    t_object *x  = (t_object *)pd_new(spam_loadbang);
    if(x)
    {
        outlet_new((t_object *)x, &s_bang);
    }
    return x;
}

extern void setup_spam0x2eloadbang(void)
{
    t_class* c = class_new(gensym("spam.loadbang"), (t_newmethod)spam_loadbang_new, (t_method)NULL, sizeof(t_object), CLASS_NOINLET, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam"));
        class_addmethod(c, (t_method)spam_loadbang_bang,  gensym("bang"), A_NULL, 0);
    }
    spam_loadbang = c;
}



