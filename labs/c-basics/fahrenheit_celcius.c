#include <stdio.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char** argv)
{
    if(argc < 2){
	printf("Send the correct arguments");
	return -1;
	}
    if(argc > 3){
	int end = atoi(argv[2]);
	int increment = atoi(argv[3]);
	for(int i = 0; i <= end; i+= increment){
	      printf("Fahrenheit: %d, Celcius: %6.1f\n", i, (5.0/9.0)*(i-32));
		}
	
	}
    else{
    int fahr = atoi(argv[1]);
    printf("Fahrenheit: %d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
	}
    return 0;
}
