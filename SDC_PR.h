/**
 * Professor RIP application
 * @file SDC_PRF.h
 * @author Jean Arthur Ousmane
 * @version 1.0.5
 * @date 5 mars. 2016
 */

#ifndef SDC_PR

#define SDC_PR
#define PORT 1234
#define CHAT_PORT 1235
#define SERVER "localhost"
#define BUFSIZE 1024
#define color(param) printf("\033[36m%s\033[0m\n",param)

/**
 * Creates a socket, prepare its address and connect to the server.
 * @return int The socket descriptor.
 */
static int sock_connect();

/**
 * Reads a message from a socket
 * @param sock The socket from which one read the message
 * @param[out] buffer The buffer to store the message
 */
static void sock_read(int sock, char * buffer);

/**
 * Sends a message from a socket
 * @param sock The socket from which one write the message
 * @param[in] buffer The buffer which contains the message to send
 */
static void sock_write(int sock, const char * buffer);


/**
 * Create a socket for UDP Protocol
 * @param[out] The prepared address
 * @return The socket descriptor
 */
static int udp_connect(struct sockaddr_in * adr);

/**
 * Sends the buffer contents from the socket
 * @param sock The socket descriptor
 * @param[out] adresseReceveur A pointer to the receiver's address
 * @param[out] buffer The message received
 * @param[out] lgadresseReceveur A pointer to the receiver's address size
 */
static void receive_message(int sock, struct sockaddr_in *adresseReceveur, char *buffer, socklen_t * lgadresseReceveur);

/**
 * Sends a message to the professor
 * @param sock The socket descriptor
 * @param[in] adresseReceveur The receiver's address
 * @param[in] buffer The message to send
 * @param lgadresseReceveur The address size
 */
static void send_message(int sock, struct sockaddr_in *adresseReceveur, const char *buffer, socklen_t lgadresseReceveur);

#endif