#define PORT 1234
#define SERVER_NAME "localhost"
#define TMPFILE "/tmp/src_els_exercise"
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
 * Send the student login to the server
 * @param sock The socket from which one sends the login
 * @param login The login to send
 */
static void send_login(int sock, const char * login);

/**
 * Read an exercise from a socket
 * @param sock The socket from which one read the exercise
 * @param buffer The buffer to store exercise
 */
static void read_message(int sock, char * buffer);

/**
 * Send an answer from a socket
 * @param sock The socket from which one send the answer
 * @param panswer A pointer to the integer answer of the exercise
 */
static void send_exercise_answer(int sock, int * panswer);