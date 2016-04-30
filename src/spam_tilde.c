/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"

static t_class* spam_process_class;

typedef struct _spam
{
    t_spam_master   s_master;
    t_sample**      s_inputs;
    t_sample**      s_outputs;
    //t_symbol*       s_fin;
} t_spam;


/*
static void spam_process_bang(t_spam *x){
    if(x->s_fin && x->s_fin->s_thing){
        pd_bang(x->s_fin->s_thing);
    }
}

static void spam_process_float(t_spam *x, float f){
    if(x->s_fin && x->s_fin->s_thing){
        pd_float(x->s_fin->s_thing, f);
    }
}

static void spam_process_symbol(t_spam *x, t_symbol* s){
    if(x->s_fin && x->s_fin->s_thing){
        pd_symbol(x->s_fin->s_thing, s);
    }
}

static void spam_process_list(t_spam *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_fin && x->s_fin->s_thing){
        pd_list(x->s_fin->s_thing, s, argc, argv);
    }
}

static void spam_process_anything(t_spam *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_fin && x->s_fin->s_thing){
        pd_typedmess(x->s_fin->s_thing, s, argc, argv);
    }
}
*/






static void spam_process_free(t_spam *x)
{
    int nins    = spam_master_get_nsignals((t_spam_master *)x, 0);
    int nouts   = spam_master_get_nsignals((t_spam_master *)x, 1);
    if(x->s_inputs)
    {
        freebytes(x->s_inputs, nins * sizeof(t_sample *));
    }
    if(x->s_outputs)
    {
        freebytes(x->s_inputs, nouts * sizeof(t_sample *));
    }
    spam_master_free(&(x->s_master));
}

static void spam_process_vis(t_spam *x, t_floatarg index)
{
    if(spam_master_visible((t_spam_master *)x, index))
    {
        error("spam.process~: index out of range!");
    }
}

static void spam_process_click(t_spam *x, t_floatarg xp, t_floatarg yp, t_floatarg s, t_floatarg c, t_floatarg a)
{
    spam_process_vis(x, 0);
}

static void spam_process_io_init(t_spam *x, t_spam_io* io){
    spam_master_io_init((t_spam_master *)x, io);
}

static void spam_process_io_dsp(t_spam *x, t_spam_io* io){
    spam_master_io_dsp((t_spam_master *)x, io);
}

t_int *spam_process_perform(t_int *w)
{
    int i;
    int n         = (int)(w[1]);
    int nins      = (int)(w[2]);
    int outs      = (int)(w[3]);
    t_sample**in  = (t_sample *)(w[4]);
    t_sample**out = (t_sample *)(w[5]);
    for(i = 0; i < nins; ++i)
    {
        
    }
    
    
    return (w+6);
}


static void spam_process_dsp(t_spam *x, t_signal **sp)
{
    int i;
    t_int* vec;
    int nins    = spam_master_get_nsignals((t_spam_master *)x, 0);
    int nouts   = spam_master_get_nsignals((t_spam_master *)x, 1);
    if(nins && nouts)
    {
        if(!x->s_inputs)
        {
            x->s_inputs  = (t_sample **)getbytes(nins * sizeof(t_sample *));
        }
        if(!x->s_outputs)
        {
           x->s_outputs = (t_sample **)getbytes(nouts * sizeof(t_sample *));
        }
        if(x->s_inputs && x->s_outputs)
        {
            dsp_add(spam_process_perform, 5, sp[0]->s_n, nins, nouts, x->s_inputs, x->s_outputs);
            spam_master_dsp((t_spam_master *)x, sp);
        }
    }
}

static void *spam_process_new(t_symbol *s, int argc, t_atom *argv)
{
    t_spam *x  = (t_spam *)pd_new(spam_process_class);
    if(x)
    {
        x->s_inputs     = NULL;
        x->s_outputs    = NULL;
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

extern void setup_spam0x2eprocess_tilde(void)
{
    t_class *c = class_new(gensym("spam.process~"), (t_newmethod)spam_process_new, (t_method)spam_process_free, sizeof(t_spam), CLASS_NOINLET, A_GIMME, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_process_click,    gensym("click"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_vis,      gensym("vis"), A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_dsp,      gensym("dsp"), A_CANT, 0);
        class_addmethod(c, (t_method)spam_process_io_init,  gensym("ioinit"), A_CANT, 0);
        class_addmethod(c, (t_method)spam_process_io_dsp,   gensym("iodsp"), A_CANT, 0);
        
        /*
        class_addmethod(c, (t_method)spam_process_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_process_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_process_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_process_anything,   gensym("anything"),A_GIMME, 0);
         */
    }
    spam_process_class = c;
    spam_master_setup();
}



