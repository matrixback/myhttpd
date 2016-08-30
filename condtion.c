#include "condtion.h"
#include <pthread.h>

int cond_init(cond_t *cond)
{
	int ret;
	ret = pthread_mutex_init(&cond->pmutex, NULL);
	if(ret == -1)
		err_exit("pthread_mutex_init");

	ret = pthread_cond_init(&cond->pcond, NULL);
	if(ret == -1)
	{
		pthread_mutex_destroy(&cond->pmutex);
		err_exit("pthread_cond_init");
	}

	return 0;
}

int cond_lock(cond_t *cond)
{
	return pthread_mutex_lock(&cond->pmutex);
}
int cond_unlock(cond_t *cond)
{
	return pthread_mutex_unlock(&cond->pmutex);
}
int cond_wait(cond_t *cond)
{
	return pthread_cond_wait(&cond->pcond, &cond->pmutex);
}
int cond_signal(cond_t *cond)
{
	return pthread_cond_signal(&cond->pcond);
}
int cond_broadcast(cond_t *cond)
{
	return pthread_cond_broadcast(&cond->pcond);
}
