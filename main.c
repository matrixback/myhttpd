#include "httpd.h"
#include "def.h"
#include "threadpool.h"

int main(void)
{
	 int port = 8000;
	 int server_sock;
	 server_init(&server_sock, port);
	 print_serverinfo(&server_sock);	 
	 
     int threads = 5;	
	 thread_pool_t pool;
	 thread_pool_init(&pool, threads);

	 int connfd;
	 struct sockaddr_in client_sock;
	 socklen_t client_len;
	 while(1)
	 {
	 	connfd = accept(server_sock, 
	 						 (struct sockaddr*)&client_sock,
	 						  &client_len);
	 	if(connfd == -1)
	 		err_exit("accept");

	 	conn_t *conn = (struct connection*)malloc(sizeof(conn_t));
	 	conn->connfd = connfd;
	 	conn->client_sock = client_sock;
	 	thread_pool_addtask(&pool, conn);
	 }

	 close(server_sock);
	 return 0;
}
