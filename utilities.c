
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

void inputInt(char* message, int * input){
  // scan to pointer
  printf("%s\n>>>", message);
  while(scanf(" %u", input) == 0) {
    // empty buffer
    while(getchar() != '\n');
    printf("%s\n>>>", "Please input an integer");
  }
  // empty buffer
  while(getchar() != '\n');
}
