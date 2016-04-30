/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"
#include "../pd/src/g_canvas.h"
#include <string.h>
#include <stdarg.h>

extern t_class* canvas_class;
static t_class* spam_process_inlet_class;
static t_class* spam_process_outlet_class;

typedef struct _spam_inlet
{
    t_class*    s_pd;
    t_symbol*   s_sym;
} t_spam_inlet;

typedef struct _spam_outlet
{
    t_class*    s_pd;
    t_outlet*   s_outlet;
    t_symbol*   s_sym;
} t_spam_outlet;

static int spam_iolets_get_ninsig(t_spam_iolets* iolets, int nstatics)
{
    return iolets->staticins * nstatics + iolets->nins_signal;
}

static int spam_iolets_get_noutsig(t_spam_iolets* iolets, int nstatics)
{
    return iolets->staticout * nstatics + iolets->nouts_signal;
}

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

static void spam_iolets_newio(t_spam_master* master, t_spam_iolets* iolets)
{
    char temp[MAXPDSTRING];
    int i;
    for(i = 0; i < spam_iolets_get_ninsig(iolets, master->s_n); ++i)
    {
        signalinlet_new((t_object *)master, 0);
    }
    if(iolets->nins_message)
    {
        iolets->ins = (t_spam_inlet *)getbytes(iolets->nins_message * sizeof(t_spam_inlet));
        for(i = 0; i < iolets->nins_message; ++i)
        {
            sprintf(temp, "$0-in-%i", i);
            iolets->ins[i].s_pd    = spam_process_inlet_class;
            iolets->ins[i].s_sym   = canvas_realizedollar(master->s_canvas, gensym(temp));
            inlet_new((t_object *)master, &(iolets->ins[i].s_pd), 0, 0);
        }
    }
    
    for(i = 0; i < spam_iolets_get_noutsig(iolets, master->s_n); ++i)
    {
        outlet_new((t_object *)master, &s_signal);
    }
    if(iolets->nout_message)
    {
        iolets->outs = (t_spam_outlet *)getbytes(iolets->nout_message * sizeof(t_spam_outlet));
        for(i = 0; i < iolets->nout_message; ++i)
        {
            sprintf(temp, "$0-out-%i", i);
            iolets->outs[i].s_pd       = spam_process_outlet_class;
            iolets->outs[i].s_sym      = canvas_realizedollar(master->s_canvas, gensym(temp));
            iolets->outs[i].s_outlet   = outlet_new((t_object *)master, NULL);
            pd_bind(&(iolets->outs[i].s_pd), iolets->outs[i].s_sym);
        }
    }
}

















static t_canvas* spam_master_load_canvas(t_spam_master* master, t_symbol* name, int index, int argc, t_atom* argv)
{
    int n;
    t_gobj* z;
    t_atom* vec, *av;
    if(master->s_canvas)
    {
        av = getbytes((argc + 4) * sizeof(t_atom));
        if(av)
        {
            SETFLOAT(av, 1);
            SETFLOAT(av+1, 1);
            SETSYMBOL(av+2, name);
            SETFLOAT(av+3, (float)index);
            if(argc && argv)
            {
                memcpy((void *)(av+4), (void *)(argv), argc * sizeof(t_atom));
            }
            pd_typedmess((t_pd *)master->s_canvas, gensym("obj"), (argc + 4), av);
            
            for(z = master->s_canvas->gl_list; z; z = z->g_next)
            {
                if(z->g_pd == canvas_class)
                {
                    n   = binbuf_getnatom(((t_object *)z)->te_binbuf);
                    vec = binbuf_getvec(((t_object *)z)->te_binbuf);
                    if(n && vec && n >= 2 && atom_getsymbol(vec) == name)
                    {
                        if(vec[0].a_type == A_SYMBOL && atom_getsymbol(vec) == name &&
                           vec[1].a_type == A_FLOAT && atom_getfloat(vec+1) == index)
                        {
                            freebytes(av, (argc + 4) * sizeof(t_atom));
                            canvas_loadbang((t_canvas *)z);
                            return (t_canvas *)z;
                        }
                    }
                }
            }
            freebytes(av, (argc + 4) * sizeof(t_atom));
            return NULL;
        }
    }
    return NULL;
}

