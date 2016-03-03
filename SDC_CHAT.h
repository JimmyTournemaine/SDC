#define BUFSIZE 1024
#define SERVER "localhost"
#define PORT 1235

// Se connecter en UPD
static int udp_connect(struct sockaddr_in *);

// Envoyer le contenu de buffer depuis la socket
static void receive_message(int sock, struct sockaddr_in *adresseReceveur, char *buffer, socklen_t *);

// Recevoir un message depuis la socket et le stocker dans buffer
static void send_message(int sock, struct sockaddr_in *adresseReceveur, const char *buffer, socklen_t);