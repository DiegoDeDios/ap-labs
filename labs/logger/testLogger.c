#include <stdio.h>
void main(){
int x = 10;
char* y = "warning";
float z = 10.0f;
infof("Info, valor: %d \n",x);
warnf("Advertencia, valor: %s\n",y);
errorf("Error, valor: %f \n",z);
panicf("Panico\n");
}