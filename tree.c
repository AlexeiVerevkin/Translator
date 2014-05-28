#include "tree.h"
#include "Lex_gen.h"
#include <stdio.h>

struct tBranch {
	Branch ** child;
	int mean;
	char * value;
	int nchild;
};

typedef struct tDump
{
	Node * current;
	int correct;
	FILE * errors_file;
} Dump;

Branch * lexpr(Dump * pdump);

Branch * stmt (Dump * pdump);

Branch * expr(Dump * pdump);

Branch * id(Dump * pdump);

Branch * arith(Dump * pdump, int stop);

Branch * num(Dump * pdump);

Branch * add(Dump * pdump);

Branch * sub(Dump * plex);

void error(Dump * pdump, int expected, int current);

void create_operator(int token, Branch * now, int i, Dump * pdump);

Branch * create_tree (Lex * plex, FILE * errors)
{
	Branch * root;
	if (plex == NULL)
	{
		printf("Invalid pointer.\n");
		return NULL;
	}
	Dump * pdump = (Dump *)malloc(sizeof (Dump));
	pdump->correct = 1;
	pdump->current = get_first(plex);
	pdump->errors_file = errors;
	root = stmt (pdump);
	if (pdump->correct == 0 || stmt == NULL)
	{
		free(pdump);
		return NULL;
	}
	free(pdump);
	return root;
}

Branch * stmt(Dump * plex)
{
	Branch * new_one;
	if (get_token(plex->current) == ID)
	{
		if (get_token(get_next(plex->current)) != ASSIGN)
		{
			error(plex, ASSIGN, get_token(get_next(plex->current)));
			return NULL;
		}
		new_one = (Branch *)malloc(sizeof (Branch));
		if (new_one == NULL)
		{
			printf("Not enouh memory.\n");
			return NULL;
		}
		new_one->mean = STMT;
		new_one->child = (Branch **)malloc(2 * sizeof(Branch));
		if (new_one->child == NULL)
		{

			printf("Not enouh memory for kids.\n");
			return NULL;
		}
		new_one->child[0] = lexpr(plex);
		plex->current = get_next(plex->current);
		new_one->child[1] = expr(plex);
		new_one->nchild = 2;
		new_one->value = NULL;
	}
	return new_one;
}

