#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SDC.h"

int main(void)
{
    /* Server launching */
    int socketRV = socket_listen();
    printf("Server is started.\n");
    
    char buffer[BUFSIZE];
    int answer = 0;
    Client students[MAXSTUDENTS];
    Client prof;
    int nbStudents = 0, i;
    int pendingAnswers;
    int max = socketRV;
    
    fd_set rdfs;
    
    /* ----------------------- Step 1 : Connections ---------------------- */
    
    /* Waiting for connections */
    while (1)
    {
        /* Add the connection socket and professor socket */
        FD_ZERO(&rdfs);
        FD_SET(socketRV, &rdfs);
        FD_SET(prof.sock, &rdfs);
        
        /* Launch select */
        if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(-1);
        }
        
        /* Nouvelle connexion */
        if(FD_ISSET(socketRV, &rdfs))
        {
            struct sockaddr_in csin = { 0 };
            socklen_t sinsize = sizeof csin;
            int csock = accept(socketRV, (struct sockaddr *) &csin, &sinsize);
            if(csock == -1)
            {
                perror("accept()");
                continue;
            }
            
            /* After connecting the client sends its name */
            if(read_client(csock, buffer) == -1)
            {
                continue; // disconnected
            }
            
            /* what is the new maximum fd ? */
            max = csock > max ? csock : max;
            
            FD_SET(csock, &rdfs);
            
            Client c = { csock };
            strcpy(c.login, buffer);
            
            /* It is the professor */
            if (strcmp(c.login, "prof") == 0) {
                prof = c;
                for (i=0; i<nbStudents; i++) {
                    sprintf(buffer, "%s is connected.\n", students[i].login);
                    write_client(csock, buffer);
                }
                printf("The professor is connected\n");
                /* Is is a student */
            } else {
                if (nbStudents < MAXSTUDENTS)
                {
                    /* Add the student and send him a welcoming message */
                    students[nbStudents++] = c;
                    sprintf(buffer, "Welcome %s !\nYou will receive your first exercise soon !", c.login);
                    write_client(csock, buffer);
                    printf("%s is connected\n", c.login);
                    
                    /* Notify the professor */
                    if(prof.sock != 0) {
                        sprintf(buffer, "%s is now connected\n", c.login);
                        write_client(prof.sock, buffer);
                    }
                } else {
                    /* Too many students */
                    strcpy(buffer, "> They are too many students, you will be disconnected.");
                    write_client(csock, buffer);
                    close(csock);
                    printf("Connection refused : too many students (MAX = %d)", MAXSTUDENTS);
                }
            }
        }
        /* The professor wants to start now */
        else if(FD_ISSET(prof.sock, &rdfs))
        {
            fprintf(stderr, "Class is beginning.\n");
            read_client(prof.sock, buffer);
            break;
        }
    }
    
    fprintf(stderr, "Anyone will be accepted now.\n");
    
    /* Nobody can connect now */
    close(socketRV);
    
    /* ----------------------- Step 2 : Exercises ---------------------- */
    
    while (1) {
        
        /* Sends exercises list */
        findExercices(buffer);
        write_client(prof.sock, buffer);
        
        /* Gets exercise */
        if( read_client(prof.sock, buffer) == 0) {
            break;
        }
        
        if (getExercise(buffer))
        {
            /* Send exercise */
            send_to_all_students(students, nbStudents, buffer);
            
            /* Wait answers */
            pendingAnswers = nbStudents;
            
            while (pendingAnswers > 0)
            {
                FD_ZERO(&rdfs);
                for (i=0; i<nbStudents; i++) {
                    FD_SET(students[i].sock, &rdfs);
                }
                
                fprintf(stderr, "%d ", pendingAnswers);
                
                if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
                {
                    perror("select()");
                    exit(EXIT_FAILURE);
                }
                
                for (i=0; i<nbStudents; i++)
                {
                    if(FD_ISSET(students[i].sock, &rdfs))
                    {
                        Client student = students[i];
                        if(read_answer(students[i].sock, &answer) < sizeof(int))
                        {
                            /* student disconnected */
                            close(students[i].sock);
                            remove_client(students, i, &nbStudents);
                            strcpy(buffer, student.login);
                            strcat(buffer, " is disconnected !");
                            write_client(prof.sock, buffer);
                        }
                        else
                        {
                            sprintf(buffer, "%s answer : %d", student.login, answer);
                            write_client(prof.sock, buffer);
                        }
                        pendingAnswers--;
                    }
                    
                }
            }
            
            fprintf(stderr, "done.\n");
        }
        
        strcpy(buffer, "finished");
        write_client(prof.sock, buffer);
    }
    
    /* Close */
    printf("The professor has left.\n");
    close(prof.sock);
    for (i=0; i<nbStudents; i++) {
        close(students[i].sock);
    }
    printf("Bye.\n");
}

