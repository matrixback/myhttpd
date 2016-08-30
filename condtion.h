#ifndef _CONDITION_H
#define _CONDITION_H


#include "def.h"

int cond_init(cond_t *cond);
int cond_lock(cond_t *cond);
int cond_unlock(cond_t *cond);
int cond_wait(cond_t *cond);
int cond_signal(cond_t *cond);
int cond_broadcast(cond_t *cond);

#endif

