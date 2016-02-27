/**
 * Student RIP application
 * @file SDC_ELS.h
 * @author Jean Arthur Ousmane
 * @author Jimmy Tournemaine <jimmy.tournemaine@yahoo.fr>
 * @version 1.0
 * @date 23 feb. 2016
 */

#define PORT 1234
#define MAXSTUDENTS 2
#define BUFSIZE 1024
#define LOGINSIZE 16
#define BASEDIR "/Users/jimmytournemaine/Documents/Dossiers Professionels/Cours/Documents/Travail/Reseau/Projet/exercices"

typedef struct {
    int sock;
    char login[LOGINSIZE];
} Client;


/**
 * Create a socket, bind it to the port and listen connections.
 * @param port Server port where bind the socket.
 * @return int The socket descriptor.
 */
static int socket_listen(int port);

/**
 * Wait a message from a socket and copy the content in the buffer
 * @param sock The socket from which read the message
 * @param[out] The buffer where copy the message
 * @return The number of 
 */
static int read_client(int sock, char * buffer);

/**
 * Wait an integer as the answer of a student for an exercise
 * @param sock The socket from which get the answer
 * @param answer[out] A pointer to where store the answer
 */
static int read_answer(int sock, int * answer);

static void write_client(int sock, const char * buffer);
static void send_connected_students(int sock, const Client * clients, int nbClients);
static void send_to_all_students(Client * students, int nbStudents, const char * buffer);

static void remove_client(Client *clients, int to_remove, int *actual);

static void findExercices(char * buffer);
static int getExercise(char * buffer);