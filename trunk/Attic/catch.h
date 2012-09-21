#ifndef _CATCH_H_
#define _CATCH_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "manager.h"
#include "resources.h"



manager catch_distributor;



FILE *catch_fopen(const char *path, const char *mode);
FILE *catch_fdopen(int fildes, const char *mode);
FILE *catch_freopen(const char *path, const char *mode, FILE *stream);
FILE *catch_tmpfile(char *file, size_t line);

void *catch_calloc(size_t nmemb, size_t size);
void *catch_malloc(size_t size);
void *catch_realloc(void *ptr, size_t size);

int catch_open_2_arg(const char *pathname, int flags);
int catch_open_3_arg(const char *pathname, int flags, mode_t mode);
int catch_creat(const char *pathname, mode_t mode);

int catch_socket(int domain, int type, int protocol);
int catch_bind(int sockfd,  const  struct  sockaddr  *my_addr,  socklen_t addrlen);
int catch_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);



int catch_start(int argc, const char **argv);

#define fopen(path, mode) catch_fopen(path, mode)
#define fdopen(fildes, mode) catch_fdopen(fildes, mode)
#define freopen(path, mode, stream) catch_freopen(path, mode, stream)
#define tmpfile() catch_tmpfile(__FILE__, __LINE__)


#define calloc(nmemb, size) catch_calloc(nmemb, size)
#define malloc(size) catch_malloc(size)
#define realloc(ptr, size) catch_realloc(ptr, size)


#define open catch_open
#define creat(pathname, mode) catch_creat(pathname, mode)


#define socket(domain, type, protocol) catch_socket(domain, type, protocol)
#define bind(sockfd, my_addr, addrlen) catch_bind(sockfd, my_addr, addrlen)
#define accept(sockfd, addr, addrlen) catch_accept(sockfd, addr, addrlen)


#endif
