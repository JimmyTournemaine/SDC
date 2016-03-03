/**
 * Authentification service of RIP application
 * @file SA.h
 * @author Jimmy Tournemaine <jimmy.tournemaine@yahoo.fr>
 * @version 1.0
 * @date 27 feb. 2016
 */

#ifndef SA

#define SA
#define PORT 1234
#define BUFSIZE 1024
#define PASSFILE ".passwords"

/**
 * Creates and binds a socket
 * @param[out] sin A pointer to the adress
 * @return The socket descriptor
 */
int sock_create(struct sockaddr_in * sin);

/**
 * Gets the <login>+<encrypted_password> to authenticate a student
 * @param sock The socket from which one read the message
 * @param[out] buffer To store the string
 * @param[out] from A pointer to the address of the sender
 * @param[out] fromsize A pointer to the size of the sender's address
 */
void read_client(int sock, char * buffer, struct sockaddr_in * from, socklen_t * fromsize);

/**
 * Sends a "boolean" to the client represents the success of the authentification
 * @param sock The socket from which one write the authentication success or not
 * @param[in] pval The address of the "boolean" to send
 * @param[in] from A pointer to the address of the receiver
 * @param fromsize The size of the receiver's address
 */
void write_client(int sock, int * pval, struct sockaddr_in * from, socklen_t fromsize);

/**
 * Checks if ids exists
 * @param[in] stream The stream which contains encrypted passwords
 * @param[out] buffer The structured string
 * @return 0|1 If succeed or not
 */
int auth(FILE * stream, char * buffer);

#endif