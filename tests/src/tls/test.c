#include <tls.h>
#include <tcp.h>
#include <test.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define MIN_PORT  6000
#define RANGE     2000

#define key_file ".key.pem"
#define cert_file ".cert.pem"

static ___before_main___(1) void init(void);
static ___after_main___(1) void deinit(void);

static int verify_callback(int preverify, X509_STORE_CTX* ctx);

static ___before_main___(1) void init(void)
{
    (void)system("echo $'\n\n\n\n\n\n\n\n' | openssl req -x509 -newkey rsa:4096 -nodes -keyout " key_file " -out " cert_file " -days 365 >/dev/null 2>&1");
}

static ___after_main___(1) void deinit(void)
{
    (void)system("rm -rf " key_file " " cert_file);
}

static int verify_callback(int preverify, X509_STORE_CTX* ctx)
{
    (void)preverify;
    (void)ctx;

    return 1;
}

test_f test_tls_init(void)
{
    /* this line will crash if something went wrong, so test without assert */
    tls_init();

    tls_clenup();
}

test_f test_client_context(void)
{
    SSL_CTX *ctx;

    tls_init();

    ctx = tls_client_context_create(cert_file, key_file, NULL);
    T_CHECK(ctx != NULL);

    tls_context_destroy(ctx);

    ctx = tls_client_context_create(cert_file, key_file, verify_callback);
    T_CHECK(ctx != NULL);

    tls_context_destroy(ctx);

    tls_clenup();
}

test_f test_client_context_with_err(void)
{
    SSL_CTX *ctx;

    tls_init();

    ctx = tls_client_context_create(cert_file, NULL, NULL);
    T_CHECK(ctx == NULL);

    ctx = tls_client_context_create(NULL, key_file, verify_callback);
    T_CHECK(ctx == NULL);

    tls_clenup();
}

test_f test_server_context(void)
{
    SSL_CTX *ctx;

    tls_init();

    ctx = tls_server_context_create(cert_file, key_file, NULL);
    T_CHECK(ctx != NULL);

    tls_context_destroy(ctx);

    ctx = tls_server_context_create(cert_file, key_file, verify_callback);
    T_CHECK(ctx != NULL);

    tls_context_destroy(ctx);

    tls_clenup();
}

test_f test_server_context_with_err(void)
{
    SSL_CTX *ctx;

    tls_init();

    ctx = tls_server_context_create(cert_file, "fake.pem", NULL);
    T_CHECK(ctx == NULL);

    ctx = tls_server_context_create("fake.pem", key_file, verify_callback);
    T_CHECK(ctx == NULL);

    tls_clenup();
}

test_f test_connection_request(void)
{
    const int port = MIN_PORT + rand() % RANGE;
    int socket;
    int fd;
    int pid;
    SSL_CTX *ctx;
    SSL *ssl;

    pid = fork();
    if (pid == 0) /* child accept */
    {
        tls_init();
        ctx = tls_server_context_create(cert_file, key_file, verify_callback);
        T_CHECK(ctx != NULL);

        socket = tcp_socket_create("localhost", port);
        T_CHECK(socket != -1);
        fd = tcp_accept_connection(socket);
        T_CHECK(fd != -1);

        ssl = tls_accept(ctx, fd);
        T_CHECK(ssl != NULL);

        tls_ssl_destroy(ssl);
        tls_context_destroy(ctx);

        close(fd);
        tcp_socket_destroy(socket);

        tls_clenup();
        _Exit(0);
    }
    else /* parent request */
    {
        tls_init();
        ctx = tls_client_context_create(cert_file, key_file,verify_callback);
        T_CHECK(ctx != NULL);

        socket = tcp_request_connection("localhost", port);
        T_CHECK(socket != -1);

        ssl = tls_request_connection(ctx, socket);
        T_CHECK(ssl != NULL);

        tls_ssl_destroy(ssl);
        tls_context_destroy(ctx);

        tcp_socket_destroy(socket);

        tls_clenup();
    }
}

