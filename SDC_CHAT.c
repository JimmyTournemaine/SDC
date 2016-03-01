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
    SOCKET sock;
    char buffer[BUFSIZE];
    fd_set rdfs;
   
    sock = udp_connect();
    
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
	   scanf("%s",buffer);
	    if(strcmp(buffer,"fin")==0)
	    {
		break;
	    }
   	    send_message(sock, &adresseReceveur, buffer);
            
     }

      /*envoie du message*/
     else if(FD_ISSET(sock, &rdfs))
        {
       	    receive_message(sock, &adresseReceveur, buffer);
            printf("%s", buffer); 
	    fflush(stdout);
   	    
        }
      
}   

    printf("Fin de la discussion\n");

   /*ferméture de la socket*/ 
   close(sock);
    return 0;
}


// Se connecter en UPD
static int udp_connect()
{

  int sock;
  struct sockaddr_in adresseReceveur;
  int lgadresseReceveur;
  struct hostent *hote;

 
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
  	perror("socket"); 
  	exit(1);
  	}

  /* recherche de l'@ IP de la machine distante */
  if ((hote = gethostbyname(SERVER)) == NULL)
	{
  	perror("gethostbyname"); 
  	close(sock); 
  	exit(2);
  	}

  /* pr'eparation de l'adresse distante : port + la premier @ IP */
  adresseReceveur.sin_family = AF_INET;
  adresseReceveur.sin_port = htons(PORT);
  bcopy(hote->h_addr, &adresseReceveur.sin_addr, hote->h_length);
  printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseReceveur.sin_addr));

  lgadresseReceveur = sizeof(adresseReceveur);
if ((bind(sock,(struct sockaddr *)&adresseReceveur,lgadresseReceveur)) == -1) 
	{
  	perror("bind"); 
  	exit(1);
  	} 

   return sock;
}



// Envoyer le contenu de buffer depuis la socket
static void send_message(SOCKET sock, struct sockaddr_in *adresseReceveur, const char *buffer)
{
   int envoye;
  int lgadresseReceveur;
lgadresseReceveur = sizeof(struct sockaddr_in);
if ((envoye = sendto(sock,buffer,strlen(buffer)+1,0,(struct sockaddr *)adresseReceveur,lgadresseReceveur )) != strlen(buffer)+1) 
	{
  	
if (errno == EINVAL)
printf("erreur de taille\n");

if (errno == EBADF)
printf("erreur de socket\n"); 
  	close(sock); 
  	exit(1);
  	}
   
}

// Recevoir un message depuis la socket et le stocker dans buffer 
static void receive_message(SOCKET sock, struct sockaddr_in *adresseReceveur, char *buffer)
{
 int recu;
int lgadresseReceveur;
lgadresseReceveur = sizeof(struct sockaddr_in);
if ((recu = recvfrom(sock,buffer,BUFSIZE,0,(struct sockaddr *)adresseReceveur,&lgadresseReceveur)) == -1) 
	{
	perror("recvfrom()"); 
	close(sock); 
	exit(1);
	}
}

