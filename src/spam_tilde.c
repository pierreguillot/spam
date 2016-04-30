/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"

static t_class* spam_tilde_class;

typedef struct _spam_tilde
{
    t_spam_master   s_master;
    float           s_f;
} t_spam_tilde;



static void spam_tilde_bang(t_spam_tilde *x){
    if(x->s_master.s_fin && x->s_master.s_fin->s_thing){
        pd_bang(x->s_master.s_fin->s_thing);
    }
}

static void spam_tilde_float(t_spam_tilde *x, float f){
    if(x->s_master.s_fin && x->s_master.s_fin->s_thing){
        pd_float(x->s_master.s_fin->s_thing, f);
    }
}

static void spam_tilde_symbol(t_spam_tilde *x, t_symbol* s){
    if(x->s_master.s_fin && x->s_master.s_fin->s_thing){
        pd_symbol(x->s_master.s_fin->s_thing, s);
    }
}

static void spam_tilde_list(t_spam_tilde *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_master.s_fin && x->s_master.s_fin->s_thing){
        pd_list(x->s_master.s_fin->s_thing, s, argc, argv);
    }
}

static void spam_tilde_anything(t_spam_tilde *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_master.s_fin && x->s_master.s_fin->s_thing){
        pd_typedmess(x->s_master.s_fin->s_thing, s, argc, argv);
    }
}







static void spam_tilde_free(t_spam_tilde *x)
{
    spam_master_free(&(x->s_master));
}

static void spam_tilde_vis(t_spam_tilde *x, t_floatarg index)
{
    if(spam_master_visible((t_spam_master *)x, index))
    {
        error("spam.process~: index out of range!");
    }
}

static void spam_tilde_click(t_spam_tilde *x, t_floatarg xp, t_floatarg yp, t_floatarg s, t_floatarg c, t_floatarg a)
{
    spam_tilde_vis(x, 0);
}

static void spam_tilde_io_init(t_spam_tilde *x, t_spam_io* io){
    spam_master_io_init((t_spam_master *)x, io);
}

static void spam_tilde_io_dsp(t_spam_tilde *x, t_spam_io* io){
    spam_master_io_dsp((t_spam_master *)x, io);
}



static void spam_tilde_dsp(t_spam_tilde *x, t_signal **sp)
{
    if(spam_master_dsp((t_spam_master *)x, sp))
    {
        pd_error(x, "spam.process~: can't perform dsp.");
    }
}

static void *spam_tilde_new(t_symbol *s, int argc, t_atom *argv)
{
    t_spam_tilde *x  = (t_spam_tilde *)pd_new(spam_tilde_class);
    if(x)
    {
        if(spam_master_init((t_spam_master *)x,
                            atom_getsymbolarg(1, argc, argv),
                            atom_getfloatarg(0, argc, argv),
                            argc-2, argv+2))
        {
            pd_free((t_pd *)x);
            return NULL;
        }
    }
    return x;
}

extern void spam_tilde_setup(void)
{
    t_class *c = class_new(gensym("spam~"), (t_newmethod)spam_tilde_new, (t_method)spam_tilde_free, sizeof(t_spam_tilde), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_tilde_click,    gensym("click"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_tilde_vis,      gensym("vis"), A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_tilde_dsp,      gensym("dsp"), A_CANT, 0);
        class_addmethod(c, (t_method)spam_tilde_io_init,  gensym(__spam_io_init__), A_CANT, 0);
        class_addmethod(c, (t_method)spam_tilde_io_dsp,   gensym(__spam_io_dsp__), A_CANT, 0);
        CLASS_MAINSIGNALIN(c, t_spam_tilde, s_f);
        class_sethelpsymbol((t_class *)c, gensym("spam"));
        
        class_addmethod(c, (t_method)spam_tilde_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_tilde_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_tilde_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_tilde_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_tilde_anything,   gensym("anything"),A_GIMME, 0);
    }
    spam_tilde_class = c;
    spam_master_setup();
}



