#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "condtion.h"
#include "def.h"

void thread_pool_init(thread_pool_t *pool, int counter);
void* thread_pool_run(void* arg);
void thread_pool_addtask(thread_pool_t *pool, conn_t *conn);

#endif
