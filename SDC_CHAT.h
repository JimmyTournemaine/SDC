#define BUFSIZE 1024
#define SERVER "localhost"
#define PORT 1234

// Se connecter en UPD
static int udp_connect();

// Envoyer le contenu de buffer depuis la socket
static void send_message(int sock, const char * buffer);

// Recevoir un message depuis la socket et le stocker dans buffer
static void write_message(int sock, char * buffer);

/* Maintenant débrouille-toi :p
 * Je te conseille de tester que le programme fonctionne avant de commencer à coder
 * Ça marche chez moi en tout cas !
 * J'ai ajouté un dossier exercices qui contient les exercices 
 *  (fin la des question pourries pour le coup...)
 * Si tu as un message du serveur qui dit qu'il trouve pas le dossier des exercices,
 *  essaie de modifier BASEDIR dans SDC.h en mettant le chemin absolut vers ce dossier ;-)
 */


/*
 * SDC_CHAT est lancé par un élève donc tu n'auras pas à modifier SDC_ELS et SDC
 * Il faudra adapter SDC_PR pour qu'il puisse envoyer et recevoir 
 *  des messages pendant les exercices
 * On laisse les connexions comme elles-sont, c'est que ensuite qu'un élève peut envoyer de messages au prof, donc au niveau de la seconde boucle infinie
 */

/*
 * J'ai modifié le Makefile pour que tout compile, tu peut y jeté un oeil si tu veux ;-)
 */

/*
 * Bon je t'ai lancé plein de pistes donc à toi de jouer !!! =)
 */