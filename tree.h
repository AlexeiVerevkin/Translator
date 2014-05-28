#ifndef TREE_H
#include <stdlib.h>
#include "Lex_gen.h"
#define TREE_H
#define STMT 1 /*statement - basic*/
#define LEXPR 2 /*left part of statement*/
#define EXPR 3 /*some expression*/
#define ARITH 4 /*arithmeticl equation*/
#define OPER 5 /*binary operator value*/
typedef struct tBranch Branch;
Branch * create_tree(Lex * plex); /*returns poiter to the root of the created tree of Lexems*/
void tree_print(Branch * root);/*print out AST*/
void tree_free(Branch * root);/*free memory after tree using*/
#undef TREE_H
#endif