char spam_master_init(t_spam_master* master, t_symbol* name, int n, int argc, t_atom* argv)
{
    int i;
    t_atom av[3];
    t_symbol *s = NULL;
    master->s_n         = 0;
    master->s_subcanvas = NULL;
    master->s_canvas    = NULL;
    master->s_currentn  = 0;
    master->s_sigs     = NULL;
    
    spam_iolets_init(&(master->s_iolets));
    master->s_canvas = canvas_new(NULL, gensym(""), 0, NULL);
    if(master->s_canvas)
    {
        pd_popsym((t_pd *)master->s_canvas);
        canvas_vis(master->s_canvas, 0);
        SETFLOAT(av, 10);
        SETFLOAT(av+1, 10);
        SETSYMBOL(av+2, gensym("switch~"));
        pd_typedmess((t_pd *)master->s_canvas, gensym("obj"), 3, av);
        if(!strncmp((const char *)class_getname(master->s_canvas->gl_list->g_pd), "block~", 6))
        {
            master->s_block = (t_object *)master->s_canvas->gl_list;
            master->s_block_tick = (t_spam_bang_method)zgetfn((t_pd *)master->s_block, &s_bang);
        }
        else
        {
            error("spam.process~: can't allocate master switch~ object.");
            return -1;
        }
    }
    else
    {
        error("spam.process~: can't allocate master canvas.");
        return -1;
    }
    s = canvas_realizedollar(master->s_canvas, gensym("$0-spam-process"));
    if(s)
    {
        s->s_thing = (struct _class **)master;
    }
    
    
    // Loads sub canvases
    master->s_subcanvas = (t_canvas **)getbytes(n * sizeof(t_canvas *));
    if(master->s_subcanvas)
    {
        master->s_n = n;
        for(i = 0; i < n; ++i)
        {
            master->s_currentn = i;
            master->s_subcanvas[i] = spam_master_load_canvas(master, name, i, argc, argv);
            if(!master->s_subcanvas[i])
            {
                error("spam.process~: can't allocate subcanvas %i.", i);
                return -1;
            }
        }
        spam_iolets_newio(master, &(master->s_iolets));
    }
    else
    {
        error("spam.process~: can't allocate subcanvases.");
        return -1;
    }
    
    return 0;
}

char spam_master_free(t_spam_master* master)
{
    t_symbol *s = NULL;
    if(master->s_canvas)
    {
        s = canvas_realizedollar(master->s_canvas, gensym("$0-spam-process"));
        if(s)
        {
            s->s_thing = NULL;
        }
        canvas_free(master->s_canvas);
        if(master->s_n && master->s_subcanvas)
        {
            freebytes(master->s_subcanvas, master->s_n * sizeof(t_canvas *));
        }
        spam_iolets_free(&(master->s_iolets));
        return 0;
    }
    return -1;
}

char spam_master_visible(t_spam_master* master, int index)
{
    if(master->s_n && master->s_subcanvas && index < master->s_n)
    {
        canvas_vis(master->s_subcanvas[index], 1);
        return 0;
    }
    return -1;
}

char spam_master_dsp(t_spam_master* master, t_signal **sp)
{
    master->s_sigs = sp;
    mess0((t_pd *)master->s_canvas, gensym("dsp"));
    return 0;
}

void spam_master_io_init(t_spam_master* master, t_spam_io* io)
{
    t_spam_iolets* iolets = &(master->s_iolets);
    if(!io->s_type){
        if(!io->s_signal){
            iolets->nins_message = (iolets->nins_message > io->s_index+1) ? iolets->nins_message : io->s_index+1;
        }
        else if(io->s_static){
            io->s_index = master->s_currentn;
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
            io->s_index = master->s_currentn;
            iolets->staticout = 1;
        }
        else{
            iolets->nouts_signal = (iolets->nouts_signal > io->s_index+1) ? iolets->nouts_signal : io->s_index+1;
        }
    }
}


void spam_master_io_dsp(t_spam_master* master, t_spam_io* io)
{
    t_signal** sp = master->s_sigs;
    t_spam_iolets* iolets = &(master->s_iolets);
    int n = io->s_type ? spam_iolets_get_ninsig(&(master->s_iolets), master->s_n) : 0;
    if(io->s_signal && (io->s_static || !iolets->staticins))
    {
        io->s_samples = sp[io->s_index+n]->s_vec;
        io->s_n       = sp[io->s_index+n]->s_n;
    }
    else if(io->s_signal && (!io->s_static && iolets->staticins))
    {
        io->s_samples = sp[io->s_index+master->s_n+n]->s_vec;
        io->s_n       = sp[io->s_index+master->s_n+n]->s_n;
    }
}

int spam_master_get_nsignals(t_spam_master* master, int type)
{
    if(type)
    {
        return spam_iolets_get_noutsig(&(master->s_iolets), master->s_n);
    }
    else
    {
        return spam_iolets_get_ninsig(&(master->s_iolets), master->s_n);
    }
}




















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

void spam_master_setup(void)
{
    t_class *c = class_new(gensym("spam-inlet"), 0, 0, sizeof(t_spam_inlet), CLASS_PD, 0);
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























