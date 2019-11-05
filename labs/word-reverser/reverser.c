#include <stdio.h>
#include <string.h>

void swap(char* a, char*b){ // I wanted to implement an in-place swap
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

void reverse(char* buffer, int size ){
       char* a = buffer;
       char* b = buffer + size - 1;
       for(char i = 0; i < size/2; i++){
           swap(a++, b--);
       }
}

 int main(){
    char c = 0;
    char buffer[64];
    while((c = getchar()) != EOF ){
       scanf("%s", buffer);
       reverse(buffer,strlen(buffer));
       printf("%s\n", buffer);
       }
     return 0;
 }

