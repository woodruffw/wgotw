#ifndef WGOTW_H
#define WGOTW_H

#include <sys/queue.h>

#define WGOTW_SESSION_FMT "wgotw-%d"
#define WGOTW_CON_TREE_FMT WGOTW_SESSION_FMT "/%s/%d"
#define WGOTW_CON_INBOUND_FMT WGOTW_CON_TREE_FMT "/inbound"
#define WGOTW_CON_OUTBOUND_FMT WGOTW_CON_TREE_FMT "/outbound"

struct wgotw_session {
	int pid;
	char *dir;
	SLIST_HEAD(, wgotw_connection) addr_head;
};

struct wgotw_connection {
	char *dir;
	char *address;
	int port;
	long count;
	SLIST_ENTRY(wgotw_connection) connections;
};

struct wgotw_session *wgotw_session_init();
void wgotw_session_free();
void wgotw_record_outgoing(int sockfd, void *buf, size_t len);
void wgotw_record_incoming(int sockfd, void *buf, size_t len);

#ifdef WGOTW_MAIN
struct wgotw_session *session;
#else
extern struct wgotw_session *session;
#endif

#endif
