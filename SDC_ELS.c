#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <termios.h>
#include "SDC_ELS.h"

int main(void)
{
    struct sockaddr_in to;
    int sock = sock_create(&to);
    
    char buffer[BUFSIZE];
    char login[255];
    char password[255];
    int accessGranted = 0, attempts = 3, answer;
    socklen_t tosize = sizeof to;
    
    /* -------------- Authentification ------------- */
    do {
        printf("Login : ");
        scanf("%s", login);
        
        ask_password(password);
        crypt_password(password);
        
        /* Sending */
        sprintf(buffer, "%s+%s", login, password);
        sendID(sock, buffer, &to, tosize);
        
        /* Get response */
        getAuthenticationResponse(sock, &accessGranted, &to, &tosize);
        if (accessGranted) {
            break;
        } else {
            printf("Sorry, try again.\n");
        }
        
        attempts--;
    } while (attempts > 0 && accessGranted != 1);
    
    close(sock);
    
    if (attempts == 0) {
        printf("3 incorrect password attempts\n");
        exit(EXIT_SUCCESS);
    }
    
    /* ----------- TCP Connection & class -----------*/
    
    /* TCP Connection */
    sock = sock_connect();
    
    /* Send login */
    send_login(sock, login);
    
    /* Print response */
    read_message(sock, buffer);
    printf("%s\n", buffer);
    fflush(stdout);
    
    /* If too many connection, the response start by '>' */
    if (buffer[0] == '>') {
        close(sock);
        exit(0);
    }
    
    while (1)
    {
        /* Print exercise */
        read_message(sock, buffer);
        printf("%s\n", buffer);
        
        /* Send answer */
        printf("Your answer ? ");
        fflush(stdout);
        
        scanf("%d", &answer);
        send_exercise_answer(sock, &answer);
        
        /* Clears console */
        clear();
    }
    
    close(sock);
}


/*************** TCP PART ********************/
static int sock_connect()
{
    int sock;
    struct hostent *host;
    struct sockaddr_in server_addr;
    
    /* local socket creation */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    /* Get host */
    if ((host = gethostbyname(SERVER_NAME)) == NULL)
    {
        perror("gethostbyname");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    /* Prepare the server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    bcopy(host->h_addr, &server_addr.sin_addr, host->h_length);
    
    
    /* Server connection */
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr))==-1)
    {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    return sock;
}


static void send_login(int sock, const char * login)
{
    if ((send(sock, login, sizeof(login), 0)) != sizeof(login))
    {
        fprintf(stderr, "Cannot send login.\n");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

static void read_message(int sock, char * buffer)
{
    ssize_t n = read(sock,buffer,BUFSIZE);

    if(n == -1) {
        fprintf(stderr, "Cannot read a message.\n");
        exit(EXIT_FAILURE);
    }
    if (n == 0) {
        printf("Your professor has leaving. The class is over.\n");
        close(sock);
        exit(EXIT_SUCCESS);
    }
    
    buffer[n] = '\0';
}

static void send_exercise_answer(int sock, int * panswer)
{
    ssize_t sent = send(sock, panswer, sizeof(int), 0);
    
    if (sent != sizeof(int)){
        fprintf(stderr, "Cannot send the answer.\n");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

/***************** UDP Authentification *********************/

static int sock_create(struct sockaddr_in * to)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    
    
    struct hostent *hostinfo = gethostbyname(SERVER_NAME);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", SERVER_NAME);
        exit(EXIT_FAILURE);
    }
    
    to->sin_addr = *(struct in_addr *) hostinfo->h_addr;
    to->sin_port = htons(PORT);
    to->sin_family = AF_INET;
    
    return sock;
}

static void sendID(int sock, char * buffer, struct sockaddr_in * to, socklen_t tosize)
{
    if(sendto(sock, buffer, BUFSIZE, 0, (struct sockaddr *)to, tosize) < 0)
    {
        perror("sendto()");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

static void getAuthenticationResponse(int sock, int * val, struct sockaddr_in * to, socklen_t * tosize)
{
    int n;
    if((n = recvfrom(sock, val, sizeof(int), 0, (struct sockaddr *) to, (socklen_t *)tosize)) < 0)
    {
        perror("recvfrom()");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

static void ask_password(char * password)
{
    struct termios oflags, nflags;
    
    /* Disabling echo */
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
    
    printf("Password: ");
    scanf("%s", password);
    password[strlen(password)] = 0;
    
    /* Restore terminal */
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

static void crypt_password(char * buffer)
{
    for (int i=0; i < strlen(buffer); i++) {
        if (i % 2 == 0) {
            buffer[i]++;
        } else {
            buffer[i]--;
        }
    }
}
