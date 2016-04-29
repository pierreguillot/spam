/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"
#include "../pd/src/g_canvas.h"
#include <strings.h>

static t_class* spam_process_class;
static t_class* spam_process_inlet_class;
static t_class* spam_process_outlet_class;

typedef struct _spam_inlet
{
    t_class*    s_pd;
    t_symbol*   s_sym;
} t_spam_inlet;

static void spam_process_inlet_bang(t_spam_inlet *x){
    if(x->s_sym && x->s_sym->s_thing){
        pd_bang(x->s_sym->s_thing);
    }
}

static void spam_process_inlet_float(t_spam_inlet *x, float f){
    if(x->s_sym && x->s_sym->s_thing){
        pd_float(x->s_sym->s_thing, f);
    }
}

static void spam_process_inlet_symbol(t_spam_inlet *x, t_symbol* s){
    if(x->s_sym && x->s_sym->s_thing){
        pd_symbol(x->s_sym->s_thing, s);
    }
}

static void spam_process_inlet_list(t_spam_inlet *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_sym && x->s_sym->s_thing){
        pd_list(x->s_sym->s_thing, s, argc, argv);
    }
}

static void spam_process_inlet_anything(t_spam_inlet *x, t_symbol* s, int argc, t_atom* argv){
    if(x->s_sym && x->s_sym->s_thing){
        pd_typedmess(x->s_sym->s_thing, s, argc, argv);
    }
}

typedef struct _spam_outlet
{
    t_class*    s_pd;
    t_outlet*   s_outlet;
    t_symbol*   s_sym;
} t_spam_outlet;

static void spam_process_outlet_bang(t_spam_outlet *x){
    outlet_bang(x->s_outlet);
}

static void spam_process_outlet_float(t_spam_outlet *x, float f){
    outlet_float(x->s_outlet, f);
}

static void spam_process_outlet_symbol(t_spam_outlet *x, t_symbol* s){
    outlet_symbol(x->s_outlet, s);
}

static void spam_process_outlet_list(t_spam_outlet *x, t_symbol* s, int argc, t_atom* argv){
    outlet_list(x->s_outlet, s, argc, argv);
}

static void spam_process_outlet_anything(t_spam_outlet *x, t_symbol* s, int argc, t_atom* argv){
    outlet_anything(x->s_outlet, s, argc, argv);
}




typedef struct _spam_iolets
{
    int             staticins;
    int             nins_message;
    int             nins_signal;
    int             staticout;
    int             nout_message;
    int             nouts_signal;
    t_spam_inlet*   ins;
    t_spam_outlet*  outs;
} t_spam_iolets;

static void spam_iolets_init(t_spam_iolets* iolets)
{
    iolets->staticins   = 0;
    iolets->nins_message= 1;
    iolets->nins_signal = 0;
    iolets->staticout   = 0;
    iolets->nout_message= 0;
    iolets->nouts_signal= 0;
    iolets->ins         = NULL;
    iolets->outs        = NULL;
}

static void spam_iolets_free(t_spam_iolets* iolets)
{
    int i;
    if(iolets->nins_message && iolets->ins)
    {
        freebytes(iolets->ins, iolets->nins_message * sizeof(t_spam_inlet));
    }
    if(iolets->nout_message && iolets->outs)
    {
        for(i = 0; i < iolets->nout_message; ++i)
        {
            pd_unbind((t_pd *)(iolets->outs+i), iolets->outs[i].s_sym);
        }
        freebytes(iolets->outs, iolets->nout_message * sizeof(t_spam_outlet));
    }
    iolets->staticins   = 0;
    iolets->nins_message= 0;
    iolets->nins_signal = 0;
    iolets->staticout   = 0;
    iolets->nout_message= 0;
    iolets->nouts_signal = 0;
    iolets->ins         = NULL;
    iolets->outs        = NULL;
}

