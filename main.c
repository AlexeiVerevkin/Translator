#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "Lex_gen.h"
#include "tree.h"
int main()
{
	FILE * out;
	out = fopen("errors.txt","w");
	int i = 1;
	Lex * test = create_Lex("x :=23.0;");
	Node * past = get_first(test);
	Branch * tree;
	while (past != NULL)
	{
		printf("%d token is %d with value %s\n", i, get_token(past), get_value(past));
		past = get_next(past);
		i++;
	}
	//tree = create_tree(test, out);
	destroy_Lex(test);
	_CrtDumpMemoryLeaks();
	return 0;
}