#include "catch.h"

#undef fopen
#undef fdopen
#undef freopen
#undef tmpfile
#undef calloc
#undef realloc
#undef malloc
#undef open
#undef creat
#undef socket
#undef bind
#undef accept


int catch_start(int argc, const char **argv)
{
    extern manager catch_distributor;
    manager_init(&catch_distributor);
    int i=1;
    for(; i<argc; i+= 2)
	if(manager_add_str(&catch_distributor, argv[i], argv[i+1]) == -1)
	    return -1;
    return 0;
}


FILE *catch_fopen(const char *path, const char *mode)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_FOPEN))
    	return fopen(path, mode);
    else
	return NULL;
}


FILE *catch_fdopen(int fildes, const char *mode)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_FDOPEN))
	return fdopen(fildes, mode); 
    else
	return NULL;
}


FILE *catch_freopen(const char *path, const char *mode, FILE *stream)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_FREOPEN))
	return freopen(path, mode, stream); 
    else
	return NULL;
}


FILE *catch_tmpfile(char *file, size_t line)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_TMPFILE))
	return tmpfile(); 
    else
	return NULL;
}

void *catch_calloc(size_t nmemb, size_t size)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_CALLOC))
	return calloc(nmemb, size);
    else
	return NULL;
}

void *catch_malloc(size_t size)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_MALLOC))
	return malloc(size);
    else
	return NULL;
}

void *catch_realloc(void *ptr, size_t size)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_REALLOC))
	return realloc(ptr, size);
    else
	return NULL;
}


int catch_open_2_arg(const char *pathname, int flags)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_OPEN))
	return open(pathname, flags);
    else
	return -1;
}

int catch_open_3_arg(const char *pathname, int flags, mode_t mode)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_OPEN))
	return open(pathname, flags, mode);
    else
	return -1;
}


int catch_creat(const char *pathname, mode_t mode)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_CREAT))
	return creat(pathname, mode);
    else
	return -1;
}


int catch_socket(int domain, int type, int protocol)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_SOCKET))
	return socket(domain, type, protocol);
    else
	return -1;
}


int catch_bind(int sockfd,  const  struct  sockaddr  *my_addr,  socklen_t addrlen)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_BIND))
	return bind(sockfd, my_addr, addrlen);
    else
	return -1;
}

int catch_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, RESOURCE_ACCEPT))
	return accept(sockfd, addr, addrlen);
    else
	return -1;
}


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
