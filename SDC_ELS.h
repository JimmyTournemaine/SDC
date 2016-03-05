/**
 * Student RIP application
 * @file SDC_ELS.h
 * @author Jimmy Tournemaine <jimmy.tournemaine@yahoo.fr>
 * @version 1.0.1
 * @date 5 mar. 2016
 */
#ifndef SDC_ELS

#define SDC_ELS
#define PORT 1234
#define SERVER_NAME "localhost"
#define BUFSIZE 1024
#define clear() printf("\033[H\033[2J");


/********************* SA Authentification *********************/

/**
 * Prepares a socket (UDP)
 * @param[out] to The addresse to send messages
 * @return The socket descriptor
 */
static int sock_create(struct sockaddr_in * to);

/**
 * Writes authentification message to the authentication service (SA)
 * @param sock The socket descriptor
 * @param[in] buffer The message to send
 * @param[in] to A pointer to the address to send the message
 * @param tosize The size of the destination address
 */
static void sendID(int sock, char * buffer, struct sockaddr_in * to, socklen_t tosize);

/**
 * Reads the authentication status from the authentication service
 * @param sock The socket descriptor
 * @param[out] val The adress where store the authentication status
 * @param[out] to A pointer to the sender's address
 * @param[out] tosize A pointer to the size of the sender's address
 */
static void getAuthenticationResponse(int sock, int * val, struct sockaddr_in * to, socklen_t * tosize);

/**
 * Asks a password
 * @param[out] The input password
 *
 * This function modify terminal properties to put in "non-echo" in order to conceal the entry of a password.
 * Then it recover the defaults properties.
 */
static void ask_password(char * password);

/**
 * Crypts a password
 * @param[in,out] The clear password - the encrypted password
 */
static void crypt_password(char * buffer);

/*********************** SDC Connection ************************/

/**
 * Creates a socket, prepare its address and connect to the server.
 * @return int The socket descriptor.
 */
static int sock_connect();

/**
 * Sends the student's login to the server
 * @param sock The socket from which one sends the login
 * @param[in] login The login to send
 */
static void send_login(int sock, const char * login);

/**
 * Read a message from a socket
 * @param sock The socket from which one read the message
 * @param[out] buffer The buffer to store the message
 */
static void read_message(int sock, char * buffer);

/**
 * Send an answer from a socket
 * @param sock The socket from which one send the answer
 * @param[in] panswer A pointer to the integer answer of the exercise
 */
static void send_exercise_answer(int sock, int * panswer);

#endif