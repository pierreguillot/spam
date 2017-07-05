/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_tie_h
#define spam_tie_h
#include "spam.def.h"

void spam_tie_setup(void);
t_spam_tie* spam_tie_get(t_glist* gl);
t_symbol* spam_tie_gensym(t_spam_tie* tie, char* txt, int i);
t_object* spam_tie_get_process(t_spam_tie* tie);


#endif // spam_utils_h



