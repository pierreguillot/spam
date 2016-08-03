/*
 // Copyright (c) 2016 Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../pd/src/m_pd.h"
#include "../pd/src/m_imp.h"
#include "../pd/src/g_canvas.h"
#include <string.h>

typedef struct _spam_connect
{
    t_object s_object;
    t_canvas* s_canvas;
} t_spam_connect;

static t_class* spam_connect_class;

static void *spam_connect_new()
{
    t_spam_connect* x = (t_spam_connect *)pd_new(spam_connect_class);
    if(x)
    {
        x->s_canvas = canvas_getcurrent();
    }
    return x;
}

static void spam_connect_bang(t_spam_connect *x)
{
    int nouts, nins, i;
    t_object *first = NULL, *second = NULL;
    int visible  = (x->s_canvas) ? glist_isvisible(x->s_canvas) : 0;
    t_editor* ed = (x->s_canvas) ? x->s_canvas->gl_editor : NULL;
    t_selection *sel;
    t_outconnect *oc;
    if(ed && visible)
    {
        for(sel = ed->e_selection; sel; sel = sel->sel_next)
        {
            if(!strncmp((const char *)class_getname(sel->sel_what->g_pd), "spam", 4))
            {
                if(first)
                {
                    second = (t_object *)sel->sel_what;
                    nouts= obj_noutlets(second);
                    nins = obj_ninlets(first);
                    for(i = 0; i < nouts && i < nins; ++i)
                    {
                        obj_disconnect(second, i, first, i);
                        oc = obj_connect(second, i, first, i);
                        sys_vgui((char *)".x%lx.c create line %d %d %d %d -width %d -fill black -tags [list l%lx cord]\n",
                                 x->s_canvas, 0, 0, 0, 0, (obj_issignaloutlet(second, i) ? 2 : 1), oc);
                    }
                    canvas_fixlinesfor(x->s_canvas, (t_text *)second);
                    first  = second;
                    second = NULL;
                }
                else
                {
                    first = (t_object *)sel->sel_what;
                }
            }
        }
        canvas_dirty(x->s_canvas, 1);
    }
}

extern void setup_spam0x2econnect(void)
{
    t_class* c = class_new(gensym("spam.connect"), (t_newmethod)spam_connect_new, NULL, (short)sizeof(t_spam_connect), 0, 0);
    if(c)
    {
        class_addbang(c, spam_connect_bang);
    }
    spam_connect_class = c;
}

