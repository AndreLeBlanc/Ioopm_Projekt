#include <stdio.h>
#include <stdbool.h>

bool is_big_endian();

int main(){ ;
  if(is_big_endian()) printf("%s", "big endian\n");
  else  printf("%s", "little endian\n"); 
  return 0;
}

bool is_big_endian() {
    int i = 1;
    char *c = (char *)&i;

    if (c[0] == 1){
      return false;	
    }
    else{ 
      return true;
    }
}
