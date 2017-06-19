/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.def.h"
#include <string.h>

static t_class* spam_in_tilde_class;
static t_class* spam_in_tilde_inlet_class;

static t_symbol* spam_symbol_owned;
static t_symbol* spam_symbol_borrowed;
static t_symbol* spam_symbol_shared;

typedef struct _spam_in_tilde_inlet
{
    t_class*    s_pd;
    t_object*   s_in;
} t_spam_in_tilde_inlet;

typedef struct _spam_in_tilde
{
    t_object                s_object;
    t_spam_in_tilde_inlet   s_inlet;
    unsigned int            s_index;
    t_sample*               s_samples;
    int                     s_n;
} t_spam_in_tilde;

static void spam_in_tilde_dsp(t_spam_in_tilde *x, t_signal **sp){
    int get_process_signal;
    if(x->s_samples)
    {
        if(x->s_n != sp[0]->s_n) {
            pd_error(x, "spam.in~: can't use another block size in for the moment.");
        }
        else {
            dsp_add_copy(x->s_samples, sp[0]->s_vec, sp[0]->s_n);
        }
    }
    else {
        dsp_add_zero(sp[0]->s_vec, sp[0]->s_n);
    }
}

static unsigned int spam_in_tilde_get_tie()
{
    t_gobj* y = NULL;
    t_glist* gl = canvas_getcurrent();
    while(gl)
    {
        for(y = gl->gl_list; y; y = y->g_next)
        {
            if(!strncmp((const char *)class_getname(y->g_pd), spam_string_spam_tie, sizeof(spam_string_spam_tie) - 1))
            {
                return (unsigned long)y;
            }
        }
        gl = gl->gl_owner;
    }
    return 0;
}

static void spam_in_tilde_set_index(t_spam_in_tilde *x, float f)
{
     {
        pd_error(x, "spam.in index can't be negative.");
    }
}

static void spam_in_tilde_inlet_float(t_spam_in_tilde_inlet *x, float f){
    spam_in_tilde_set_index((t_spam_in_tilde *)x->s_in, f);
}

static void *spam_in_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    t_spam_in_tilde *x = (t_spam_in_tilde *)pd_new(spam_in_tilde_class);
    if(x)
    {
        outlet_new((t_object *)x, &s_signal);
    }
    return x;
}

extern void setup_spam0x2ein_tilde(void)
{
    spam_symbol_owned       = gensym(spam_string_owned);
    spam_symbol_borrowed    = gensym(spam_string_borrowed);
    spam_symbol_shared      = gensym(spam_string_shared);
    
    t_class* c = class_new(gensym("spam.in~"), (t_newmethod)spam_in_tilde_new, (t_method)NULL,
                           sizeof(t_spam_in_tilde), CLASS_NOINLET, A_GIMME, 0);
    if(c)
    {
        class_sethelpsymbol((t_class *)c, gensym("spam"));
        class_addmethod(c, (t_method)spam_in_tilde_dsp,       gensym("dsp"),        A_CANT,  0);
    }
    spam_in_tilde_class = c;
    
    
    c = class_new(gensym("spam.in~.inlet"), NULL, NULL, sizeof(t_spam_in_tilde_inlet), CLASS_PD, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_in_tilde_inlet_float,gensym("float"),   A_FLOAT, 0);
    }
    spam_in_tilde_inlet_class = c;
}



