#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#ifndef __utilities__
#define __utilities__

void inputChar(char* message, char* input){
  // scan to pointer
  printf("%s\n>>>", message);
  scanf(" %c", input);
  // empty buffer
  while(getchar() != '\n');
}

void inputString(char* message, char* input){
  // scan to pointer
  printf("%s\n>>>", message);
  scanf("%[^\n]s", input);
  // empty buffer
  while(getchar() != '\n');
}

void inputInt(char* message, int* input){
  // scan to pointer
  printf("%s\n>>>", message);
  while(scanf(" %d", input) == 0) {
    // empty buffer
    while(getchar() != '\n');
    printf("%s\n>>>", "Please input an integer");
  }
  // empty buffer
  while(getchar() != '\n');
}

void inputFloat(char* message, float* input){
  // scan to pointer
  printf("%s\n>>>", message);
  while(scanf(" %f", input) == 0) {
    // empty buffer
    while(getchar() != '\n');
    printf("%s\n>>>", "Please input an integer");
  }
  // empty buffer
  while(getchar() != '\n');
}
 
void inputBool(char* message, bool* input){
  char string[5];
  bool done = false;
  // scan to pointer
  printf("%s\n>>>", message);
  while(!done) {
    scanf("%[^\n]s", string);
    // Check if 'true', 't', 'false' or 'f'
    if(strcasecmp(string, "true") || strcasecmp(string, "t")) {
      *input = true;
      done = true;
    } else if(strcasecmp(string, "false") || strcasecmp(string, "f")) {
      *input = false;
      done = true;
    } else {
      printf("%s\n>>>", "Please input 'true', 't', 'false' or 'f'");    
    }
    // empty buffer
    while(getchar() != '\n');
  }
}

#endif
