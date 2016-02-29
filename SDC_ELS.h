/**
 * Student RIP application
 * @file SDC_ELS.h
 * @author Jimmy Tournemaine <jimmy.tournemaine@yahoo.fr>
 * @version 1.0
 * @date 23 feb. 2016
 */

#define PORT 1234
#define SERVER_NAME "localhost"
#define BUFSIZE 1024


/********************* SA Authentification *********************/

/**
 * Prepare a socket (UDP)
 * @param[out] to The addresse to send messages
 * @return  The socket descriptor
 */
static int sock_create(struct sockaddr_in * to);

/**
 * Write authentification message to the authentication service
 * @param sock The socket descriptor
 * @param buffer The message to send
 * @param to The address to send the message
 * @param tosize The size of the destination address
 */
static void sendID(int sock, char * buffer, struct sockaddr_in * to, socklen_t tosize);

/**
 * Read the authentication status from the authentication service
 * @param sock The socket descriptor
 * @param val The adress where store the authentication status
 * @param to The sender address
 * @param tosize The size of the sender's address
 */
static void getAuthenticationResponse(int sock, int * val, struct sockaddr_in * to, socklen_t * tosize);

/**
 * Ask a password
 * @param[out] The input password
 *
 * This function modify terminal properties to put in "non-echo" in order to conceal the entry of password.
 * Then it recover the defaults properties.
 */
static void ask_password(char * password);

/**
 * Crypt a password
 * @param[in,out] The clear password that become a encrypt password
 */
static void crypt_password(char * buffer);

/*********************** SDC Connection ************************/

/**
 * Create a socket, prepare its address and connect to the server.
 *
 * @param port Server port.
 * @param server_name The server name.
 * @return int The socket descriptor.
 */
static int sock_connect();

/**
 * Send the student login to the server
 * @param sock The socket from which one sends the login
 * @param login The login to send
 */
static void send_login(int sock, const char * login);

/**
 * Read a message from a socket
 * @param sock The socket from which one read the message
 * @param buffer The buffer to store the message
 */
static void read_message(int sock, char * buffer);

/**
 * Send an answer from a socket
 * @param sock The socket from which one send the answer
 * @param panswer A pointer to the integer answer of the exercise
 */
static void send_exercise_answer(int sock, int * panswer);