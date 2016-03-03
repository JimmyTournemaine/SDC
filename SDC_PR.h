/**
 * Professor RIP application
 * @file SDC_PRF.h
 * @author Jean Arthur Ousmane
 * @version 1.0
 * @date 26 feb. 2016
 */

#define PORT 1234
#define CHAT_PORT 1235
#define SERVER "localhost"
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


// Se connecter en UPD
static int udp_connect(struct sockaddr_in *);

// Envoyer le contenu de buffer depuis la socket
static void receive_message(int sock, struct sockaddr_in *adresseReceveur, char *buffer, socklen_t *);

// Recevoir un message depuis la socket et le stocker dans buffer
static void send_message(int sock, struct sockaddr_in *adresseReceveur, const char *buffer, socklen_t);
