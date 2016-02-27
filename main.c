/* Lancement d'un client :  client_tcp port serveur */
/* compilation : gcc -o client_tcp client_tcp.c   */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "sdc_pr.h"



static void app(const char *address, const char *name)
{
   int n;
   SOCKET sock = init_connection(address);
   char buffer[BUF_SIZE];
   char rep[5];

   fd_set rdfs;

   /*le professeur entre son nom*/
   write_server(sock, name);
    
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
            n = read_server(sock, buffer);
	    if(n==0)
	    {
		 printf("Tous les étudiants sont connectés\n");
                 break;
	    }
            printf("Liste des étudiants connectés %s\n", buffer);
            fflush(stdout);
        }
        
        
        /*attendre la saisie clavier pour démarrer le cours*/
        else if(FD_ISSET(STDIN_FILENO, &rfds))
        {
             break;
        }
        
    }
    
 
do
{	 /*recevoir la liste des exercices*/
     n = read_server(sock, buffer);
     if(n==0)
     {
	printf("La liste des exercices n'est pas disponible\n");
      }
    printf("Liste des exercices %s\n", buffer);
    fflush(stdout);
    
    /*choix de l'exercice à faire*/
    printf("Choisissez le nom d'un exercice sur la liste\n");
    scanf("%s",buffer);

    write_server(sock, buffer);
    printf("Nom de l'exercice envoyé\n");
    fflush(stdout); 
    
    
    /*réception des réponses*/
    do
    {
        n = read_server(sock, buffer);
        printf("%s\n", buffer);
        fflush(stdout);
    }
    while(n!=0);

    
    printf("Voulez-vous donner un autre exercice ?(oui/non)\n");
    scanf("%s",rep);
}

while(strcmp(rep,"oui")==0); 


     /*fin du cours*/ 
    strcpy(buffer, "end");

    write_server(sock, buffer);
    printf("Fin du cours\n");
    fflush(stdout); 

   
     /* fermeture de la socket */
    end_connection(sock);

}    
 

   
static int init_connection(const char *address)
{    
   /* creation de la socket locale */
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   /* recuperation de l'adresse IP du serveur (a partir de son nom) */
   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   /* preparation de l'adresse du serveur */
  
    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;


   /* demande de connexion au serveur */
   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }

   /* le serveur a accepte la connexion */
   printf("connexion acceptee\n");
   fflush(stdout);

return sock;
}    
  


static void end_connection(int sock)
{
   /* fermeture de la socket */
   closesocket(sock);
}



/*lecture sur la socket*/
static int read_server(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}


/*écriture sur la socket*/
static void write_server(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}



int main(int argc, char **argv)
{
   if(argc < 2)
   {
      printf("Usage : %s [address] [nom]\n", argv[0]);
      return EXIT_FAILURE;
   }


   app(argv[1], argv[2]);

   return EXIT_SUCCESS;
} 
  
    
   



