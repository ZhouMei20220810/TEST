/*
 * connection.h
 *
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <common/common.h>
#include <common/socket_helper.h>

AWE_BEGIN_DECLS

typedef enum ConnType{
    CONN_STREAM = 1,
    CONN_DGRAM,
}ConnType;

typedef enum af_t{
    STREAM            = SOCK_STREAM,
    DGRAM            = SOCK_DGRAM,

    STREAM_V6        = SOCK_STREAM,
    DGRAM_V6        = SOCK_DGRAM,
}af_t;

typedef struct connection connection_t;

/*
 * Create a connection.
 * @return 0: success,
 *          < 0: failed.
 */
AWE_DECLARE(connection_t*) connection_stream(int af, int fd,
        struct sockaddr* addr, socklen_t addrlen, int use_ssl, uint32_t id);
AWE_DECLARE(connection_t*) connection_stream2(int af, int use_ssl, uint32_t id);

AWE_DECLARE(connection_t*) connection_dgram(int af, int fd, uint32_t id);
AWE_DECLARE(connection_t*) connection_dgram2(int af, uint32_t id);


AWE_DECLARE(connection_t*) connection_addref(connection_t *conn);

/**
 * Destroy the connection_t variable and free the associated memory.
 * @param conn the connection_t variable to destroy.
 */
AWE_DECLARE(void) connection_autorelease(connection_t **conn);

/**
 * Open connection
 * @return 0: The connection is pending
 *           1: local connection connected
 *           > 1: Connection had opened
 *           < 0: error
 */
AWE_DECLARE(status_t) connection_open(connection_t *conn, const char *host, uint16_t port, uint16_t bindPort);

AWE_DECLARE(status_t) connection_open2(connection_t *conn, struct sockaddr *addr, socklen_t addrlen, uint16_t bindPort);

/*
 * Close connection
 */
AWE_DECLARE(status_t) connection_close(connection_t *conn);

AWE_DECLARE(status_t) connection_ssl_connect(connection_t *conn, bool sync);

AWE_DECLARE(status_t) connection_ssl_handshake(connection_t *conn, int timeout);

AWE_DECLARE(int) connection_ssl_pending(connection_t *conn);

/**
 * Read bytes from the connection.
 *
 * @param buffer    Pointer to destination buffer.
 * @param size       Number of bytes to read.
 * @return Number of bytes read.
 *             < 0 if error.
 */
AWE_DECLARE(int) connection_read(connection_t *conn,
        void *buffer, size_t size);

AWE_DECLARE(int) connection_read2(connection_t *conn,
        void *buffer, size_t size, struct sockaddr *fromAddress);

/**
 * Write bytes to the connection.
 *
 * @param buffer    Pointer to source buffer.
 * @param size       Number of bytes to read.
 * @return Number of bytes written.
 *             < 0 if error.
 */
AWE_DECLARE(int) connection_write(connection_t *conn,
        const void *buffer, size_t size);

AWE_DECLARE(int) connection_write2(connection_t *conn,
        const char *host, uint16_t port, const void *buffer, size_t size);

AWE_DECLARE(void) connection_setid(connection_t *conn, uint32_t id);

AWE_DECLARE(int) connection_fd(connection_t *conn);
AWE_DECLARE(int) connection_af(connection_t *conn);

AWE_DECLARE(struct sockaddr*) connection_addr(connection_t *conn);
AWE_DECLARE(socklen_t) connection_addrlen(connection_t *conn);

AWE_DECLARE(status_t) connection_localHost(connection_t *conn, char *buf, int buflen);
AWE_DECLARE(uint16_t) connection_localPort(connection_t *conn);

AWE_DECLARE(status_t) connection_remoteHost(connection_t *conn, char* host, int len);
AWE_DECLARE(uint16_t) connection_remotePort(connection_t *conn);

AWE_END_DECLS

#endif /* CONNECTION_H_ */
