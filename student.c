#include <stdio.h>
#include "student.h"

struct student student_id()
{
    struct student el;
    
    printf("Login : ");
    scanf("%s", el.login);
    
    printf("Password : ");
    scanf("%s", el.password);
    
    return el;
}