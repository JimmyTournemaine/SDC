#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include "SDC_PRF.h"

int main(void)
{
    char buffer[BUFSIZE] = "prof";
    int count = 0;
    
    /* Server connection */
    int sock = sock_connect();
    strcpy(buffer, "prof");
    sock_write(sock, buffer);
    
    /* Keep wait students or start class */
    fd_set rdfs;
    
    printf("Press enter to start class.\n");
    
    while(1)
    {
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);
        
        if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(EXIT_FAILURE);
        }
        
        /* Something from standard input */
        if(FD_ISSET(STDIN_FILENO, &rdfs))
        {
            /* Notify that we want start class */
            strcpy(buffer, "start");
            sock_write(sock, buffer);
            break;
        }
        /* A student is connected  */
        else if(FD_ISSET(sock, &rdfs))
        {
            sock_read(sock, buffer);
            printf("%s", buffer);
        }
    }
    
    
    /* Le cours commence */
    while(1)
    {
        /* Get exercises list */
        sock_read(sock, buffer);
        printf("%s\n", buffer);
        
        /* Let professor choose the exercise he wants */
        printf("Select an exercise : ");
        fflush(stdout);
        scanf("%s", buffer);
        
        /* Send the exercise name */
        sock_write(sock, buffer);
        
        /* Get answers, disconnection message or end of the exercise */
        while(1) {
            sock_read(sock, buffer);
            if(strcmp(buffer, "finished") == 0){
                break;
            } else {
                printf("%s\n", buffer);
                fflush(stdout);
            }
        }
        
        printf("Exercise done.\n");
        fflush(stdout);
    }
    
    close(sock);
}

static void sock_write(int sock, const char * buffer)
{
    if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(3);
    }
}

static void sock_read(int sock, char * buffer)
{
    ssize_t n = read(sock,buffer,BUFSIZE);
    
    if(n == -1) {
        fprintf(stderr, "Cannot read a message.\n");
        exit(4);
    }
    
    buffer[n] = '\0';
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