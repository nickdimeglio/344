#include <stdlib.h>
#include <stdio.h>


int main(void)
{
    // char name;       WRONG

    // char name[11];   OK

    char* name = calloc(sizeof(char), 11);
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Hello %s!\n", name);
    return 0;
}

