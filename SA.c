#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include "SA.h"

int main(void)
{
    struct sockaddr_in sin;
    int sock = sock_create(&sin);
    char buffer[1024];
    int val;
    FILE * fd;
    
    while (1)
    {
        /* Open file */
        if((fd = fopen(PASSFILE, "r")) == NULL) {
            fprintf(stderr, "Cannot open/read password file.\n");
            exit(-1);
        }
        
        struct sockaddr_in from = { 0 };
        socklen_t fromsize = sizeof from;
        
        /* Read login - password */
        read_client(sock, buffer, &from, &fromsize);
        val = auth(fd, buffer);
        
        /* Send password validation */
        write_client(sock, &val, &from, fromsize);
        
        /* Close file */
        fclose(fd);
    }
    
    /* Close socket */
    close(sock);
}

int sock_create(struct sockaddr_in * sin)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    
    sin->sin_addr.s_addr = htonl(INADDR_ANY);
    sin->sin_family = AF_INET;
    sin->sin_port = htons(PORT);
    
    if(bind (sock, (struct sockaddr *) sin, sizeof *sin) == -1)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
    
    return sock;
}

void read_client(int sock, char * buffer, struct sockaddr_in * from, socklen_t * fromsize)
{
    int n;
    if((n = recvfrom(sock, buffer, BUFSIZE-1, 0, (struct sockaddr *)from, (socklen_t *)fromsize)) < 0)
    {
        perror("recvfrom()");
        exit(EXIT_FAILURE);
    }
    
    buffer[n] = '\0';
}

void write_client(int sock, int * pval, struct sockaddr_in * from, socklen_t fromsize)
{
    if(sendto(sock, pval, sizeof(int), 0, (struct sockaddr *)from, fromsize) < 0)
    {
        perror("sendto()");
        exit(EXIT_FAILURE);
    }
}

int auth(FILE * stream, char * buffer)
{
    char line[BUFSIZE], c;
    int i=0;
    while ((c = fgetc(stream)) != EOF)
    {
        /* Si fin de la ligne */
        if (c == '\n')
        {
            line[i++] = 0;
            /* Si OK */
            if (strcmp(line, buffer) == 0) {
                return 1;
            }
            /* Sinon nouvelle ligne */
            else {
                i=0;
                continue;
            }
        }
        line[i++] = c;
    }
    
    return 0;
}