test_f test_connection_accept(void)
{
    const int port = MIN_PORT + rand() % RANGE;
    int socket;
    int fd;
    int pid;
    SSL_CTX *ctx;
    SSL *ssl;

    pid = fork();
    if (pid == 0) /* child request */
    {
        tls_init();
        ctx = tls_client_context_create(cert_file, key_file,verify_callback);
        T_CHECK(ctx != NULL);

        socket = tcp_request_connection("localhost", port);
        T_CHECK(socket != -1);

        ssl = tls_request_connection(ctx, socket);
        T_CHECK(ssl != NULL);

        tls_ssl_destroy(ssl);
        tls_context_destroy(ctx);

        tcp_socket_destroy(socket);

        tls_clenup();

        _Exit(0);
    }
    else /* parent accept */
    {
        tls_init();
        ctx = tls_server_context_create(cert_file, key_file, verify_callback);
        T_CHECK(ctx != NULL);

        socket = tcp_socket_create("localhost", port);
        T_CHECK(socket != -1);
        fd = tcp_accept_connection(socket);
        T_CHECK(fd != -1);

        ssl = tls_accept(ctx, fd);
        T_CHECK(ssl != NULL);

        tls_ssl_destroy(ssl);
        tls_context_destroy(ctx);

        close(fd);
        tcp_socket_destroy(socket);

        tls_clenup();
    }
}

test_f test_full_connection(void)
{
    char buffer[1024] = {0};
    const char text[] = "Kukos";
    const char text2[] = "C Master";
    const int port = MIN_PORT + rand() % RANGE;

    int socket;
    int fd;
    int pid;
    fd_set set;
    fd_set copy;

    SSL_CTX *ctx;
    SSL *ssl;

    pid = fork();
    if (pid == 0) /* child accept */
    {
        tls_init();
        ctx = tls_server_context_create(cert_file, key_file, verify_callback);
        T_CHECK(ctx != NULL);

        socket = tcp_socket_create("localhost", port);
        T_CHECK(socket != -1);

        fd = tcp_accept_connection(socket);
        T_CHECK(fd != -1);

        listen(fd, 1);

        ssl = tls_accept(ctx, fd);
        T_CHECK(ssl != NULL);

        /* prepare */
        FD_ZERO(&set);
        FD_SET(fd, &set);

        /* wait for msg */
        copy = set;
        T_CHECK(select(fd + 1, &copy, NULL, NULL, NULL) >= 0);
        T_EXPECT(FD_ISSET(fd, &copy), true);

        SSL_read(ssl, buffer, sizeof(buffer));
        T_EXPECT(strcmp(buffer, text), 0);

        usleep(100);
        /* write msg */
        SSL_write(ssl, text2, sizeof(text2));
        usleep(100);

        tls_ssl_destroy(ssl);
        tls_context_destroy(ctx);

        close(fd);
        tcp_socket_destroy(socket);

        tls_clenup();
        _Exit(0);
    }
    else /* parent request */
    {
        tls_init();
        ctx = tls_client_context_create(cert_file, key_file,verify_callback);
        T_CHECK(ctx != NULL);

        socket = tcp_request_connection("localhost", port);
        T_CHECK(socket != -1);

        /* prepare */
        fd = socket;
        listen(fd, 1);
        FD_ZERO(&set);
        FD_SET(fd, &set);

        ssl = tls_request_connection(ctx, fd);
        T_CHECK(ssl != NULL);

        copy = set;
        usleep(100);
        SSL_write(ssl, text, sizeof(text));

        /* wait for msg */
        T_CHECK(select(fd + 1, &copy, NULL, NULL, NULL) >= 0);
        T_EXPECT(FD_ISSET(fd, &copy), true);

        SSL_read(ssl, buffer, sizeof(buffer));
        T_EXPECT(strcmp(buffer, text2), 0);

        usleep(100);
        tls_ssl_destroy(ssl);
        tls_context_destroy(ctx);

        tcp_socket_destroy(socket);

        tls_clenup();
    }
}

void test(void)
{
    TEST(test_tls_init());
    TEST(test_client_context());
    TEST(test_client_context_with_err());
    TEST(test_server_context());
    TEST(test_server_context_with_err());
    TEST(test_connection_request());
    TEST(test_connection_accept());
    TEST(test_full_connection());
}

int main(void)
{
    TEST_INIT("TLS");
    test();
    TEST_SUMMARY();
}