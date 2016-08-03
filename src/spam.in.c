/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.def.h"
#include <string.h>


static t_class* spam_in_class;
static t_symbol* spam_symbol_owned;
static t_symbol* spam_symbol_borrowed;
static t_symbol* spam_symbol_shared;

typedef struct _spam_in
{
    t_object        s_object;
    t_symbol*       s_type;
    unsigned int    s_index;
    t_symbol*       s_symbol;
} t_spam_in;

static void spam_in_bang(t_spam_in *x){
    outlet_bang(x->s_object.te_outlet);
}

static void spam_in_float(t_spam_in *x, float f){
    outlet_float(x->s_object.te_outlet, f);
}

static void spam_in_symbol(t_spam_in *x, t_symbol* s){
    outlet_symbol(x->s_object.te_outlet, s);
}

static void spam_in_list(t_spam_in *x, t_symbol* s, int argc, t_atom* argv){
    outlet_list(x->s_object.te_outlet, s, argc, argv);
}

static void spam_in_anything(t_spam_in *x, t_symbol* s, int argc, t_atom* argv){
    outlet_anything(x->s_object.te_outlet, s, argc, argv);
}

static void spam_in_free(t_spam_in *x){
    if(x->s_symbol)
    {
        pd_unbind((t_pd *)x, x->s_symbol);
    }
}

static t_object* spam_in_get_tie()
{
    char temp[MAXPDSTRING];
    t_gobj* y = NULL;
    t_glist* gl = canvas_getcurrent();
    while(gl)
    {
        for(y = gl->gl_list; y; y = y->g_next)
        {
            if(!strncmp((const char *)class_getname(y->g_pd), spam_string_spam_tie, sizeof(spam_string_spam_tie) - 1))
            {
                return (t_object*)y;
            }
        }
        gl = gl->gl_owner;
    }
    return NULL;
}

static void *spam_in_new(t_symbol* s, int argc, t_atom* argv)
{
    t_spam_in *x = NULL;
    t_object  *t = NULL;
    if(argc && argv)
    {
        
    }
    t_symbol  *m = atom_getsymbolarg(0, argc, argv);
    if(m != spam_symbol_owned && m != spam_symbol_borrowed && m != spam_symbol_shared)
    {
        error("spam.in must be owned, borrowed or shared.");
        return NULL;
    }
    if(argc > 1)
    {
        
    }
    /*
    if(f < 0)
    {
        error("spam.in must have a positive index.");
        return NULL;
    }
    */
    x  = (t_spam_in *)pd_new(spam_in_class);
    if(x)
    {
        t           = spam_in_get_tie();
        x->s_type   = s;
        //x->s_index  = (unsigned int)f;
        if(t)
        {
            //x->s_symbol = spam_tie_gensym(x->s_tie, spam_string_in, x->s_index);
            if(x->s_symbol)
            {
                pd_bind((t_pd *)x, x->s_symbol);
            }
        }
        outlet_new((t_object *)x, NULL);
    }
    return x;
}

extern void setup_spam0x2ein(void)
{
    spam_symbol_owned       = gensym(spam_string_owned);
    spam_symbol_borrowed    = gensym(spam_string_borrowed);
    spam_symbol_shared      = gensym(spam_string_shared);
    
    t_class* c = class_new(gensym("spam.in"), (t_newmethod)spam_in_new, (t_method)spam_in_free,
                           sizeof(t_spam_in), CLASS_NOINLET, A_GIMME, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam"));
        class_addmethod(c, (t_method)spam_in_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_in_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_in_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_in_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_in_anything,   gensym("anything"),A_GIMME, 0);
    }
    spam_in_class = c;
}



