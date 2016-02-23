#include <stdio.h>
#include "student.h"

struct student student_id()
{
    struct student el;
    
    /* Ask login */
    printf("Login : ");
    scanf("%s", el.login);
    
    /* Ask password */
    printf("Password : ");
    scanf("%s", el.password);
    
    return el;
}