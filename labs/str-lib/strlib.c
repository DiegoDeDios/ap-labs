

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int mystrlen(char *str){
        const char *s;
        for(s = str; *s; ++s);
        return (s-str);
}

char *mystradd(char *origin, char *addition){
    int originalLength = mystrlen(origin);
    int additionLength = mystrlen(addition);
    char* newString = malloc(sizeof(char)*originalLength + additionLength);
    strcpy(newString, origin);
    strcpy(newString + originalLength, addition);
    return newString;
}

int mystrfind(char *origin, char *substr){
    char* a = origin;
    char* b = substr;
    while(a < origin + mystrlen(origin)){
      if(*a == *b){
        if(b == substr + mystrlen(substr) - 1){
          return 1;
        }
          a+=1;
          b+=1;
      }
      else{
          a+=1;
          b = substr;
      } 
    }
    return 0;
}



