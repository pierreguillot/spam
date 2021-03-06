/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "spam.utils.h"

extern void spam_tilde_setup(void);
extern void setup_spam0x2ein(void);
extern void setup_spam0x2eout(void);
extern void setup_spam0x2ein_tilde(void);
extern void setup_spam0x2eout_tilde(void);
extern void setup_spam0x2econnect(void);

extern void spam_setup(void)
{
    spam_tilde_setup();
    setup_spam0x2ein();
    setup_spam0x2eout();
    setup_spam0x2ein_tilde();
    setup_spam0x2eout_tilde();
    setup_spam0x2econnect();
}



