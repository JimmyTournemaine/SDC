#define PORT 1234
#define BUFSIZE 1024
#define PASSFILE ".passwords"

/**
 * Create and bind a socket
 * @param sin The adress
 * @return The socket descriptor
 */
int sock_create(struct sockaddr_in * sin);

/**
 * Get the <login>+<encrypted_password> to authenticate
 * @param sock The socket from which one read the message
 * @param buffer To store ids
 * @param from The address of the sender
 * @param fromsize The size of the sender's adress
 */
void read_client(int sock, char * buffer, struct sockaddr_in * from, socklen_t * fromsize);

/**
 * Send a "boolean" to the client represents the success of the authentification
 * @param sock The socket from which one write the authentication success or not
 * @param pval The address of the "boolean" to send
 * @param from The address of the client
 * @param fromsize The size of the clients' address
 */
void write_client(int sock, int * pval, struct sockaddr_in * from, socklen_t fromsize);

/**
 * Check if ids exists
 * @param buffer The structured string
 * @return 0|1 If succeed or not
 */
int auth(FILE * stream, char * buffer);