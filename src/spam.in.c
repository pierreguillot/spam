/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.def.h"
#include <string.h>


static t_class* spam_in_class;
static t_class* spam_in_inlet_class;

static t_symbol* spam_symbol_owned;
static t_symbol* spam_symbol_borrowed;
static t_symbol* spam_symbol_shared;

typedef struct _spam_in_inlet
{
    t_class*    s_pd;
    t_object*   s_in;
} t_spam_in_inlet;

typedef struct _spam_in
{
    t_object        s_object;
    t_spam_in_inlet s_inlet;
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
    if(x->s_symbol) {
        pd_unbind((t_pd *)x, x->s_symbol);
    }
}

static unsigned int spam_in_get_tie()
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

static void spam_in_set_index(t_spam_in *x, float f)
{
    char temp[MAXPDSTRING];
    const unsigned long adress = spam_in_get_tie();
    if(x->s_symbol) {
         pd_unbind((t_pd *)x, x->s_symbol);
    }
    if(f > 0)
    {
        if(adress)
        {
            int check_if_borrowed_and_out_of_range;
            int notify_the_process;
            sprintf(temp, "%lx-%s-%s-%lx", adress, spam_string_in, x->s_type->s_name, (unsigned long)f);
            x->s_symbol = gensym(temp);
            pd_bind((t_pd *)x, x->s_symbol);
        }
    }
    else {
        pd_error(x, "spam.in index can't be negative.");
    }
}

static void spam_in_inlet_float(t_spam_in_inlet *x, float f){
    spam_in_set_index((t_spam_in *)x->s_in, f);
}

static void *spam_in_new(t_symbol* s, int argc, t_atom* argv)
{
    t_spam_in *x = (t_spam_in *)pd_new(spam_in_class);
    if(x)
    {
        x->s_index  = 0;
        x->s_symbol = NULL;
        x->s_type   = atom_getsymbolarg(0, argc, argv);
        if(x->s_type != spam_symbol_owned
           && x->s_type != spam_symbol_borrowed
           && x->s_type != spam_symbol_shared)
        {
            pd_error(x, "spam.in needs a least one symbol argument that must be owned, borrowed or shared.");
            return NULL;
        }
        if(argc > 1)
        {
            if(argv[1].a_type == A_FLOAT)
            {
                spam_in_set_index(x, atom_getfloat(argv+1));
            }
            else
            {
                pd_error(x, "spam.in second argument must be a float that defines the index.");
                return NULL;
            }
        }
        else
        {
            x->s_inlet.s_pd = spam_in_inlet_class;
            x->s_inlet.s_in = (t_object *)x;
            inlet_new((t_object *)x, &(x->s_inlet.s_pd), &s_float, &s_float);
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
        class_addmethod(c, (t_method)spam_in_bang,       gensym("bang"),        A_NULL,  0);
        class_addmethod(c, (t_method)spam_in_float,      gensym("float"),       A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_in_symbol,     gensym("symbol"),      A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_in_list,       gensym("list"),        A_GIMME, 0);
        class_addmethod(c, (t_method)spam_in_anything,   gensym("anything"),    A_GIMME, 0);
    }
    spam_in_class = c;
    
    
    c = class_new(gensym("spam.in.inlet"), NULL, NULL, sizeof(t_spam_in_inlet), CLASS_PD, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_in_inlet_float,gensym("float"),   A_FLOAT, 0);
    }
    spam_in_inlet_class = c;
}



