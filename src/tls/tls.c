#include <tls.h>
#include <openssl/err.h>
#include <log.h>
#include <openssl/engine.h>
#include <openssl/conf.h>

/*
    Create tls context for server / client (based on get_method function)

    PARAMS
    @IN cert - path to cert file
    @IN key - path to key file
    @IN verify_callback - callback
    @IN get_method - get CTX_METHOD

    RETURN
    NULL iff failure
    New SSL_CTX iff success
*/
static SSL_CTX *__tls_context_create(const char *cert, const char *key, int (*verify_callback)(int preverify, X509_STORE_CTX* ctx), const SSL_METHOD *(*get_method)(void));

/*
    Private Verify Callback, used when used dont want to specify own

    This function is only wrapper for normal verify, thats log some values
    and return preverify

    PARAMS
    @IN preverify - ssl preverify
    @IN ctx - pointer to ctx

    RETURN
    Preverify status
*/
static int __verify_callback(int preverify, X509_STORE_CTX* ctx);

static int __verify_callback(int preverify, X509_STORE_CTX* ctx)
{
    char   buf[256];
    X509   *cert;
    int    depth;

    TRACE();
    cert = X509_STORE_CTX_get_current_cert(ctx);
    depth = X509_STORE_CTX_get_error_depth(ctx);

    X509_NAME_oneline(X509_get_subject_name(cert), buf, 256);

    LOG("Preverify %d, depth=%d:%s\n", preverify, depth, buf);

    return preverify;
}

static SSL_CTX *__tls_context_create(const char *cert, const char *key, int (*verify_callback)(int preverify, X509_STORE_CTX* ctx), const SSL_METHOD *(*get_method)(void))
{
    SSL_CTX *ctx;
    const SSL_METHOD *method;

    TRACE();

    if (cert == NULL)
        ERROR("cert == NULL\n", NULL);

    if (key == NULL)
        ERROR("key == NULL\n", NULL);

    LOG("Getting SSL metod\n");
    method = get_method();
    ctx = SSL_CTX_new(method);
    if (ctx == NULL)
        ERROR("SSL_CTX_new error\n", NULL);

    LOG("Getting cert from %s and key from %s\n", cert, key);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM) <= 0)
    {
        SSL_CTX_free(ctx);
        ERROR("SSL_CTX_use_cert error\n", NULL);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM) <= 0)
    {
        SSL_CTX_free(ctx);
        ERROR("SSL_CTX_use_priv_key error\n", NULL);
    }

    if (SSL_CTX_check_private_key(ctx) != 1)
    {
        SSL_CTX_free(ctx);
        ERROR("cert %s is not valid with priv key %s\n", NULL, cert, key);
    }

    SSL_CTX_set_verify_depth(ctx, 0);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback ? verify_callback : __verify_callback);

    LOG("TLS context created and configured\n");

    return ctx;
}

void tls_init(void)
{
    TRACE();

    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    OpenSSL_add_all_algorithms();

    LOG("SSL libarry inited\n");
}

void tls_clenup(void)
{
    TRACE();

    FIPS_mode_set(0);
    CRYPTO_cleanup_all_ex_data();
    CONF_modules_unload(1);
    EVP_cleanup();
    ERR_remove_state(0);
    ERR_free_strings();
    ENGINE_cleanup();
    SSL_COMP_free_compression_methods();

    LOG("SSL libary cleaned\n");
}

SSL_CTX *tls_server_context_create(const char *cert, const char *key, int (*verify_callback)(int preverify, X509_STORE_CTX* ctx))
{
    return __tls_context_create(cert, key, verify_callback, SSLv23_server_method);
}

SSL_CTX *tls_client_context_create(const char *cert, const char *key, int (*verify_callback)(int preverify, X509_STORE_CTX* ctx))
{
    return __tls_context_create(cert, key, verify_callback, SSLv23_method);
}

void tls_context_destroy(SSL_CTX *ctx)
{
    if (ctx == NULL)
        return;

    SSL_CTX_free(ctx);
}

SSL *tls_accept(SSL_CTX *ctx, int fd)
{
    SSL *ssl;

    ssl = SSL_new(ctx);
    if (ssl == NULL)
        ERROR("SSL_new error\n", NULL);

    SSL_set_fd(ssl, fd);

    if (SSL_accept(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        ERROR("cannot accept tls connection from %d fd\n", NULL, fd);
    }

    LOG("TLS connection for %d fd accepted\n", fd);

    return ssl;
}

SSL *tls_request_connection(SSL_CTX *ctx, int fd)
{
    SSL *ssl;

    ssl = SSL_new(ctx);
    if (ssl == NULL)
        ERROR("SSL_new error\n", NULL);

    SSL_set_fd(ssl, fd);

    if (SSL_connect(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        ERROR("SSL_connect error\n", NULL);
    }

    LOG("TLS connection for %d fd done\n", fd);

    return ssl;
}

void tls_ssl_destroy(SSL *ssl)
{
    TRACE();

    if (ssl == NULL)
        return;

    SSL_free(ssl);
}
