#ifndef WGOTW_UTIL_H
#define WGOTW_UTIL_H

struct sockfacts {
	char *address;
	int port;
};

struct sockfacts *sockfacts_init(int sockfd);
void sockfacts_free(struct sockfacts *facts);

#endif
