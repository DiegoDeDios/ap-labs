#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char charcmp(void* x, void* y);
char intcmp(void* x, void* y);
char floatcmp(void* x, void* y);
void mergesort(void* arr, int left, int right, size_t size, char(*compare)(void * x, void * y));
void merge(void* arr, int left, int m, int right, size_t size, char(*compare)(void * x, void * y));
void printFloat(float* arr, char size);
void printChar(char* arr, char size);
void printInt(int* arr, char size);

int main()
{
	//------Floats---------------
   printf("Floats\n");
   float a[] = {9.0f,5.0f,2.0f,7.0f};
   printFloat(a, sizeof(a)/sizeof(float));
   printf("Sorted:\n");
   mergesort(a, 0, 3, sizeof(float), floatcmp);
   printFloat(a, sizeof(a)/sizeof(float));

   //------------Integers---------
   printf("Integers\n");
   int b[] = {5,9,11,3,1};
   printInt(b, sizeof(b)/sizeof(int));
   printf("Sorted:\n");
   mergesort(b, 0, 4, sizeof(int), intcmp);
   printInt(b, sizeof(b)/sizeof(int));

    //------------Characters---------
   printf("Chars\n");
   char c[] = {'z','x','a','b'};
   printChar(c, sizeof(c)/sizeof(char));
   printf("Sorted:\n");
   mergesort(c, 0, 3, sizeof(char), charcmp);
   printChar(c, sizeof(c)/sizeof(char));
    return 0;
}

void merge(void* arr, int left, int m, int right, size_t size, char(*compare)(void * x, void * y)){
	int i, j, k;
	int n1 = m - left + 1;
	int n2 = right - m;
	void* lSide = malloc(size*n1); 
	void* rSide = malloc(size*n2);
	for(i = 0; i < n1; i++){
		//lSide[i] = arr[left + i];
		memcpy(lSide + size*i, arr+size*(left+i),size);
		}
	for(j = 0; j < n2; j++){
		//rSide[j] = arr[m + 1 + j];
		memcpy(rSide + size*j, arr+size*(m+1+j),size);
		}
	i = 0; 
	j = 0;
	k = left;
	while(i < n1 && j < n2){
		//lSide[i] <= rSide[j])
		//compare(lSide + size*i, rSide + size*j)
		if(compare(lSide + size*i, rSide + size*j)){
			//arr[k] = lSide[i];
			memcpy(arr + size*k, lSide+ size*i, size);
			i++; 
			}
		else{
			//arr[k] = rSide[j];
			memcpy(arr + size*k, rSide+ size*j, size);
			j++;
			}
		k++;
	}
	while(i < n1){
		//arr[k] = lSide[i];
		memcpy(arr + size*k, lSide+ size*i, size);
		i++;
		k++;
		}
	while(j < n2){
		//arr[k] = rSide[j];
		memcpy(arr + size*k, rSide+ size*j, size);
		j++;
		k++;
		}
	free(lSide);
	free(rSide);
}

void mergesort(void* arr, int left, int right, size_t size, char(*compare)(void * x, void * y)){
        if(left < right){
                int m = left+(right-left)/2;
                mergesort(arr, left, m, size, compare);
                mergesort(arr, m+1, right, size, compare);
                merge(arr, left, m, right, size, compare);
                }
}

char charcmp(void* x, void* y){
	char * a = (char*)x;
    char * b = (char*)y;
	return *a <= *b ? 1 : 0;
}
char intcmp(void* x, void* y){
	int * a = (int*)x;
    int * b = (int*)y;
	return *a <= *b ? 1 : 0;
}
char floatcmp(void* x, void* y){
	float * a = (float*)x;
    float * b = (float*)y;
	return *a <= *b ? 1 : 0;
}

void printFloat(float* arr, char size){
	for(char i = 0; i < size; i++){
		printf("%.6f\n",arr[i]);
		}
	}
void printChar(char* arr, char size){
	for(char i = 0; i < size; i++){
		printf("%c\n",arr[i]);
		}
}
void printInt(int* arr, char size){
	for(char i = 0; i < size; i++){
		printf("%d\n",arr[i]);
		}
}