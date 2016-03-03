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
#include <errno.h>
#include "SDC_CHAT.h"

int main(void)
{
    
    struct sockaddr_in adresseReceveur;
    socklen_t lgadresseReceveur = sizeof adresseReceveur;
    
    char buffer[BUFSIZE];
    fd_set rdfs;
    
    int sock = udp_connect(&adresseReceveur);
    
    while(1){
        
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);
        
        
        if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(EXIT_FAILURE);
        }
        
        
        /*réception de la réponse du professeur*/
        if(FD_ISSET(STDIN_FILENO, &rdfs))
        {
            fgets(buffer, BUFSIZE, stdin);
            buffer[strlen(buffer)-1] = '\0';
            if(strcmp(buffer,"")==0)
            {
                break;
            }
            send_message(sock, &adresseReceveur, buffer, lgadresseReceveur);
            
        }
        
        /*envoie du message*/
        else if(FD_ISSET(sock, &rdfs))
        {
       	    receive_message(sock, &adresseReceveur, buffer, &lgadresseReceveur);
            printf("%s\n", buffer);
            fflush(stdout);
            
        }
        
    }
    
    printf("Goodbye !\n");
    close(sock);
    return 0;
}


// Se connecter en UPD
static int udp_connect(struct sockaddr_in * adresseReceveur)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        fprintf(stderr, "Cannot create socket.\n");
        exit(EXIT_FAILURE);
    }
    
    
    struct hostent *hostinfo = gethostbyname(SERVER);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", SERVER);
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    adresseReceveur->sin_addr = *(struct in_addr *) hostinfo->h_addr;
    adresseReceveur->sin_port = htons(PORT);
    adresseReceveur->sin_family = AF_INET;
    
    return sock;
}



// Envoyer le contenu de buffer depuis la socket
static void send_message(int sock, struct sockaddr_in *adresseReceveur, const char *buffer, socklen_t lgadresseReceveur)
{
    printf("J'envoie : %s\n", buffer);
    if (sendto(sock,buffer,BUFSIZE,0,(struct sockaddr *)adresseReceveur, sizeof(*adresseReceveur)) < 0)
    {
        fprintf(stderr, "Cannot send message.\n");
        exit(1);
    }
    
}

// Recevoir un message depuis la socket et le stocker dans buffer
static void receive_message(int sock, struct sockaddr_in *adresseReceveur, char *buffer, socklen_t * lgadresseReceveur)
{
    if (recvfrom(sock,buffer,BUFSIZE,0,(struct sockaddr *)adresseReceveur,lgadresseReceveur) == -1)
    {
        fprintf(stderr, "Cannot receive message.\n");
        exit(1);
    }
}

