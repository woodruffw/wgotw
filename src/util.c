#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util.h"

struct sockfacts *sockfacts_init(int sockfd)
{
	struct sockfacts *facts = malloc(sizeof(struct sockfacts *));

	struct sockaddr_storage addr;
	socklen_t addr_len = sizeof(addr);

	if (getpeername(sockfd, (struct sockaddr *) &addr, &addr_len)) {
		return NULL;
	}
	else {
		if (addr.ss_family == AF_INET) {
			facts->address = malloc(INET_ADDRSTRLEN);
			struct sockaddr_in *s = (struct sockaddr_in *) &addr;
			inet_ntop(AF_INET, &s->sin_addr, facts->address, INET_ADDRSTRLEN);
			facts->port = ntohs(s->sin_port);
		}
		else if (addr.ss_family == AF_INET6) {
			facts->address = malloc(INET6_ADDRSTRLEN);
			struct sockaddr_in6 *s = (struct sockaddr_in6 *) &addr;
			inet_ntop(AF_INET, &s->sin6_addr, facts->address, INET6_ADDRSTRLEN);
			facts->port = ntohs(s->sin6_port);
		}
		else {
			return NULL;
		}
	}

	return facts;
}

void sockfacts_free(struct sockfacts *facts)
{
	free(facts->address);
	free(facts);
}

