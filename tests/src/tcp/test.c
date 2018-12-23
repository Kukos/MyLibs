#include <tcp.h>
#include <test.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

#define MIN_PORT  6000
#define RANGE     2000

test_f test_create_socket(void)
{
    const int port = MIN_PORT + rand() % RANGE;
    int socket;

    socket = tcp_socket_create("localhost", port);
    T_CHECK(socket != -1);

    tcp_socket_destroy(socket);
}

test_f test_connection_request(void)
{
    const int port = MIN_PORT + rand() % RANGE;
    int socket;
    int fd;
    int pid;

    pid = fork();
    if (pid == 0) /* child accept */
    {
        socket = tcp_socket_create("localhost", port);
        T_CHECK(socket != -1);
        fd = tcp_accept_connection(socket);
        T_CHECK(fd != -1);
        close(fd);
        tcp_socket_destroy(socket);
        _Exit(0);
    }
    else /* parent request */
    {
        socket = tcp_request_connection("localhost", port);
        T_CHECK(socket != -1);
        tcp_socket_destroy(socket);
    }
}

test_f test_accept_connection(void)
{
    const int port = MIN_PORT + rand() % RANGE;
    int socket;
    int fd;
    int pid;

    pid = fork();
    if (pid == 0) /* child request */
    {
        socket = tcp_request_connection("localhost", port);
        T_CHECK(socket != -1);
        tcp_socket_destroy(socket);
        _Exit(0);
    }
    else /* parent accept */
    {
        socket = tcp_socket_create("localhost", port);
        T_CHECK(socket != -1);
        fd = tcp_accept_connection(socket);
        T_CHECK(fd != -1);
        close(fd);
        tcp_socket_destroy(socket);
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

    pid = fork();
    if (pid == 0) /* child accept */
    {
        socket = tcp_socket_create("localhost", port);
        T_CHECK(socket != -1);

        fd = tcp_accept_connection(socket);
        T_CHECK(fd != -1);

        listen(fd, 1);

        /* prepare */
        FD_ZERO(&set);
        FD_SET(fd, &set);

        /* wait for msg */
        copy = set;
        T_CHECK(select(fd + 1, &copy, NULL, NULL, NULL) >= 0);
        T_EXPECT(FD_ISSET(fd, &copy), true);

        unused_retval(read(fd, buffer, sizeof(buffer)));
        T_EXPECT(strcmp(buffer, text), 0);

        usleep(100);
        /* write msg */
        unused_retval(write(fd, text2, sizeof(text2)));
        usleep(100);

        close(fd);
        tcp_socket_destroy(socket);
        _Exit(0);
    }
    else /* parent request */
    {
        socket = tcp_request_connection("localhost", port);
        T_CHECK(socket != -1);

        /* prepare */
        fd = socket;
        listen(fd, 1);
        FD_ZERO(&set);
        FD_SET(fd, &set);

        copy = set;
        usleep(100);
        unused_retval(write(fd, text, sizeof(text)));

        /* wait for msg */
        T_CHECK(select(fd + 1, &copy, NULL, NULL, NULL) >= 0);
        T_EXPECT(FD_ISSET(fd, &copy), true);

        unused_retval(read(fd, buffer, sizeof(buffer)));
        T_EXPECT(strcmp(buffer, text2), 0);

        usleep(100);
        tcp_socket_destroy(socket);
    }
}

void test(void)
{
    TEST(test_create_socket());
    TEST(test_connection_request());
    TEST(test_accept_connection());
    TEST(test_full_connection());
}

int main(void)
{
    TEST_INIT("TCP");
    test();
    TEST_SUMMARY();
}