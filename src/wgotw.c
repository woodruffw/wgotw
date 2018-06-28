#define WGOTW_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>

#include "util.h"
#include "wgotw.h"

__attribute__((constructor (101))) void wgotw_init()
{
    wgotw_session_init();

    DEBUG("%s", "wgotw_init()");
}

__attribute__((destructor (101))) void wgotw_deinit()
{
    wgotw_session_free();

    DEBUG("%s", "wgotw_deinit()");
}

static struct wgotw_connection *connection_init(struct sockfacts *facts)
{
    struct wgotw_connection *con = malloc(sizeof(struct wgotw_connection));
    int addr_len = strlen(facts->address);

    con->address = malloc(addr_len + 1);
    memcpy(con->address, facts->address, addr_len);
    con->address[addr_len] = '\0';
    con->port = facts->port;
    con->count = 0;

    char *addr_dir;
    asprintf(&addr_dir, WGOTW_SESSION_FMT "/%s", session->pid, con->address);
    mkdir(addr_dir, 0777);

    asprintf(&(con->dir), "%s/%d", addr_dir, con->port);
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

static struct wgotw_connection *ensure_session_connection(int sockfd)
{
    struct sockfacts *facts = sockfacts_init(sockfd);

    struct wgotw_connection *con;
    SLIST_FOREACH(con, &(session->addr_head), connections) {
        if (con->port == facts->port && !strcmp(con->address, facts->address)) {
            sockfacts_free(facts);
            return con;
        }
    }

    con = connection_init(facts);
    SLIST_INSERT_HEAD(&(session->addr_head), con, connections);

    sockfacts_free(facts);
    return con;
}

void wgotw_session_init()
{
    session = malloc(sizeof(struct wgotw_session));
    session->pid = getpid();

    asprintf(&(session->dir), WGOTW_SESSION_FMT, session->pid);

    session->opts.verbose = getenv("WGOTW_VERBOSE") ? 1 : 0;
    session->opts.debug = getenv("WGOTW_DEBUG") ? 1 : 0;

    SLIST_INIT(&(session->addr_head));

    /* actually place the session on disk */
    mkdir(session->dir, 0777);

    DEBUG("session info:\n"
        "\tpid: %d\n"
        "\tdir: %s\n"
        "\tverbose: %d\n"
        "\tdebug: %d\n",
        session->pid, session->dir, session->opts.verbose, session->opts.debug);
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

void wgotw_record(int type, int sockfd, const void *buf, size_t len)
{
    struct wgotw_connection *con = ensure_session_connection(sockfd);

    char *fmt;
    switch (type) {
        case BUFFER_INBOUND:
            fmt = "inbound";
            break;
        case BUFFER_OUTBOUND:
            fmt = "outbound";
            break;
        default:
            fmt = "unknown";
    }

    char *fname;
    asprintf(&fname, "%s/%s.%ld", con->dir, fmt, con->count);

    FILE *dump = fopen(fname, "w");
    fwrite(buf, 1, len, dump);

    fclose(dump);
    free(fname);

    con->count++;
}

