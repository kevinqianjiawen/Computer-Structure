#include <stdio.h>

int x = 2;
void set(){
  x = 3;
}

int main(void){
  set();
  printf("%d",x);
}


