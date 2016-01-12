#ifndef __util_h_
#define __util_h_

/// \file util.h
///
/// \author Christoffer Gustafsson
/// \version 1.0
/// \date 2015-10-12
/// \bug None know


#define MAX_STRING_SIZE 1337

/// Extends stdlib's "toupper" function with å, ä and ö.
///
/// \param input character to be translated to upper case
/// \returns input translated to upper case if possible, 
///          else return input unchanged
char toupper_ex(char input);

/// Post question and recieve input from user
///
/// \param msg text to display to user
/// \param allowed allowed characters
/// \returns input from user
char ask_question_char(char *msg, char *allowed);

/// Post question and recieve input from user
///
/// \param msg text to display to user
/// \param start see below
/// \param end input from user must be within <start> to <end>
/// \returns input from user
int ask_question_int(char *msg, int start, int end);

/// Post question and recieve input from user
///
/// \param msg text to display to user
/// \param allowed allowed characters
/// \returns input from user, remeber to free()
char* ask_question_string(char *msg, char *allowed);

#endif
