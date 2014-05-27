#ifndef LEX_GEN_H
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define LEX_GEN_H
#define NUM -1
#define ID -2
#define DIVN -3
#define ASSIGN -4
typedef struct tLex Lex; /*Current of Lexes*/
typedef struct tNode Node; /*Node of one Lex*/
Lex * create_Lex(char * input); /*Create a current of Lex*/
Node * get_next (Node * in); /*Get Next Node */
Node * get_prev(Node * in); /*Get Previous Node*/
void destroy_Lex(Lex * plex); /*Free memory from Lex*/
int get_token(Node * in); /*Get token from Node*/
Node * get_first(Lex * plex); /*Get first Node of Lex*/
Node * get_last(Lex * plex); /*Get last Node of Lex*/
char * get_value(Node * in); /*Get value of Next*/
#undef LEX_GEN_H
#endif