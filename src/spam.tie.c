/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.tie.h"
#include "../pd/src/g_canvas.h"
#include <string.h>

static t_class* spam_tie_class;


static void spam_tie_setprocess(t_spam_tie* x, t_object* object)
{
    x->s_process = object;
}

static void *spam_tie_new()
{
    t_spam_tie *x = (t_spam_tie *)pd_new(spam_tie_class);
    if(x)
    {
        static int index = 0;
        x->s_index = index++;
    }
    return x;
}

void spam_tie_setup(void)
{
    t_class *c = class_new(gensym("spam.tie"), (t_newmethod)spam_tie_new, NULL, sizeof(t_spam_tie), CLASS_NOINLET, 0);
    if(c)
    {
        class_addmethod(c, (t_method)spam_tie_setprocess,  gensym("setprocess"), A_CANT,  0);
    }
    spam_tie_class = c;
}


t_spam_tie* spam_tie_get(t_glist* gl)
{
    t_gobj* y = NULL;
    while(gl)
    {
        for(y = gl->gl_list; y; y = y->g_next)
        {
            if(!strncmp((const char *)class_getname(y->g_pd), "spam.tie", 8))
            {
                return (t_spam_tie*)y;
            }
        }
        gl = gl->gl_owner;
    }
    return NULL;
}

t_symbol* spam_tie_gensym(t_spam_tie* tie, char* txt, int i)
{
    char temp[MAXPDSTRING];
    if(tie)
    {
        sprintf(temp, "spam-%i-%s-%i", tie->s_index, txt, i);
        return gensym(temp);
    }
    return NULL;
}

t_object* spam_tie_get_process(t_spam_tie* tie)
{
    if(tie)
    {
        return tie->s_process;
    }
    return NULL;
}






















