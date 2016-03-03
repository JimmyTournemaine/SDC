#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "SDC_PR.h"

int main(void)
{
    char buffer[BUFSIZE] = "prof";
    char c;
    struct sockaddr_in adr;
    socklen_t sizeadr = sizeof adr;
    
    /*connexion UDP*/
    int socket = udp_connect(&adr);
    
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
        
        /* Purge stdin */
        do {
            c = getchar();
        } while (c != EOF && c != '\n');
        
        /* Send the exercise name */
        sock_write(sock, buffer);
        
        /* Get answers, disconnection message or end of the exercise */
        int max = (sock > socket) ? sock : socket;
        while(1) {
            
            FD_ZERO(&rdfs);
            FD_SET(sock, &rdfs);
            FD_SET(socket, &rdfs);
            
            if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
            {
                perror("select()");
                exit(EXIT_FAILURE);
            }
            
            /* Affichage réponses */
            if(FD_ISSET(sock, &rdfs))
            {
                sock_read(sock, buffer);
                if(strcmp(buffer, "finished") == 0)
                {
                    break;
                }
                else
                {
                    printf("%s\n", buffer);
                    fflush(stdout);
                }
            }
            
            /* Chat */
            else if(FD_ISSET(socket, &rdfs))
            {
                receive_message(socket,&adr,buffer, &sizeadr);
                printf("Une question vous est posée :\n%s\n", buffer);
                
                printf("Votre réponse : ");
                fgets(buffer, BUFSIZE, stdin);
                buffer[strlen(buffer)-1] = '\0';
                send_message(socket,&adr,buffer, sizeadr);
            }
        }
        
        printf("Exercise done.\n");
        
        sleep(3);
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
    if ((host = gethostbyname(SERVER)) == NULL)
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


// Se connecter en UPD
static int udp_connect(struct sockaddr_in * adresseReceveur)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        perror("socket()");
    }
    
    adresseReceveur->sin_addr.s_addr = htonl(INADDR_ANY);
    adresseReceveur->sin_family = AF_INET;
    adresseReceveur->sin_port = htons(CHAT_PORT);
    
    if(bind (sock, (struct sockaddr *) adresseReceveur, sizeof *adresseReceveur) == -1)
    {
        perror("bind()");
    }
    
    return sock;
}



// Envoyer le contenu de buffer depuis la socket
static void send_message(int sock, struct sockaddr_in *adresseReceveur, const char *buffer, socklen_t lgadresseReceveur)
{
    int n;
    if ((n = sendto(sock,buffer,BUFSIZE,0,(struct sockaddr *)adresseReceveur,lgadresseReceveur)) < 0)
    {
        fprintf(stderr, "Cannot send your message.\n");
    }
    
}

// Recevoir un message depuis la socket et le stocker dans buffer
static void receive_message(int sock, struct sockaddr_in *adresseReceveur, char *buffer, socklen_t * lgadresseReceveur)
{
    int recu;
    if ((recu = recvfrom(sock,buffer,BUFSIZE,0,(struct sockaddr *)adresseReceveur,lgadresseReceveur)) == -1)
    {
        fprintf(stderr, "Cannot receive message the student message.\n");
    }
}