Branch * lexpr(Dump * plex)
{
	if (get_token(plex->current) != ID)
	{
		error(plex, ID, get_token(plex->current));
	}
	Branch * new_one = (Branch *)malloc(sizeof (Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->child = (Branch **)malloc(sizeof(Branch));
	if (new_one->child == NULL)
	{
		printf("Not enough memory for kids.\n");
		return NULL;
	}
	new_one->child[0] = id (plex);
	new_one->mean = LEXPR;
	new_one->nchild = 1;
	new_one->value = NULL;
	return new_one;
}

Branch * expr(Dump * plex)
{
	Branch * new_one = (Branch *)malloc(sizeof (Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->mean = EXPR;
	new_one->nchild = 1;
	new_one->value = NULL;
	new_one->child = (Branch *)malloc(sizeof(Branch));
	if (new_one->child == NULL)
	{
		printf("Not enough memory for kids.\n");
		return NULL;
	}
	new_one->child[0] = arith(plex, ';');
	return new_one;
}

Branch * arith(Dump * plex, int stop)
{
	int i = 0;
	Branch * new_one = (Branch *)malloc(sizeof(Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->mean = ARITH;
	new_one->value = NULL;
	new_one->child = NULL;
	while (get_token(plex->current) != stop)
	{
		if (stop == ')')
		{
			if (get_token(plex->current) == ';' || get_token(plex->current) == NULL)
			{
				error(plex, ')', ';');
				return NULL;
			}
		}
		if (get_token(plex->current) == '(')
		{
			plex->current = get_next(plex->current);
			Branch * one = arith(plex, ')');
			plex->current = get_next(plex->current);
			new_one->child = (Branch **)realloc(new_one->child, (i + 1)*sizeof(Branch));
			if (new_one->child == NULL)
			{
				printf("Not enough memory for kids.\n");
				return NULL;
			}
			new_one->child[i] = one;
			i++;
		}
		if (get_token(plex->current) == ID)
		{
			new_one->child = (Branch **)realloc(new_one->child, (i + 1)*sizeof(Branch));
			if (new_one->child == NULL)
			{
				printf("Not enough memory for kids.\n");
				return NULL;
			}
			new_one->child[i] = id(plex);
			i++;
		}
		if (get_token(plex->current) == NUM)
		{
			new_one->child = (Branch **)realloc(new_one->child, (i + 1)*sizeof(Branch));
			if (new_one->child == NULL)
			{
				printf("Not enough memory for kids.\n");
				return NULL;
			}
			new_one->child[i] = num(plex);
			i++;
		}
		if (get_token(plex->current) == '+')
		{
			create_operator('+', new_one, i, plex);
			i++;
		}
		if (get_token(plex->current) == '-')
		{

			create_operator('-', new_one, i, plex);
			i++;
		}
	}
	new_one->nchild = i;
	return new_one;
}

Branch * num(Dump * plex)
{
	Branch * new_one = (Branch *)malloc(sizeof (Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->child = NULL;
	new_one->mean = NUM;
	new_one->nchild = 0;
	new_one->value = strdup(get_value(plex->current));
	plex->current = get_next(plex->current);
	return new_one;
}

Branch * id(Dump * plex)
{
	Branch * new_one = (Branch *)malloc(sizeof(Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->mean = ID;
	new_one->nchild = 0;
	new_one->child = NULL;
	new_one->value = strdup(get_value(plex->current));
	plex->current = get_next(plex->current);
	return new_one;
}

Branch * add(Dump * plex)
{
	Branch * new_one = (Branch *)malloc(sizeof (Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->child = NULL;
	new_one->mean = ADD;
	new_one->nchild = 0;
	new_one->value = NULL;
	plex->current = get_next(plex->current);
	return new_one;
}

Branch * sub (Dump * plex)
{
	Branch * new_one = (Branch *)malloc(sizeof (Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->child = NULL;
	new_one->mean = SUB;
	new_one->nchild = 0;
	new_one->value = NULL;
	plex->current = get_next(plex->current);
	return new_one;
}

Branch * mult(Dump * plex)
{
	Branch * new_one = (Branch *)malloc(sizeof (Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		return NULL;
	}
	new_one->child = NULL;
	new_one->mean = MULT;
	new_one->nchild = 0;
	new_one->value = NULL;
	plex->current = get_next(plex->current);
	return new_one;
}

void tree_print (Branch * root)
{
	if (root != NULL)
	{
		printf("The mean of Node is %d, his value is %s, he has %d kids, his kids are:\n", root->mean, root->value, root->nchild);
		if (root->nchild != 0)
		{
			for (int i = 0; i < root->nchild; i++)
			{
				tree_print(root->child[i]);
			}
		}
		else
		{
			printf("He has no kids.\n");
		}
	}
}

void tree_free(Branch * root)
{
	if (root != NULL)
	{
		if (root->nchild != 0)
		{
			for (int i = 0; i < root->nchild; i++)
			{
				tree_free(root->child[i]);
			}
			root->nchild = 0;
		}
		if (root->nchild == 0)
		{
			if (root->child != NULL)
			{
				free(root->child);
			}
			if (root->value != NULL)
			{
				free(root->value);
			}
			free(root);
		}
	}
}

void create_operator(int token, Branch * now, int i, Dump* plex)
{
	now->child = (Branch **)realloc(now->child, (i + 1)*sizeof(Branch));
	if (now->child == NULL)
	{
		printf("Not enough memory for kids.\n");
		now->child[i] = NULL;
		return;
	}
	if (!(get_token(get_next(plex->current)) == NUM || get_token(get_next(plex->current)) == ID || get_token(get_next(plex->current)) == '('))
	{
		now->child[i] = NULL;
		error(plex, OPER, NULL);
		return;
	}
	if (!(get_token(get_prev(plex->current)) == NUM || get_token(get_prev(plex->current)) == ID || get_token(get_prev(plex->current)) == ')'))
	{
		now->child[i] = NULL;
		error(plex, OPER, NULL);
		return;
	}
	if (token == '+')
	{
		now->child[i] = add(plex);
	}
	if (token == '-')
	{
		now->child[i] = sub(plex);
	}
}

void error(Dump * plex, int expected, int current)
{
	plex->correct = 0;
	fprintf(plex->errors_file, "Error occured while parsing:\n");
	if (expected == OPER)
	{
		fprintf(plex->errors_file, "Incorrect operator usage. No variables or numbers surrounding him.\n");
	}
	else
	{
		fprintf(plex->errors_file, "Symbol with token %d was expected, but got %d.\n", expected, current);
	}
	plex->current = get_next(plex->current);
}