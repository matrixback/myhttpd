#ifndef _HTTPD_H
#define _HTTPD_H

#include "def.h"

void server_init(int *sock_fd, int  port);
void print_serverinfo(const int *server_sock);
void task_run(conn_t *conn);
void unresolved(int client);
void send_not_found(int client);
void send_ok_headers(int client);
void send_ok_css(int client);
void send_index(int client);
ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, const void *buf, size_t count);



#endif
