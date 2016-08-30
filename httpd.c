#include "httpd.h"
#include "def.h"
#include <ctype.h>
#include <string.h>
void server_init(int *sockfd, int port)
{
	int ret;
	int listenfd;

	listenfd = socket(PF_INET, SOCK_STREAM,0);
	if(listenfd == -1)
		err_exit("socket");

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		err_exit("setsockopt");

	ret = bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(ret == -1)
		err_exit("bind");

	ret = listen(listenfd, SOMAXCONN);
	if(ret == -1)
		err_exit("listen");
	
	*sockfd = listenfd;

	
}

void print_serverinfo(const int *server_sock)
{
	struct sockaddr_in server;
	socklen_t len;
	int ret;
	ret = getsockname(*server_sock, (struct sockaddr*)&server, &len);
	if(ret == -1)
		err_exit("getsockname");
	printf("server work ip: %s, port: %d.\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

}

void task_run(conn_t *conn)
{
	char buf[1024];
	char method[16];	
	char path[512];

	char ip[16];
	strcpy(ip, inet_ntoa(conn->client_sock.sin_addr));
	
	int query_len = read(conn->connfd, buf, sizeof(buf));
	if(query_len == 0)
	{
		printf("read no data\n");
		close(conn->connfd);
		return;
	}
 	//获得方法和路径
	int i=0, j;
	while (isspace(buf[i]) && i < query_len)  //isspace return 0 when is not a space
		i++;
	j = 0;
	while(!isspace(buf[i])  && i < query_len)
		method[j++] = buf[i++];
	method[j] = '\0';

	while (isspace(buf[i]) && i < query_len)  //isspace return 0 when is not a space
		i++;
	path[0] = '.';
	j = 1;
	while(!isspace(buf[i])  && i < query_len)
		path[j++]=buf[i++];
	path[j] = '\0';
	
	printf("[HTTP] [IP: %s] [METHOD: %s] [PATH: %s]\n", ip, method, path);
	if(strcmp("GET", method) && strcmp("POST", method))
	{
		unresolved(conn->connfd);
		return;
	}

	if(strcmp("GET", method) == 0)
	{
		printf("path: %s\n", path);
		int fd = open(path, O_RDONLY);
		if(fd == -1)
		{
			send_not_found(conn->connfd);
			close(conn->connfd);
			free(conn);
			err_exit("open");
		}
		
		if(strcmp(path, "./index.html") == 0){
			printf("yes, enter the ok header\n");
			send_ok_headers(conn->connfd);
		}
		else
			send_ok_css(conn->connfd);
		int nread;
		while( (nread = read(fd, buf, 1024)) > 0)
		{
			write(conn->connfd, buf, nread);
		}
		close(fd);
		close(conn->connfd);
		free(conn);	
	}
}

void unresolved(int client)
{
 char buf[1024];

 sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type: text/html\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "</TITLE></HEAD>\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "</BODY></HTML>\r\n");
 send(client, buf, strlen(buf), 0);
}

void send_not_found(int client)
{
 char buf[1024];

 sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type: text/html\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "your request because the resource specified\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "is unavailable or nonexistent.\r\n");
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "</BODY></HTML>\r\n");
 send(client, buf, strlen(buf), 0);
}

void send_ok_headers(int client)
{
 char buf[1024];
 
 strcpy(buf, "HTTP/1.0 200 OK\r\n");
 send(client, buf, strlen(buf), 0);
 strcpy(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type:text/html\r\n");
 send(client, buf, strlen(buf), 0);
 strcpy(buf, "\r\n");
 send(client, buf, strlen(buf), 0);
}

void send_ok_css(int client)
{
	char buf[1024]; 
 	strcpy(buf, "HTTP/1.0 200 OK\r\n");
 	send(client, buf, strlen(buf), 0);
 	strcpy(buf, SERVER_STRING);
 	send(client, buf, strlen(buf), 0);
 	sprintf(buf, "Content-Type:text/css\r\n");
 	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
 	send(client, buf, strlen(buf), 0);
}

void send_index(int client)
{

 char buf[1024];
 
 strcpy(buf, "<html> <head> <title>matrix</title> </head><body> <p>hi, i am matrix,nice to meet you</p> </body></html>");
 send(client, buf, strlen(buf), 0);
}
ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nread;
    char *bufp = (char*)buf;

    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
                return -1;
            }
            else if (nread == 0)
                return count - nleft;

            bufp += nread;
            nleft -= nread;
        }
	}
    return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwritten;
    char *bufp = (char*)buf;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, bufp, nleft)) < 0)
        {
           if (errno == EINTR)
           {
                continue;
                return -1;
           }
            else if (nwritten == 0)
                continue;

            bufp += nwritten;
            nleft -= nwritten;
        }
    }
    return count;
}



