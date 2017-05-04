#define WGOTW_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <stdbool.h>

#include "util.h"
#include "wgotw.h"

__attribute__((constructor (101))) void wgotw_init()
{
	session = wgotw_session_init();

	printf("[+] wgotw_init(): %s\n", session->dir);
}

__attribute__((destructor (101))) void wgotw_deinit()
{
	wgotw_session_free(session);
	printf("[+] wgotw_deinit()\n");
}

static struct wgotw_connection *connection_init(struct sockfacts *facts)
{
	struct wgotw_connection *con = malloc(sizeof(struct wgotw_connection));
	int addr_len = strlen(facts->address);

	con->address = malloc(addr_len);
	memcpy(con->address, facts->address, addr_len);
	con->port = facts->port;
	con->count = 0;

	/*
		this is why i don't write C anymore
	*/
	int len = snprintf(NULL, 0, WGOTW_SESSION_FMT "/%s", session->pid,
		con->address) + 1;
	char *addr_dir = malloc(len);
	snprintf(addr_dir, len, WGOTW_SESSION_FMT "/%s", session->pid,
		con->address);
	mkdir(addr_dir, 0777);

	len = snprintf(NULL, 0, "%s/%d", addr_dir, con->port) + 1;
	con->dir = malloc(len);
	snprintf(con->dir, len, "%s/%d", addr_dir, con->port);
	mkdir(con->dir, 0777);

	free(addr_dir);

	return con;
}

static void connection_free(struct wgotw_connection *con)
{
	free(con->address);
	free(con->dir);
	free(con);
}

static void ensure_session_connection(int sockfd)
{
	struct sockfacts *facts = sockfacts_init(sockfd);

	struct wgotw_connection *con;
	SLIST_FOREACH(con, &(session->addr_head), connections) {
		printf("ensure_connection: %s:%d\n", con->address, con->port);
		if (con->port == facts->port && !strcmp(con->address, facts->address)) {
			return;
		}
	}

	con = connection_init(facts);
	SLIST_INSERT_HEAD(&(session->addr_head), con, connections);

	sockfacts_free(facts);
}

struct wgotw_session *wgotw_session_init()
{
	session = malloc(sizeof(struct wgotw_session));
	session->pid = getpid();
	size_t len = snprintf(NULL, 0, WGOTW_SESSION_FMT, session->pid) + 1;
	session->dir = malloc(len);
	snprintf(session->dir, len, WGOTW_SESSION_FMT, session->pid);

	mkdir(session->dir, 0777);

	SLIST_INIT(&(session->addr_head));

	return session;
}

void wgotw_session_free()
{
	free(session->dir);

	struct wgotw_connection *con;
	while (!SLIST_EMPTY(&(session->addr_head))) {
		con = SLIST_FIRST(&(session->addr_head));
		SLIST_REMOVE_HEAD(&(session->addr_head), connections);
		connection_free(con);
	}

	free(session);
}

void wgotw_record_outgoing(int sockfd, void *buf, size_t len)
{
	ensure_session_connection(sockfd);

	printf("outgoing: got %zu bytes\n", len);
}

void wgotw_record_incoming(int sockfd, void *buf, size_t len)
{
	ensure_session_connection(sockfd);

	printf("incoming: got %zu bytes\n", len);
}