static void spam_iolets_set(t_spam_iolets* iolets, t_spam_io* io)
{
    if(!io->s_type){
        if(!io->s_signal){
            iolets->nins_message = (iolets->nins_message > io->s_index+1) ? iolets->nins_message : io->s_index+1;
        }
        else if(io->s_static){
            iolets->staticins = 1;
        }
        else{
            iolets->nins_signal = (iolets->nins_signal > io->s_index+1) ? iolets->nins_signal : io->s_index+1;
        }
    }
    else{
        if(!io->s_signal){
            iolets->nout_message = (iolets->nout_message > io->s_index+1) ? iolets->nout_message : io->s_index+1;
        }
        else if(io->s_static){
            iolets->staticout = 1;
        }
        else{
            iolets->nouts_signal = (iolets->nouts_signal > io->s_index+1) ? iolets->nouts_signal : io->s_index+1;
        }
    }
}

static void spam_iolets_set_samples(t_spam_iolets* iolets, t_spam_io* io, t_spam_signal* signal, int nstatic)
{
    if(!io->s_type)
    {
        if(io->s_signal && io->s_static)
        {
            ;
        }
        else if(io->s_signal && !io->s_static)
        {
            
        }
    }
    else
    {
        if(io->s_signal && io->s_static)
        {
            
        }
        else if(io->s_signal && !io->s_static)
        {
            
        }
    }
}


static int spam_iolets_has_insig(t_spam_iolets* iolets)
{
    return (iolets->staticins || iolets->nins_signal) ? 1 : 0;
}

static int spam_iolets_has_outsig(t_spam_iolets* iolets)
{
    return (iolets->staticout || iolets->nouts_signal) ? 1 : 0;
}

static int spam_iolets_get_ninsig(t_spam_iolets* iolets, int nstatics)
{
    return iolets->staticins * nstatics + iolets->nins_signal;
}

static int spam_iolets_get_noutsig(t_spam_iolets* iolets, int nstatics)
{
    return iolets->staticout * nstatics + iolets->nouts_signal;
}

static void spam_iolets_process(t_spam_iolets* iolets, t_object* x, t_canvas* cnv, int nstatics)
{
    char temp[MAXPDSTRING];
    int i, sig = 0;
    
    sig = spam_iolets_has_insig(iolets);
    if(sig)
    {
        for(i = 0; i < spam_iolets_get_ninsig(iolets, nstatics); ++i)
        {
            signalinlet_new((t_object *)x, 0);
        }
    }
    if(iolets->nins_message)
    {
        iolets->ins = (t_spam_inlet *)getbytes(iolets->nins_message * sizeof(t_spam_inlet));
        for(i = 0; i < iolets->nins_message; ++i)
        {
            sprintf(temp, "$0-in-%i", i);
            iolets->ins[i].s_pd    = spam_process_inlet_class;
            iolets->ins[i].s_sym   = canvas_realizedollar(cnv, gensym(temp));
            inlet_new((t_object *)x, &(iolets->ins[i].s_pd), 0, 0);
        }
    }
    
    sig = spam_iolets_has_outsig(iolets);
    if(sig)
    {
        for(i = 0; i < spam_iolets_get_noutsig(iolets, nstatics); ++i)
        {
            outlet_new((t_object *)x, &s_signal);
        }
    }
    
    if(iolets->nout_message)
    {
        iolets->outs = (t_spam_outlet *)getbytes(iolets->nout_message * sizeof(t_spam_outlet));
        for(i = 0; i < iolets->nout_message; ++i)
        {
            sprintf(temp, "$0-out-%i", i);
            iolets->outs[i].s_pd       = spam_process_outlet_class;
            iolets->outs[i].s_sym      = canvas_realizedollar(cnv, gensym(temp));
            iolets->outs[i].s_outlet   = outlet_new((t_object *)x, NULL);
            pd_bind(&(iolets->outs[i].s_pd), iolets->outs[i].s_sym);
        }
    }
}











