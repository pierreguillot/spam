/*
 // Copyright (c) 2012-2015 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
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
    int i, j, index = 0;
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    int nx1, nx2, ny1, ny2;
    t_gobj *y = NULL;
    t_gobj *list[512];
    t_outconnect *oc;
    t_symbol* name;
    t_canvas* mcnv = x->s_canvas;
    t_canvas* cnv;
    
    if(mcnv)
    {
        for(i = 0; i < 512; i++)
        {
            list[i] = NULL;
        }
        for(y = mcnv->gl_list; y; y = y->g_next)
        {
            if(strncmp((const char *)class_getname(y->g_pd), "spam.", 5) == 0 && glist_isselected(mcnv, y))
            {
                gobj_getrect(y, mcnv, &nx1, &ny1, &nx2, &ny2);
                for(i = 0; i < index; i++)
                {
                    gobj_getrect(list[i], mcnv, &x1, &y1, &x2, &y2);
                    if(ny1 < y1 || (ny1 == y1 && nx1 < x1))
                    {
                        for(j = index; j > i; j--)
                        {
                            list[j] = list[j-1];
                        }
                        list[i] = y;
                        index = (index + 1 > 255) ? 255 : index+1;
                        break;
                    }
                }
                if(i == index)
                {
                    list[index] = y;
                    index = (index + 1 > 255) ? 255 : index+1;
                }
            }
            else if(y->g_pd == canvas_class)
            {
                cnv = (t_glist *)y;
                if(cnv->gl_obj.te_binbuf && binbuf_getnatom(cnv->gl_obj.te_binbuf) && binbuf_getvec(cnv->gl_obj.te_binbuf))
                {
                    if(binbuf_getvec(cnv->gl_obj.te_binbuf)->a_type == A_SYMBOL)
                        name = atom_getsymbol(binbuf_getvec(cnv->gl_obj.te_binbuf));
                    else
                        name  = NULL;
                    
                    if(name && strncmp(name->s_name, "spam.", 5) == 0 && glist_isselected(mcnv, y))
                    {
                        gobj_getrect(y, mcnv, &nx1, &ny1, &nx2, &ny2);
                        for(i = 0; i < index; i++)
                        {
                            gobj_getrect(list[i], mcnv, &x1, &y1, &x2, &y2);
                            if(ny1 < y1 || (ny1 == y1 && nx1 < x1))
                            {
                                for(j = index; j > i; j--)
                                {
                                    list[j] = list[j-1];
                                }
                                list[i] = y;
                                index = (index + 1 > 255) ? 255 : index+1;
                                break;
                            }
                        }
                        if(i == index)
                        {
                            list[index] = y;
                            index = (index + 1 > 255) ? 255 : index+1;
                        }
                    }
                    
                }
            }
        }
        for(i = 0; i < index-1; i++)
        {
            for(j = 0; j < obj_noutlets((t_object *)list[i]) && j < obj_ninlets((t_object *)list[i+1]); j++)
            {
                obj_disconnect((t_object *)list[i], j, (t_object *)list[i+1], j);
                oc = obj_connect((t_object *)list[i], j, (t_object *)list[i+1], j);
                if(glist_isvisible(mcnv))
                {
                    sys_vgui((char *)".x%lx.c create line %d %d %d %d -width %d -fill black -tags [list l%lx cord]\n",
                             glist_getcanvas(mcnv), 0, 0, 0, 0,
                             (obj_issignaloutlet((t_object *)list[i], j) ? 2 : 1),
                             oc);
                }
                
            }
            canvas_fixlinesfor(mcnv, (t_text *)list[i]);
        }
        canvas_dirty(mcnv, 1);
    }
}

void setup_spam0x2econnect(void)
{
    t_class* c = class_new(gensym("spam.connect"), (t_newmethod)spam_connect_new, NULL, (short)sizeof(t_spam_connect), 0, 0);
    if(c)
    {
        class_addbang(c, spam_connect_bang);
    }
    spam_connect_class = c;
}

