#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(){
    //vars
    char string[] = "Strings are hard!", tOrF[20];
    int myNum = 15, myNum2 = 15;
    bool thisThisTrue = false;

    //fun place
    printf("Hello world ',:^)\n");
    printf("I am learning C! \n");
    printf("My number is: %d\n", myNum);
    printf("What are strings? %s", string);

    int result = myNum + myNum2;

    printf("Result of %d + %d is: %d\n", myNum, myNum2, result);
    for (int i = 0; i < strlen(tOrF); i++)
    {
        tOrF[i] = string[i];
    }
    
    printf("This sentence is true: %s\n", tOrF);

    for (int i = 0; i < 20; i++)
    {
        printf("Character of string in %d index: %c\n",i, tOrF[i]);
    }
    

    return 0;
}