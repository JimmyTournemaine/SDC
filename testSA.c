#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

#define PORT 1234
#define SERVER_NAME "localhost"
#define BUFSIZE 1024

int sock_create(struct sockaddr_in * to)
{
    int sock;
    struct hostent *host = NULL;
    
    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket()");
        exit(-1);
    }
    
    if ((host = gethostbyname(SERVER_NAME)) == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", SERVER_NAME);
        exit(-2);
    }
    
    bcopy(host->h_addr, &(to->sin_addr), host->h_length);
    to->sin_port = PORT;
    to->sin_family = AF_INET;
    
    printf("port : %d\n", PORT);
    printf("port : %d\n", htons(PORT));
    
    return sock;
}

void sock_write(int sock, char * buffer, struct sockaddr_in * to, socklen_t tosize)
{
    if(sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&to, tosize) < strlen(buffer))
    {
        perror("sendto()");
        exit(-3);
    }
}

void sock_read(int sock, char * resp, struct sockaddr_in * to, socklen_t * tosize)
{
    if(recvfrom(sock, resp, 1, 0, (struct sockaddr *)&to, tosize) < 1)
    {
        perror("recvfrom()");
        exit(-4);
    }
}

void ask_password(char * password)
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
    
    printf("password: ");
    scanf("%s", password);
    password[strlen(password)] = 0;
    
    /* Restore terminal */
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    struct sockaddr_in addr;
    socklen_t addr_size  = sizeof addr_size;
    int sock = sock_create(&addr);
    char login[255];
    char password[255];
    char rep;
    
    printf("Login : ");
    scanf("%s", login);
    
    //ask_password(password);
    
    sock_write(sock, login, &addr, addr_size);
    printf("login envoyé !\n");
    /*sock_write(sock, password, &addr, addr_size);
    printf("password envoyé !\n");
    
    sock_read(sock, &rep, &addr, &addr_size);
    
    if (rep) {
        printf("Access granted\n");
    } else {
        printf("Access denied\n");
    }*/
    
    close(sock);
}


