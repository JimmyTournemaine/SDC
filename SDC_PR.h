/**
 * Professor RIP application
 * @file SDC_PRF.h
 * @author Jean Arthur Ousmane
 * @version 1.0
 * @date 26 feb. 2016
 */

#define PORT 1234
#define SERVER_NAME "localhost"
#define BUFSIZE 1024

/**
 * Create a socket, prepare its address and connect to the server.
 *
 * @param port Server port.
 * @param server_name The server name.
 * @return int The socket descriptor.
 */
static int sock_connect();

/**
 * Read a message from a socket
 * @param sock The socket from which one read the message
 * @param buffer The buffer to store the message
 */
static void sock_read(int sock, char * buffer);

/**
 * Send a message from a socket
 * @param sock The socket from which one write the message
 * @param buffer The buffer wich contains the message to send
 */
static void sock_write(int sock, const char * buffer);