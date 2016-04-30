/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef spam_io_h
#define spam_io_h
#include "spam.def.h"

char spam_io_init(t_spam_io* io, int idx, int type, int sig, int stat);
char spam_io_free(t_spam_io* io);
char spam_io_dsp(t_spam_io* io);

#endif // spam_io_h



