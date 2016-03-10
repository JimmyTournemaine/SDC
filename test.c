#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 1024

int explodeAnswer(char * buffer, ssize_t offset)
{
    int i;
    char temp[BUFSIZE];
    
    if(strncmp(buffer+offset, "finished", 8) == 0) {
        return 1;
    }
    
    for (i=offset; i<strlen(buffer); i++) {
        if (buffer[i] >= 48 && buffer[i] <= 57) {
            break;
        }
    }
    strncpy(temp, buffer+offset, i+1);

    printf("%s\n", temp);
    
    if(i < strlen(buffer)) {
        return explodeAnswer(buffer, i+1);
    }
    
    return 0;
}

int main(void)
{
    char buffer[BUFSIZE] = "tourneji a repondu : 2finishedbigard.txt\nlapin.txt\nvert.txt";
    
    if (explodeAnswer(buffer, 0) > 0)
        printf("finished trouvé\n");
    
    return 0;
}

