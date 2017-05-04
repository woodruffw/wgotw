#ifndef WGOTW_H
#define WGOTW_H

#include <sys/queue.h>

#define WGOTW_SESSION_FMT "wgotw-%d"

#ifdef WGOTW_MAIN
struct wgotw_session *session;
#else
extern struct wgotw_session *session;
#endif

#define VERBOSE(fmt, ...) \
	do { \
		if (session->opts.verbose) { \
			fprintf(stderr, "[+] " fmt "\n", __VA_ARGS__); \
		} \
	} while(0)

#define DEBUG(fmt, ...) \
	do { \
		if (session->opts.debug) { \
			fprintf(stderr, "[!] " fmt "\n", __VA_ARGS__); \
		} \
	} while(0)

enum WGOTW_BUFFER_TYPE {
	BUFFER_INBOUND,
	BUFFER_OUTBOUND,
	BUFFER_UNKNOWN,
};

struct wgotw_options {
	int verbose;
	int debug;
};

struct wgotw_session {
	int pid;
	char *dir;
	struct wgotw_options opts;
	SLIST_HEAD(, wgotw_connection) addr_head;
};

struct wgotw_connection {
	char *dir;
	char *address;
	int port;
	long count;
	SLIST_ENTRY(wgotw_connection) connections;
};

void wgotw_session_init();
void wgotw_session_free();
void wgotw_record(int type, int sockfd, const void *buf, size_t len);



#endif
