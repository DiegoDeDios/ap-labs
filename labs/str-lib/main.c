#include <stdio.h>

int mystrlen(char*);
char* mystradd(char*, char*);
int mystrfind(char*, char*);

int main(argc, argv)
char argc;
char** argv;
{
   if(argc < 4){
	printf("You need to pass 3 arguments\n");
	return -1;
	}
   int strLen = mystrlen(argv[1]);
   char* strAdd = mystradd(argv[1],argv[2]);
   char* isSubstr = mystrfind(strAdd,argv[3]) ? "yes":"no";
   printf("Initial Length\t: %d\nNew String\t: %s\nSubString was found\t: %s\n", strLen, strAdd, isSubstr); 
   return 0;
}
