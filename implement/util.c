#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "util.h"
#include "main.h"
#include "../gc.h"

extern heap_t *our_heap;

char toupper_ex(char input){
  switch(input){
  // case '�': return '�';
  // case '�': return '�';
  // case '�': return '�';
  default : return toupper(input);
  }
}

bool char_allowed(char input, char *allowed){
  if(allowed == NULL) return false;
  int c=0;
  while(allowed[c] != '\0'){
    if(toupper_ex(input) == allowed[c++])
      return true;
  }
  return false;
}

char ask_question_char(char *msg, char *allowed){
  bool have_answer = false;
  char input;
  while(!have_answer){
    printf("%s", msg);
    input = getchar();
    if(input != '\n'){
      while(getchar() != '\n');
      have_answer = char_allowed(input, allowed);
    }

    if(!have_answer){
      printf("Ej till�tet svar; godk�nda tecken [%s]\n", allowed);
    }
  }
  return toupper_ex(input);
}

int ask_question_int(char *msg, int start, int end){
  bool have_answer = false;
  char buffer[sizeof(int)*8+1];
  while(!have_answer){
    printf("%s", msg);
    buffer[sizeof(buffer)/sizeof(char)-2] = '\0';
    fgets(buffer, sizeof(buffer)/sizeof(char), stdin);
    int c=0;
    have_answer = true;
    while(buffer[c] != '\0' && buffer[c] != '\n'){
      if(!char_allowed(buffer[c],"1234567890")){
	if(!(c == 0 && buffer[0] == '-')) have_answer = false;
      }
      c++;
    }
    if(buffer[0] == '\n' || (buffer[0]=='-' && buffer[1]=='\n')){
      printf("Fel; ange en siffra\n");
      have_answer = false;
    }
    else if(buffer[sizeof(buffer)/sizeof(char)-2] != '\0' && buffer[sizeof(buffer)/sizeof(char)-2] != '\n'){
      printf("Fel; talet du angav �r f�r stort\n");
      while(getchar() != '\n');
      have_answer = false;
    }
    else if(have_answer && (atoi(buffer) < start || atoi(buffer) > end)){
      printf("Fel; talet �r inte inom spannet %d till %d.\n", start, end);
      have_answer = false;
    }
    else if(!have_answer)
      printf("Ej till�tet svar; godk�nda tecken [0123456789]\n");
  }
  return atoi(buffer);
}

char* ask_question_string(char *msg, char *allowed){
  bool have_answer = false;
  char str[MAX_STRING_SIZE];
  while(!have_answer){
    printf("%s", msg);
    str[sizeof(str)-2] = '\0';
    fgets(str, sizeof(str), stdin);
    int c=0;
    have_answer = true;
    while(str[c] != '\0' && str[c] != '\n'){
      if(!char_allowed(str[c], allowed))
	have_answer = false;
      c++;
    }

    if(str[0] == '\n'){
      printf("Fel; du m�ste skriva n�got\n");
      have_answer = false;
    }
    else if(str[sizeof(str)-2] != '\0' && str[sizeof(str)-2] != '\n'){
      printf("Fel; F�r mycket text\n");
      while(getchar() != '\n');
      have_answer = false;
    }
    else if(!have_answer)
      printf("Ej till�tet svar; godk�nda tecken [%s]\n", allowed);
  }

  int c=-1;
  while(str[++c] != '\0');
  if(str[c-1] == '\n')
    str[c-1] = '\0';

  void *return_str = h_alloc_data(our_heap, (strlen(str)+1) * sizeof(char));
  strcpy((char *)return_str, str);

  return return_str;
}