static int read_client(int sock, char * buffer)
{
    int n = 0;
    
    if((n = recv(sock, buffer, BUFSIZE, 0)) < 0)
    {
        perror("recv()"); // if recv error we disconnect clients
    }
    
    buffer[n] = 0;
    
    return n;
}

static int read_answer(int sock, int * answer)
{
    return recv(sock, answer, sizeof(int), 0);
}

static void write_client(int sock, const char * buffer)
{
    if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(1);
    }
}

static void send_to_all_students(Client * students, int nbStudents, const char * buffer)
{
    int i = 0;
    for (i=0; i<nbStudents; i++) {
        write_client(students[i].sock, buffer);
    }
}


static int socket_listen()
{
    int socket_RV;
    struct sockaddr_in adresseRV;
    int lgadresseRV;
    
    /* creation de la socket de RV */
    if ((socket_RV = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Cannot create the socket.\n");
        exit(-1);
    }
    
    /* preparation de l'adresse locale */
    adresseRV.sin_family = AF_INET;
    adresseRV.sin_port = htons(PORT);
    adresseRV.sin_addr.s_addr = htonl(INADDR_ANY);
    
    lgadresseRV = sizeof(adresseRV);
    
    /* attachement de la socket a l'adresse locale */
    if ((bind(socket_RV, (struct sockaddr *) &adresseRV, lgadresseRV)) == -1)
    {
        fprintf(stderr, "Cannot bind the socket to local address.\n");
        exit(-2);
    }
    
    /* declaration d'ouverture du service */
    if (listen(socket_RV, 3)==-1)
    {
        fprintf(stderr, "Cannot listen for connections on the socket.\n");
        exit(-3);
    }
    
    return socket_RV;
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
    /* we remove the client in the array */
    memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
    /* number client - 1 */
    (*actual)--;
}

static void findExercices(char * buffer)
{
    char list[256];
    DIR * dirp;
    struct dirent * entry;
    strcpy(buffer, "");
    
    /* Open directory */
    if((dirp = opendir(BASEDIR)) == NULL){
        fprintf(stderr, "Cannot open %s directory.\n", BASEDIR);
        exit(EXIT_FAILURE);
    }
    
    /* Get file names */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) {
            sprintf(list, "%s\n", entry->d_name);
            strcat(buffer, list);
        }
    }
    buffer[strlen(buffer)-1] = '\0';
    
    /* Close opened directory */
    closedir(dirp);
}

static int getExercise(char * buffer)
{
    int fd;
    size_t n;
    char pathname[BUFSIZE];
    
    sprintf(pathname, "%s/%s", BASEDIR, buffer);
    
    if ((fd = open(pathname, O_RDONLY)) == -1) {
        return 0;
    }
    if ((n = read(fd, buffer, BUFSIZE)) == -1) {
        close(fd);
        return 0;
    }
    buffer[n] = '\0';
    
    close(fd);
    
    return 1;
}


