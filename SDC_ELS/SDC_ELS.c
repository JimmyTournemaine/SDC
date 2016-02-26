#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "SDC_ELS.h"

int main(void)
{
    char buffer[BUFSIZE];
    char login[16];
    int sock, answer;
    
    sock = sock_connect();
    
    printf("Login : ");
    scanf("%s", login);
    send_login(sock, login);
    
    read_message(sock, buffer);
    printf("%s\n", buffer);
    
    while (1)
    {
        read_message(sock, buffer);
        printf("%s\n", buffer);
        
        printf("Your answer ? ");
        scanf("%d", &answer);
        
        send_exercise_answer(sock, &answer);
    }
}


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
        exit(EXIT_FAILURE);
    }
    
    return sock;
}


static void send_login(int sock, const char * login)
{
    if ((send(sock, login, sizeof(login), 0)) != sizeof(login))
    {
        fprintf(stderr, "Cannot send login.\n");
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
}

static void send_exercise_answer(int sock, int * panswer)
{
    ssize_t sent = send(sock, panswer, sizeof(int), 0);
    
    if (sent != sizeof(int)){
        fprintf(stderr, "Cannot send the answer.\n");
        exit(EXIT_FAILURE);
    }
}