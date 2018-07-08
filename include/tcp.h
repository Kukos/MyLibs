#ifndef TCP_H
#define TCP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
    Simple library for TCP unix sockets

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

/*
    Get socket for tcp connection using by server

    PARAMS
    @IN host_name - host name
    @IN port - port

    RETURN
    New binded Socket iff success
    -1 iff failure
*/
int tcp_socket_create(const char *host_name, int port);

/*
    Destroy socket (close fd)

    PARAMS
    @IN socket - socket

    RETURN
    This is a void function
*/
void tcp_socket_destroy(int socket);

/*
    Request new tcp connection from client to server

    PARAMS
    @IN host_name - host name
    @IN port - port

    RETURN
    New Socket for tcp iff success
    -1 iff failure
*/
int tcp_request_connection(const char *host_name, int port);

/*
    Accept new tcp connection from socket s

    PARAMS
    @IN s - socket

    RETURN
    New fd binded with tcp on socket s iff success
    -1 iff failure
*/
int tcp_accept_connection(int s);

#endif