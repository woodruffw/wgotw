#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "util.h"
#include "wgotw.h"

/* send calls */
static ssize_t (*__real_send)(int sockfd, const void *buf, size_t len,
	int flags);
static ssize_t (*__real_sendto)(int sockfd, const void *buf, size_t len,
	int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
static ssize_t (*__real_sendmsg)(int sockfd, const struct msghdr *msg,
	int flags);

/* recv calls */
static ssize_t (*__real_recv)(int sockfd, void *buf, size_t len, int flags);
static ssize_t (*__real_recvfrom)(int sockfd, void *buf, size_t len, int flags,
	struct sockaddr *src_addr, socklen_t *addrlen);
static ssize_t (*__real_recvmsg)(int sockfd, struct msghdr *msg, int flags);

__attribute__((constructor)) void wgotw_socket_init()
{
	DEBUG("%s", "WGOtW: wrapping socket functions...");

	__real_send = dlsym(RTLD_NEXT, "send");
	__real_sendto = dlsym(RTLD_NEXT, "sendto");
	__real_sendmsg = dlsym(RTLD_NEXT, "sendmsg");

	__real_recv = dlsym(RTLD_NEXT, "recv");
	__real_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
	__real_recvmsg = dlsym(RTLD_NEXT, "recvmsg");
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
	wgotw_record(BUFFER_OUTBOUND, sockfd, buf, len);

	return __real_send(sockfd, buf, len, flags);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
	const struct sockaddr *dest_addr, socklen_t addrlen)
{
	wgotw_record(BUFFER_OUTBOUND, sockfd, buf, len);

	return __real_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
	// printf("sendmsg: iovlen: %d\n", msg->msg_iovlen);

	return __real_sendmsg(sockfd, msg, flags);
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
	size_t nrecv = __real_recv(sockfd, buf, len, flags);

	wgotw_record(BUFFER_INBOUND, sockfd, buf, nrecv);

	return nrecv;
}

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
	struct sockaddr *src_addr, socklen_t *addrlen)
{
	size_t nrecv = __real_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);

	wgotw_record(BUFFER_INBOUND, sockfd, buf, nrecv);

	return nrecv;
}

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	size_t nrecv = __real_recvmsg(sockfd, msg, flags);

	// printf("recvmsg: iovlen: %d\n", msg->msg_iovlen);

	return nrecv;
}
