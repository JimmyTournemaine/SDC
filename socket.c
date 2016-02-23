#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "socket.h"

int sock_connect(int port, const char * server_name)
{
    int sock;
    struct hostent *host;
    struct sockaddr_in server_addr;
    
    /* creation de la socket locale */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(-1);
    }
    
    /* recuperation de l'adresse IP du serveur (a partir de son nom) */
    if ((host = gethostbyname(server_name)) == NULL)
    {
        perror("gethostbyname");
        exit(-2);
    }
    
    /* preparation de l'adresse du serveur */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((unsigned short) port);
    bcopy(host->h_addr, &server_addr.sin_addr, host->h_length);


    /* Connexion au serveur */
    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr))==-1)
    {
        perror("connect");
        exit(-3);
    }
    
    return sock;
}