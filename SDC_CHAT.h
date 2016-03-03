/**
 * Chat RIP application
 * @file SDC_ELS.h
 * @author Jean-Arthur Ousmane
 * @version 1.0
 * @date 2 mar. 2016
 */
#ifndef SDC_CHAT

#define SDC_CHAT
#define BUFSIZE 1024
#define SERVER "localhost"
#define PORT 1235

/**
 * UDP Connection
 * @param[out] The address
 * @return The socket descriptor
 */
static int udp_connect(struct sockaddr_in *);

/**
 * Receives a message of the professor
 * @param sock The socket descriptor
 * @param[out] adresseReceveur A pointer to the address
 * @param[out] buffer The buffer where store the received message
 * @param[out] lgadresseReceveur A pointer to the address size
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