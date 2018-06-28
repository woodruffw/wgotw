#ifndef WGOTW_H
#define WGOTW_H

#include <sys/queue.h>

/**
 * @brief The format for WGOtW session directories.
 * @details When WGOtW creates a new session, it will create it under
 * a directory with a name of this format. The number used will be the
 * process id of the running program.
 *
 */
#define WGOTW_SESSION_FMT "wgotw-%d"

#ifdef WGOTW_MAIN
struct wgotw_session *session;
#else
extern struct wgotw_session *session;
#endif

/**
 * @brief Writes a verbose message to stderr.
 * @details If WGOtW has been invoked with `WGOTW_VERBOSE=1`, then
 * this will print out informational messages. Otherwise, nothing will
 * be printed.
 *
 */
#define VERBOSE(fmt, ...) \
    do { \
        if (session->opts.verbose) { \
            fprintf(stderr, "[+] " fmt "\n", __VA_ARGS__); \
        } \
    } while(0)

/**
 * @brief Writes a debug message to stderr.
 * @details If WGOtW has been invoked with `WGOTW_DEBUG=1`, then this
 * will print out debugging messages. Otherwise, nothing will be printed.
 */
#define DEBUG(fmt, ...) \
    do { \
        if (session->opts.debug) { \
            fprintf(stderr, "[!] " fmt "\n", __VA_ARGS__); \
        } \
    } while(0)

/**
 * Enumerates the buffer types recognized by WGOtW.
 */
enum WGOTW_BUFFER_TYPE {
    /**
     * Inbound buffers.
     */
    BUFFER_INBOUND,

    /**
     * Outbound buffers.
     */
    BUFFER_OUTBOUND,

    /**
     * Unknown (other) buffers.
     */
    BUFFER_UNKNOWN,
};

/**
 * @brief Global options recognized by WGOtW.
 * @details These options are populated in wgotw_session_init(), and are
 * *generally* obtained through environment variables passed to the program
 * that WGOtW is hooked into.
 */
struct wgotw_options {
    /**
     * @brief Whether or not to print verbose messages.
     *
     * See VERBOSE()
     */
    int verbose;

    /**
     * @brief Whether or not to print debug messages.
     *
     * See DEBUG()
     */
    int debug;
};

/**
 * @brief Encapsulates WGOtW's runtime state.
 * @details wgotw_session keeps track of the hooked PID, session directory,
 * runtime options, and a list of all connections seen so far.
 */
struct wgotw_session {
    /**
     * @brief The PID of the process what WGOtW is hooked into.
     */
    int pid;

    /**
     * @brief The (relative) name of WGOtW's session directory.
     * @details Example: `wgotw-1234`
     */
    char *dir;

    /**
     * @brief The wgotw_options associated with the session.
     */
    struct wgotw_options opts;

    /**
     * @brief The list of all connections recorded by the session.
     * @details Each connection is encapsulated in a wgotw_connection.
     */
    SLIST_HEAD(, wgotw_connection) addr_head;
};

/**
 * @brief Encapsulates a connection recorded by the WGOtW session.
 * @details wgotw_connection keeps track of the directory that socket buffers
 * saved into, the address (IPv4 or IPv6) of the remote host, the connection's
 * port, and a tail pointer to the rest of the connections in the session.
 */
struct wgotw_connection {
    /**
     * @brief The (relative) name of the connection's directory.
     * @details Example: `wgotw-1234/127.0.0.1/80`
     */
    char *dir;

    /**
     * @brief The IPv4 or IPv6 address of the remote host.
     */
    char *address;

    /**
     * @brief The connection's port.
     */
    int port;

    /**
     * @brief The number of messages sent and received so far.
     * @details This number is used to save buffers as unique files, e.g.
     * `inbound.1`, `inbound.2`, `outbound.3`.
     */
    long count;

    /**
     * @brief The remaining connections in the list.
     */
    SLIST_ENTRY(wgotw_connection) connections;
};

/**
 * @brief Creates the WGOtW session for the hooked process.
 * @details This populates the global ::session variable.
 */
void wgotw_session_init();

/**
 * @brief Destroys the WGOtW session.
 * @details This is called during process teardown, and renders ::session
 * useless.
 */
void wgotw_session_free();

/**
 * @brief Record a buffer sent over the given socket.
 *
 * @param type The ::WGOTW_BUFFER_TYPE of the buffer
 * @param sockfd The socket associated with the buffer
 * @param buf The buffer
 * @param len The length of the buffer, in bytes
 */
void wgotw_record(int type, int sockfd, const void *buf, size_t len);

#endif
