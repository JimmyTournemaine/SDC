

/**
 * Crée une socket, prépare l'adresse et tente de se connecter au serveur
 *
 * @param port Le numéro de port du serveur.
 * @param server_name Le nom humain du serveur.
 * @param server_addr Un pointeur pouvant accueillir l'adresse du serveur.
 * @return int Le descripteur de la socket.
 */
int sock_connect(int port, const char * server_name);