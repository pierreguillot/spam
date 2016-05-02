/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"
#include "spam.tie.h"
#include "../pd/src/g_canvas.h"
#include <string.h>
#include <stdarg.h>

static t_class* spam_process_inlet_class;
static t_class* spam_process_outlet_class;

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
    iolets->nins_message= 0;
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
    int i;
    int ninsig = spam_iolets_get_ninsig(iolets, master->s_n);
    for(i = 1; i < ninsig; ++i)
    {
        signalinlet_new((t_object *)master, 0);
    }
    if(iolets->nins_message)
    {
        iolets->ins = (t_spam_inlet *)getbytes(iolets->nins_message * sizeof(t_spam_inlet));
        if(!ninsig)
        {
            master->s_fin = spam_tie_gensym((t_spam_tie *)master->s_tie, __spam_in_string__, 0);
        }
        for(i = ninsig ? 0 : 1; i < iolets->nins_message; ++i)
        {
            iolets->ins[i].s_pd    = spam_process_inlet_class;
            iolets->ins[i].s_sym   = spam_tie_gensym((t_spam_tie *)master->s_tie, __spam_in_string__, i);
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
            iolets->outs[i].s_pd       = spam_process_outlet_class;
            iolets->outs[i].s_sym      = spam_tie_gensym((t_spam_tie *)master->s_tie, __spam_out_string__, i);
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

static t_object* spam_mater_get_object(t_canvas* cnv, const char* name, size_t s)
{
    t_gobj* y = NULL;
    for(y = cnv->gl_list; y; y = y->g_next)
    {
        if(!strncmp((const char *)class_getname(y->g_pd), name, s))
        {
            return (t_object *)y;
        }
    }
    return NULL;
}

char spam_master_init(t_spam_master* master, t_symbol* name, int n, int argc, t_atom* argv)
{
    int i;
    t_atom av[3];
    master->s_n         = 0;
    master->s_subcanvas = NULL;
    master->s_canvas    = NULL;
    master->s_currentn  = 0;
    master->s_sigs      = NULL;
    master->s_outputs   = NULL;
    master->s_routputs  = NULL;
    master->s_blksize   = 0;
    master->s_fin       = NULL;
    master->s_block     = NULL;
    master->s_tie       = NULL;
    
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
        SETSYMBOL(av+2, gensym("spam.tie"));
        pd_typedmess((t_pd *)master->s_canvas, gensym("obj"), 3, av);
        master->s_block = spam_mater_get_object(master->s_canvas,  "block~", 6);
        if(!master->s_block)
        {
            error("spam.process~: can't allocate master switch~ object.");
            return -1;
        }
        master->s_tie  = spam_mater_get_object(master->s_canvas,  "spam.tie", 8);
        if(!master->s_block)
        {
            error("spam.process~: can't allocate tie object.");
            return -1;
        }
        mess1((t_pd *)master->s_tie, gensym("setprocess"), (t_object*)master);
    }
    else
    {
        error("spam.process~: can't allocate master canvas.");
        return -1;
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
    int nouts = spam_iolets_get_noutsig(&(master->s_iolets), master->s_n);
    if(master->s_canvas)
    {
        canvas_free(master->s_canvas);
        if(master->s_n && master->s_subcanvas)
        {
            freebytes(master->s_subcanvas, master->s_n * sizeof(t_canvas *));
        }
        spam_iolets_free(&(master->s_iolets));
        if(master->s_outputs)
        {
            freebytes(master->s_outputs, master->s_blksize * nouts * sizeof(t_sample *));
            master->s_outputs = NULL;
            master->s_blksize = 0;
        }
        if(master->s_routputs)
        {
            freebytes(master->s_routputs,nouts * sizeof(t_sample **));
            master->s_routputs = NULL;
        }
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

static t_int *spam_process_perform(t_int *w)
{
    int i, j;
    int n                   = (int)(w[1]);
    int nouts               = (int)(w[2]);
    t_sample* iout          = (t_sample *)(w[3]);
    t_sample** rout         = (t_sample **)(w[4]);
    t_object* block         = (t_object *)(w[5]);
    
    for(i = 0; i < nouts * n; ++i)
    {
        iout[i] = 0.f;
    }
    pd_bang((t_pd *)block);
    for(i = 0; i < nouts; ++i)
    {
        for(j = 0; j < n; ++j)
        {
            rout[i][j] = iout[i*n+j];
        }
    }
    
    return (w+6);
}





char spam_master_dsp(t_spam_master* master, t_signal **sp)
{
    int i;
    int nins = spam_iolets_get_ninsig(&(master->s_iolets), master->s_n);
    int nouts = spam_iolets_get_noutsig(&(master->s_iolets), master->s_n);
    if(master->s_outputs)
    {
        freebytes(master->s_outputs, master->s_blksize * nouts * sizeof(t_sample *));
        master->s_outputs = NULL;
        master->s_blksize = 0;
    }
    if(master->s_routputs)
    {
        freebytes(master->s_routputs,nouts * sizeof(t_sample **));
        master->s_routputs = NULL;
    }
    if(sp && sp[0])
    {
        master->s_outputs = getbytes(sp[0]->s_n * nouts * sizeof(t_sample *));
        if(master->s_outputs)
        {
            master->s_blksize = sp[0]->s_n;
            master->s_sigs    = sp;
            master->s_routputs = (t_sample **)getbytes(nouts * sizeof(t_sample *));
            if(master->s_routputs)
            {
                for(i = 0; i < nouts; ++i)
                {
                    master->s_routputs[i] = sp[nins+i]->s_vec;
                }
                dsp_add(spam_process_perform, 5, (t_int)sp[0]->s_n, (t_int)nouts, (t_int)master->s_outputs, (t_int)master->s_routputs, (t_int)(master->s_block));
                mess0((t_pd *)master->s_canvas, gensym("dsp"));
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    
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
    if(!io->s_signal)
    {
        pd_error(master, "spam.process~: try use message input for signal.");
    }
    if(io->s_type == 0)
    {
        if(io->s_static)
        {
            io->s_samples = sp[io->s_index]->s_vec;
            io->s_n       = sp[io->s_index]->s_n;
        }
        else if(!io->s_static)
        {
            io->s_samples = sp[io->s_index + master->s_n * iolets->staticins]->s_vec;
            io->s_n       = sp[io->s_index + master->s_n * iolets->staticins]->s_n;
        }
    }
    else
    {
        if(io->s_static)
        {
            io->s_samples = master->s_outputs + master->s_blksize * io->s_index;
            io->s_n       = master->s_blksize;
        }
        else if(!io->s_static)
        {
            io->s_samples = master->s_outputs + master->s_blksize * (io->s_index + master->s_n * iolets->staticout);
            io->s_n       = master->s_blksize;
        }
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
    spam_tie_setup();
    t_class* c = class_new(gensym("spam-inlet"), 0, 0, sizeof(t_spam_inlet), CLASS_PD, 0);
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























