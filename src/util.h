#ifndef WGOTW_UTIL_H
#define WGOTW_UTIL_H

/**
 * @brief Represents some basic "facts" about a socket.
 */
struct sockfacts {
    /**
     * @brief The destination address (IPv4 or IPv6) associated with the socket.
     */
    char *address;

    /**
     * @brief The port associated with the socket.
     */
    int port;
};

/**
 * @brief Creates a new ::sockfacts from the given socket file descriptor.
 * @param sockfd The socket to inspect
 * @return A pointer to the new ::sockfacts structure.
 */
struct sockfacts *sockfacts_init(int sockfd);

/**
 * @brief Destroys the given ::sockfacts structure.
 * @details This should only be called on ::sockfacts structures that were
 * created through sockfacts_init().
 *
 * @param facts A pointer to the ::sockfacts to destroy.
 */
void sockfacts_free(struct sockfacts *facts);

#endif
