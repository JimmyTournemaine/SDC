/**
 * Server RIP application
 * @file SDC_ELS.h
 * @author Jimmy Tournemaine <jimmy.tournemaine@yahoo.fr>
 * @version 1.0.1
 * @date 5 mar. 2016
 */
#ifndef SCD

#define SDC
#define PORT 1234
#define MAXSTUDENTS 20
#define BUFSIZE 1024
#define LOGINSIZE 16
#define BASEDIR "exercices"

typedef struct {
    int sock;
    char login[LOGINSIZE];
} Client;


/**
 * Create a socket, bind it to the port and listen connections.
 * @return int The socket descriptor.
 */
static int socket_listen();

/**
 * Waits for a message from a socket and copy the content in the buffer
 * @param sock The socket from which read the message
 * @param[out] The buffer where copy the message
 * @return The number of 
 */
static int read_client(int sock, char * buffer);

/**
 * Waits for an integer as the answer of a student for an exercise
 * @param sock The socket from which get the answer
 * @param answer[out] A pointer to where store the answer
 */
static int read_answer(int sock, int * answer);

/**
 * Sends a message to a client from a socket
 * @param sock The socket descriptor
 * @param[in] buffer The message to send 
 */
static void write_client(int sock, const char * buffer);

/**
 * Sends a message to all students
 * @param[in] students The connected students list
 * @param The number of connected students
 * @param[in] buffer The message to send
 */
static void send_to_all_students(Client * students, int nbStudents, const char * buffer);

/**
 * Removes a client when it disconnects
 * @param[in,out] The list of clients (students)
 * @param to_remove The index in the list of the client to remove
 * @param[in,out] The number of connected clients.
 *
 */
static void remove_client(Client *clients, int to_remove, int *actual);

/**
 * Finds exercises to send next on the server
 * @param[out] buffer The exercises found on the server.
 */
static void findExercices(char * buffer);

/**
 * Gets an exercise
 * @param[in,out] The exercise filename - The exercise text
 * @return 1|0 A boolean if exercise was opened and read
 */
static int getExercise(char * buffer);



#endif