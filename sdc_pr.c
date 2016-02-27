/* Lancement d'un client :  client_tcp port serveur */
/* compilation : gcc -o client_tcp client_tcp.c   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define TAILLE 1024
#define PORT 12345
#define SERVER "localhost"

int main (int argc, char **argv)
{
    int sock;
    char buffer[TAILLE];
    char rep[5];
    struct sockaddr_in adresse_serveur;
    struct hostent *hote;
    unsigned short port;
    int val;
    int n;
    
    
    
   /* creation de la socket locale */
if ((sock = socket(AF_INET, SOCK_STREAM,0))==-1)
	{
  	perror("socket");
  	exit(1);
	}

/* recuperation de l'adresse IP du serveur (a partir de son nom) */
if ((hote = gethostbyname(argv[2]))==NULL)
	{
  	perror("gethostbyname");
  	exit(2);
	}

/* preparation de l'adresse du serveur */
port = (unsigned short) atoi(argv[1]);

adresse_serveur.sin_family = AF_INET;
adresse_serveur.sin_port = htons(port);
bcopy(hote->h_addr, &adresse_serveur.sin_addr, hote->h_length);

printf("L'adresse en notation pointee %s\n", inet_ntoa((struct in_addr) adresse_serveur.sin_addr));
fflush(stdout);

/* demande de connexion au serveur */
if (connect(sock, (struct sockaddr *) &adresse_serveur, sizeof(adresse_serveur))==-1 )
	{
  	perror("connect");
  	exit(3);
	}

/* le serveur a accepte la connexion */
printf("connexion acceptee\n");
fflush(stdout);

    
    
    
    /*préciser que c'est le professeur qui connecte*/
    strcpy(buffer, "prof");
    if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
    
    
    printf("Appuyer sur une touche quand vous serez prêt à démarrer le cours\n");   
    while(1)
    {
	fd_set rfds;
        FD_ZERO(&rfds);
        
        /* ajouter la saisie clavier */
        FD_SET(STDIN_FILENO, &rfds);
        
        /* ajouter la socket */
        FD_SET(sock, &rfds);
        
        if(select(sock + 1, &rfds, NULL, NULL, NULL) == -1)
        {
            perror("select()");
            exit(errno);
        }
        
        
        /*attendre la socket*/
        if(FD_ISSET(sock, &rfds))
        {
            /*recevoir la liste des étudiants déjà connectés*/
            if((n = recv(sock, buffer, sizeof buffer, 0)) < 0)
            {
                perror("recv()");
                exit(errno);
            }
            printf("Liste des étudiants connectés %s\n", buffer);
            fflush(stdout);
        }
        
        
        /*attendre la saisie clavier*/
        else if(FD_ISSET(STDIN_FILENO, &rfds))
        {
             break;
        }
        
    }
    
 
do{
	 /*recevoir la liste des exercices*/
    if((n = recv(sock, buffer, sizeof buffer, 0)) < 0)
    {
        perror("recv()");
        exit(errno);
    }
    printf("Liste des exercices %s\n", buffer);
    fflush(stdout);
    
    /*choix de l'exercice à faire*/
    printf("Choisissez le nom d'un exercice sur la liste\n");
    scanf("%s",buffer);
    if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
    printf("Nom de l'exercice envoyé\n");
    fflush(stdout); 
    
    
    /*réception des réponses*/
    do
    {
        if((n = recv(sock, buffer, sizeof buffer, 0)) < 0)
        {
            perror("recv()");
            exit(errno);
        }
        printf("%s\n", buffer);
        fflush(stdout);
    }
    while(strcmp(buffer,"finished")!=0);
    
printf("Voulez-vous donner un autre exercice ?(oui/non)\n");
scanf("%s",rep);
}
while(strcmp(rep,"oui")==0); 


/*fin du cours*/ 
strcpy(buffer, "end");
if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
    printf("Fin du cours\n");
    fflush(stdout); 

   
/* fermeture de la socket */
    close (sock);
    fflush(stdout);

return 0;
}


