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







typedef struct _spam
{
    t_object        s_obj;
    t_spam_master   s_master;
    t_spam_signal   s_signal;
    int             s_nrows;
    int             s_ncolumns;
    
    t_symbol*       s_fin;
    int             s_nins;
    t_spam_inlet*   s_ins;
    
    char            s_instatic;
    int             s_ninssig;
    
    int             s_nouts;
    t_spam_outlet*  s_outs;
    char            s_outstatic;
    int             s_noutssig;
} t_spam;


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

static void spam_process_free(t_spam *x)
{
    int i;
    if(x->s_master.s_cnv)
    {
        pd_unbind((t_pd *)x, canvas_realizedollar(x->s_master.s_cnv, gensym("$0-master")));
    }
    if(x->s_nins && x->s_ins)
    {
        freebytes(x->s_ins, x->s_nins * sizeof(t_spam_inlet));
    }
    if(x->s_nouts && x->s_nouts)
    {
        for(i = 0; i < x->s_nouts; ++i)
        {
            pd_unbind((t_pd *)(x->s_outs+i), x->s_outs[i].s_sym);
        }
        freebytes(x->s_outs, x->s_nouts * sizeof(t_spam_outlet));
    }
    spam_signal_free(&(x->s_signal));
    spam_master_close(&(x->s_master));
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

static void spam_process_setio(t_spam *x, t_int iotype, t_int idx, t_int sig, t_int stic)
{
    if(iotype)
    {
        x->s_nouts = (idx + 1) > x->s_nouts ? (idx + 1) : x->s_nouts;
    }
    else
    {
        if(stic)
        {
            x->s_instatic = 1;
        }
        else if(sig)
        {
            x->s_ninssig = (idx + 1) > x->s_ninssig ? (idx + 1) : x->s_ninssig;
        }
        else
        {
            x->s_nins = (idx + 1) > x->s_nins ? (idx + 1) : x->s_nins;
        }
    }
}

static void spam_process_dsp(t_spam *x, t_signal **sp)
{
    int i, j, n;
    t_sample* v;
    t_symbol* s;
    t_atom* av;
    char temp[MAXPDSTRING];
    int nins = x->s_instatic * x->s_nrows * x->s_ncolumns + x->s_ninssig;
    int nouts = x->s_outstatic * x->s_nrows * x->s_ncolumns + x->s_noutssig;
    spam_signal_alloc(&(x->s_signal), (nins > nouts) ? nins : nouts, sp[0]->s_n);
    if(x->s_signal.s_samples)
    {
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
    else
    {
        pd_error(x, "can't allocate signal buffer.");
    }
}

static void *spam_process_new(t_symbol *s, int argc, t_atom *argv)
{
    int i, j;
    t_atom av[4];
    char temp[MAXPDSTRING];
    t_symbol* name;
    t_spam *x  = (t_spam *)pd_new(spam_process_class);
    if(x)
    {
        spam_signal_init(&(x->s_signal));
        
        x->s_nins       = 0;
        x->s_ins        = NULL;
        x->s_fin        = NULL;
        x->s_instatic   = 0;
        x->s_ninssig    = 0;
        
        x->s_nouts      = 0;
        x->s_outs       = NULL;
        
        x->s_nrows      = atom_getfloatarg(0, argc, argv);
        x->s_ncolumns   = atom_getfloatarg(1, argc, argv);
        name            = atom_getsymbolarg(2, argc, argv);
        if(x->s_nrows < 1|| x->s_ncolumns < 1 || name == &s_)
        {
            error("spam: wrong argument, expecting <nrows> <ncolumns> <name> <...>");
            pd_free((t_pd *)x);
            return NULL;
        }
        if(spam_master_init(&(x->s_master)))
        {
            pd_free((t_pd *)x);
            return NULL;
        }
        pd_bind((t_pd *)x, canvas_realizedollar(x->s_master.s_cnv, gensym("$0-master")));
        
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
        if(x->s_ninssig || x->s_instatic)
        {
            for(i = 0; i < (int)x->s_instatic * x->s_nrows * x->s_ncolumns + x->s_ninssig; ++i)
            {
                signalinlet_new(&(x->s_obj), 0.f);
            }
        }
        if(x->s_nins)
        {
            if(!(x->s_ninssig || x->s_instatic))
            {
                x->s_fin = canvas_realizedollar(x->s_master.s_cnv, gensym("$0-in-0"));
            }
            x->s_ins = (t_spam_inlet *)getbytes(x->s_nins * sizeof(t_spam_inlet));
            for(i = (int)(!(x->s_ninssig || x->s_instatic)); i < x->s_nins; ++i)
            {
                sprintf(temp, "$0-in-%i", i);
                x->s_ins[i].s_pd    = spam_process_inlet_class;
                x->s_ins[i].s_sym   = canvas_realizedollar(x->s_master.s_cnv, gensym(temp));
                inlet_new(&(x->s_obj), &x->s_ins[i].s_pd, 0, 0);
            }
        }
        
        if(x->s_nouts)
        {
            x->s_outs = (t_spam_outlet *)getbytes(x->s_nouts * sizeof(t_spam_outlet));
            for(i = 0; i < x->s_nouts; ++i)
            {
                sprintf(temp, "$0-out-%i", i);
                x->s_outs[i].s_pd       = spam_process_outlet_class;
                x->s_outs[i].s_sym      = canvas_realizedollar(x->s_master.s_cnv, gensym(temp));
                x->s_outs[i].s_outlet   = outlet_new((t_object *)x, NULL);
                pd_bind((t_pd *)(x->s_outs+i), x->s_outs[i].s_sym);
            }
        }
    }
    return x;
}

extern void setup_spam0x2eprocess_tilde(void)
{
    t_class *c = class_new(gensym("spam.process~"), (t_newmethod)spam_process_new, (t_method)spam_process_free, sizeof(t_spam), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_process_click,    gensym("click"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_vis,      gensym("vis"), A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_dsp,      gensym("dsp"), A_CANT, 0);
        class_addmethod(c, (t_method)spam_process_setio,    gensym("setio"), A_CANT, 0);
        
        
        class_addmethod(c, (t_method)spam_process_bang,       gensym("bang"),    A_NULL,  0);
        class_addmethod(c, (t_method)spam_process_float,      gensym("float"),   A_FLOAT, 0);
        class_addmethod(c, (t_method)spam_process_symbol,     gensym("symbol"),  A_SYMBOL,0);
        class_addmethod(c, (t_method)spam_process_list,       gensym("list"),    A_GIMME, 0);
        class_addmethod(c, (t_method)spam_process_anything,   gensym("anything"),A_GIMME, 0);
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



