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
    t_spam_signal   s_signal;
    t_spam_iolets   s_iolet;
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
    spam_iolets_free(&(x->s_iolet));
    spam_signal_free(&(x->s_signal));
    spam_master_free(&(x->s_master));
}

static void spam_process_vis(t_spam *x, t_floatarg index)
{
    if(x->s_master.s_canvas)
    {
        //canvas_vis(cnv, vis);
    }
}

static void spam_process_click(t_spam *x, t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    spam_process_vis(x, 0);
}

static void spam_process_setio(t_spam *x, t_spam_io* io){
    spam_iolets_set(&(x->s_iolet), io);
}

static void spam_process_getsample(t_spam *x, t_spam_io* io){
    spam_iolets_set_samples(&(x->s_iolet), io, &(x->s_signal), x->s_master.s_n);
}

t_int *spam_perform(t_int *w)
{
    t_sample *in1 = (t_sample *)(w[1]);
    t_sample *in2 = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    int n = (int)(w[4]);
    while (n--) *out++ = *in1++ + *in2++;
    return (w+5);
}


static void spam_process_dsp(t_spam *x, t_signal **sp)
{
    post("spam_process_dsp");
    /*
    int i, j, n;
    t_sample* v;
    t_symbol* s;
    t_atom* av;
    char temp[MAXPDSTRING];
     */
    int nins    = spam_iolets_get_ninsig(&(x->s_iolet), x->s_master.s_n);
    int nouts   = spam_iolets_get_noutsig(&(x->s_iolet), x->s_master.s_n);
    int max     = (nins > nouts) ? nins : nouts;
    if(!spam_signal_alloc(&(x->s_signal), max, (sp && sp[0]) ? sp[0]->s_n : 0))
    {
        mess0((t_pd *)x->s_master.s_canvas, gensym("dsp"));
        /*
        j = 0;
        if(x->s_instatic)
        {
            for(j = 0; j < x->s_nrows * x->s_ncolumns; ++j)
            {
                sprintf(temp, "$0-in_static-%i", j);
                s = canvas_realizedollar(x->s_master.s_canvas, gensym(temp));
                if(s && s->s_thing)
                {
                    mess3(s->s_thing, gensym("set"), x->s_samples+j*sp[0]->s_n, 0, 0);
                }
            }
        }
        for(i = 0; i < x->s_ninssig; ++i, ++j)
        {
            sprintf(temp, "$0-in_tilde-%i", i);
            s = canvas_realizedollar(x->s_master.s_canvas, gensym(temp));
            if(s && s->s_thing)
            {
                mess3(s->s_thing, gensym("set"), x->s_samples+j*sp[0]->s_n, 0, 0);
            }
        }
         */
    }
}

static void *spam_process_new(t_symbol *s, int argc, t_atom *argv)
{
    t_spam *x  = (t_spam *)pd_new(spam_process_class);
    if(x)
    {
        spam_signal_init(&(x->s_signal));
        spam_iolets_init(&(x->s_iolet));
        if(spam_master_init((t_spam_master *)x,
                            atom_getsymbolarg(1, argc, argv),
                            atom_getfloatarg(0, argc, argv),
                            argc-2, argv+2))
        {
            pd_free((t_pd *)x);
            return NULL;
        }
        
        spam_iolets_newio(&(x->s_iolet), (t_object *)x, x->s_master.s_canvas, x->s_master.s_n);
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
        class_addmethod(c, (t_method)spam_process_setio,    gensym("setio"), A_CANT, 0);
        class_addmethod(c, (t_method)spam_process_getsample,gensym("getsamples"), A_CANT, 0);
        
        /*
        class_addmethod(c, (t_method)spam_process_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_process_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_process_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_process_anything,   gensym("anything"),A_GIMME, 0);
         */
    }
    spam_process_class = c;
    setup_iolet_setup();
}



