#include "threadpool.h"
#include "def.h"
#include "condtion.h"
#include "httpd.h"

//线程初始化后，就立即执行任务。阻塞在信号量上。没有销毁函数。直接用命令行杀进程。

void thread_pool_init(thread_pool_t *pool, int counter)
{
	cond_init(&pool->ready);
	pool->first = NULL;
	pool->last = NULL;
	pool->counter = counter;
	int i;
	for(i = 0; i<counter; i++)
	{
		pthread_t tid;
		pthread_create(&tid, NULL, thread_pool_run, pool);		
	}
}

void* thread_pool_run(void* arg)
{
	thread_pool_t *pool = arg;
	printf("thread 0x%x is running...\n", (int)pthread_self());
	while(1)
	{
		cond_lock(&pool->ready);
		while(pool->first == NULL)
			cond_wait(&pool->ready);
		task_t *task = pool->first;
		pool->first = task->next;
		cond_unlock(&pool->ready);
		task_run(task->conn); 
	}
}
void thread_pool_addtask(thread_pool_t *pool, conn_t *conn)
{
	task_t *newtask = (task_t*)malloc(sizeof(task_t));
	newtask->conn = conn;
	newtask->next = NULL;

	cond_lock(&pool->ready);
	if(pool->first == NULL)
		pool->first = newtask;
	else
		pool->last->next = newtask;
	pool->last = newtask;

	cond_signal(&pool->ready);
	cond_unlock(&pool->ready);
}


