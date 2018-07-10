#ifndef TLS_LIB_H
#define TLS_LIB_H

/*
    Simple library for TLS connection

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <openssl/ssl.h>

/*
    Init openssl for TLS connection
    Do it only once per program

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void tls_init(void);

/*
    Clean openssl for TLS connection
    Do it only once per program

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void tls_clenup(void);

/*
    Create TLS context for server
    1 context can has a few connections

    PARAMS
    @IN cert - path to cert file
    @IN key - path to priv key file
    @IN verify_callback - callback to verify certs or NULL (use built in callback)

    RETURN
    NULL iff failure
    Pointer to new configured context
*/
SSL_CTX *tls_server_context_create(const char *cert, const char *key, int (*verify_callback)(int preverify, X509_STORE_CTX* ctx));


/*
    Create TLS context for client
    1 context can has a few connections

    PARAMS
    @IN cert - path to cert file
    @IN key - path to priv key file
    @IN verify_callback - callback to verify certs or NULL (use built in callback)

    RETURN
    NULL iff failure
    Pointer to new configured context
*/
SSL_CTX *tls_client_context_create(const char *cert, const char *key, int (*verify_callback)(int preverify, X509_STORE_CTX* ctx));

/*
    Destroy TLS context

    PARAMS
    @IN ctx - tls context

    RETURN
    This is a void function
*/
void tls_context_destroy(SSL_CTX *ctx);

/*
    Accept new TLS connection, from fd to context

    PARAMS
    @IN ctx - tls context where connection will be added
    @IN fd - connection file descriptor

    RETURN
    NULL iff cannot accept
    NEW pointer to SSL connection iff success
*/
SSL *tls_accept(SSL_CTX *ctx, int fd);

/*
    Request for TLS connectio, from fd to context

    PARAMS
    @IN ctx - tls context where connection will be added
    @IN fd - connection file descriptor

    RETURN
    NULL iff failure
    NEW pointer to SSL connection iff success
*/
SSL *tls_request_connection(SSL_CTX *ctx, int fd);

/*
    Destroy SSL

    PARAMS
    @IN ssl - ssl

    RETURN
    This is a void function
*/
void tls_ssl_destroy(SSL *ssl);

#endif