#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include "SDC_PR.h"

int main(void)
{
    char buffer[BUFSIZE] = "prof";
    char c;
    struct sockaddr_in adr;
    socklen_t sizeadr = sizeof adr;
    
    /* Chat connection */
    int socket = udp_connect(&adr);
    
    /* Server connection */
    int sock = sock_connect();
    
    /* Sends "login" */
    strcpy(buffer, "prof");
    sock_write(sock, buffer);
    

    fd_set rdfs;
    printf("Press enter to start class.\n");
    
    /* ------------------------ Wait Students -----------------*/
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
        
        /* Start class */
        if(FD_ISSET(STDIN_FILENO, &rdfs))
        {
            strcpy(buffer, "start");
            sock_write(sock, buffer);
            break;
        }
        /* Print new connected student  */
        else if(FD_ISSET(sock, &rdfs))
        {
            sock_read(sock, buffer);
            printf("%s", buffer);
        }
    }
    
    fflush(stdout);
    
    /* --------------- Send exercises and get students answers --------------- */
    while(1)
    {
        /* Select and exercise to send to the students */
        sock_read(sock, buffer);
        printf("%s\n", buffer);
        
        printf("Select an exercise : ");
        //fflush(stdout);
        scanf("%s", buffer);
        
        do { // Purge stdin for fgets
            c = getchar();
        } while (c != EOF && c != '\n');
        
        /* Exit condition */
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        
        /* Send the exercise name */
        sock_write(sock, buffer);
        
        printf("--------------------------------------------------------------\n");
        
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
            
            /* Answer / End of the exercise */
            if(FD_ISSET(sock, &rdfs))
            {
                sock_read(sock, buffer);
                if (explodeAnswer(buffer, 0) == 1){
                    break;
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
        
        printf("Exercise done.\n--------------------------------------------------------------\n");
        printf("Press enter for the next exercise.");
        do {
            c = getchar();
        } while (c != EOF && c != '\n');
    }
    
    close(sock);
}

/* static int explodeAnswer(char * buffer, ssize_t offset)
{
    int i;
    char temp[BUFSIZE];
    
    if(strncmp(buffer+offset, "finished", 8) == 0) {
        return 1;
    }
    
    for (i=offset; i<strlen(buffer); i++) {
        if (buffer[i] >= 48 && buffer[i] <= 57) {
            break;
        }
    }
    strncpy(temp, buffer+offset, i+1);
    
    color(temp);
    
    if(i < strlen(buffer)) {
        return explodeAnswer(buffer, i+1);
    }
    
    return 0;
} */

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
