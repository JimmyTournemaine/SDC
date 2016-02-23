#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "socket.h"

#define PORT 1234
#define SERVER "tpreseaux"
#define TMPFILE "/tmp/src_els_exercise"
#define BUFSIZE 1024

int main(void)
{
    char attempts = 0;
    char buffer[BUFSIZE];
    int socket, fd, response = 0;
    struct student el;
    ssize_t size;
    
    /* Server connection */
    socket = sock_connect(PORT, SERVER);
    
    /* Student authentification */
    do {
        tries++;
        
        // login/password
        el = student_id();
        
        // send data
        if ((send(sock, &el, sizeof(el), 0)) != sizeof(el))
        {
            fprintf(stderr, "Cannot send authentification request.\n");
            exit(-1);
        }
        
        // success ?
        if (recv(sock, &response, sizeof(int), 0) == -1) {
            fsprintf(stderr, "An error occurred during reception of authentification message.\n");
            exit(-2);
        }
        if (response == 0) {
            printf("Sorry, try again.\n");
        }

    } while (response != 1 || tries < 3);
    
    if (response == 0) {
        printf("3 incorrect password attempts\n");
        exit(1);
    }
    printf("authentication successful\n");
    
    /* Prepare exercise file */
    if ((fd = open(TMPFILE, O_WRONLY | O_CREAT | O_TRUNC, 0640)) == -1)
    {
        fprintf(stderr, "Cannot open or create a temporary file at %s\n", TMPFILE);
        exit(-3);
    }
    
    /* Wait course starting */
    if (recv(sock, NULL, 0, 0) == -1) {
        fprintf(stderr, "Course starting signal reception error.\n");
        exit(-4);
    }
    
    while (1)
    {
        /* Size to receive */
        if(recv(sock, &size, sizeof(size), 0) == -1) {
            fprintf(stderr, "Cannot receive the size of the file to read");
            exit(-5);
        }
        
        /* Notify the server that we are waiting for the exercise */
        if ((send(sock, "ok", 2, 0)) != 2)
        {
            fprintf(stderr, "Cannot notify server that we are waiting for the exercise file");
            exit(-6);
        }
        
        /* Get the exercise */
      	if((n=read(socket_service,buf,BUFSIZE)) != size)
        {
            fprintf(stderr, "The file is too large, you need to increase the buffer size");
            exit(-7);
        }
        
        /* Print exercice */
        printf("%s\n", size);
        
        printf("Your answer ? ");
        scanf("%d", &response);
        
        /* Send answer */
        if ((send(sock, &rep, sizeof(int), 0)) != sizeof(int))
        {
            fprintf(stderr, "Cannot send the answer.\n");
            exit(-8);
        }
    }
}