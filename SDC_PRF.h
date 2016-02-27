/**
 * Professor RIP application
 * @file SDC_PRF.h
 * @author Jean Arthur Ousmane
 * @version 1.0
 * @date 26 feb. 2016
 */

#define PORT 1234
#define SERVER_NAME "localhost"
#define BUFSIZE 1024

static int sock_connect();
static void sock_read(int sock, char * buffer);
static void sock_write(int sock, const char * buffer);