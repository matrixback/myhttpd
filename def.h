#ifndef _DEF_H_
#define _DEF_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define err_exit(m) (perror(m), exit(1))
#define SERVER_STRING "Server: matrix_httpd/1.0\r\n"

//保存连接的套接字描述符及客户信息
typedef struct connection
{
	int connfd;
	struct sockaddr_in client_sock;

}conn_t;

//线程池中的任务队列
typedef  struct task
{
	conn_t *conn;
	struct task *next;
}task_t;

//条件变量
typedef struct condtion{
	pthread_mutex_t pmutex;
	pthread_cond_t pcond;
}cond_t;

typedef struct threadpool
{
	cond_t ready;
	task_t *first;
	task_t *last;
	int counter;
}thread_pool_t;



#endif
