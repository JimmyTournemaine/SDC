/**
 * Use sockets easily
 *
 * @file product.h
 * @author Jimmy Tournemaine <jimmy.tournemaine@yahoo.fr>
 * @version 1.0
 * @date 23 feb. 2016
 *
 * Some utilities to use sockets easily.
 */

/**
 * Create a socket, prepare its address and connect to the server
 *
 * @param port Server port.
 * @param server_name The server name.
 * @return int The socket descriptor.
 */
int sock_connect(int port, const char * server_name);