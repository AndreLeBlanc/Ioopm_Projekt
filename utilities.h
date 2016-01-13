/// \file
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#ifndef __utilities__
#define __utilities__

void inputChar(char* message, char* input);
void inputString(char* message, char* input);
void inputInt(char* message, int* input);
void inputFloat(char* message, float* input);
void inputBool(char* message, bool* input);

#endif
