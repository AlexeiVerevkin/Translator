#ifndef TREE_H
#include <stdlib.h>
#include "Lex_gen.h"
#define TREE_H
#define STMT 1
#define LEXPR 2
#define EXPR 3
#define ARITH 4
#define ADD 5
#define SUB 6
#define MULT 7
#define DIV 8
#define OPER 9
typedef struct tBranch Branch;
Branch * create_tree(Lex * plex); /*returns poiter to the root of the created tree of Lexems*/
void tree_print(Branch * root);/*print out AST*/
void tree_free(Branch * root);/*free memory after tree using*/
#undef TREE_H
#endif