typedef struct _spam
{
    t_object        s_obj;
    t_spam_master   s_master;
    t_spam_signal   s_signal;
    t_spam_iolets   s_iolet;
    int             s_nrows;
    int             s_ncolumns;
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
    spam_master_free((t_object *)x, &(x->s_master));
}

static void spam_process_vis(t_spam *x, t_floatarg index)
{
    if(x->s_master.s_cnv)
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
    spam_iolets_set_samples(&(x->s_iolet), io, &(x->s_signal), x->s_nrows * x->s_ncolumns);
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
    int nins    = spam_iolets_get_ninsig(&(x->s_iolet), x->s_nrows * x->s_ncolumns);
    int nouts   = spam_iolets_get_noutsig(&(x->s_iolet), x->s_nrows * x->s_ncolumns);
    int max     = (nins > nouts) ? nins : nouts;
    if(!spam_signal_alloc(&(x->s_signal), max, (sp && sp[0]) ? sp[0]->s_n : 0))
    {
        mess0((t_pd *)x->s_master.s_cnv, gensym("dsp"));
        /*
        j = 0;
        if(x->s_instatic)
        {
            for(j = 0; j < x->s_nrows * x->s_ncolumns; ++j)
            {
                sprintf(temp, "$0-in_static-%i", j);
                s = canvas_realizedollar(x->s_master.s_cnv, gensym(temp));
                if(s && s->s_thing)
                {
                    mess3(s->s_thing, gensym("set"), x->s_samples+j*sp[0]->s_n, 0, 0);
                }
            }
        }
        for(i = 0; i < x->s_ninssig; ++i, ++j)
        {
            sprintf(temp, "$0-in_tilde-%i", i);
            s = canvas_realizedollar(x->s_master.s_cnv, gensym(temp));
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
    int i, j;
    t_atom av[4];
    t_symbol* name;
    t_spam *x  = (t_spam *)pd_new(spam_process_class);
    if(x)
    {
        spam_signal_init(&(x->s_signal));
        spam_iolets_init(&(x->s_iolet));
        //x->s_fin        = NULL;
        
        x->s_nrows      = atom_getfloatarg(0, argc, argv);
        x->s_ncolumns   = atom_getfloatarg(1, argc, argv);
        name            = atom_getsymbolarg(2, argc, argv);
        if(x->s_nrows < 1|| x->s_ncolumns < 1 || name == &s_)
        {
            error("spam: wrong argument, expecting <nrows> <ncolumns> <name> <...>");
            pd_free((t_pd *)x);
            return NULL;
        }
        if(spam_master_init((t_object *)x, &(x->s_master)))
        {
            pd_free((t_pd *)x);
            return NULL;
        }
        
        SETFLOAT(av, x->s_nrows);
        SETFLOAT(av+1, x->s_ncolumns);
        for(i = 0; i < x->s_nrows; ++i)
        {
            SETFLOAT(av+2, i);
            for(j = 0; j < x->s_ncolumns; ++j)
            {
                SETFLOAT(av+3, j);
                if(spam_master_load_canvas(&(x->s_master), name, 4, av, argc-3, argv+3))
                {
                    error("spam: can't load subpatch '%s'", name->s_name);
                    pd_free((t_pd *)x);
                    return NULL;
                }
            }
        }
        
        spam_iolets_process(&(x->s_iolet), (t_object *)x, x->s_master.s_cnv, x->s_ncolumns * x->s_nrows);
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
    
    c = class_new(gensym("spam-inlet"), 0, 0, sizeof(t_spam_inlet), CLASS_PD, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_process_inlet_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_process_inlet_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_inlet_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_process_inlet_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_process_inlet_anything,   gensym("anything"),A_GIMME, 0);
    }
    spam_process_inlet_class = c;
    
    
    c = class_new(gensym("spam-outlet"), 0, 0, sizeof(t_spam_outlet), CLASS_PD, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_process_outlet_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_process_outlet_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_outlet_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_process_outlet_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_process_outlet_anything,   gensym("anything"),A_GIMME, 0);
    }
    spam_process_outlet_class = c;
    
}



