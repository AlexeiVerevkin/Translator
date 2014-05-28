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

void error(Dump * pdump, int nexpected);

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
		error(plex, ID);
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
	if (check(ASSIGN, get_token(plex->current)) == 1)
	{
		error(plex, get_token(plex->current));
	}
	new_one->mean = ARITH;
	new_one->value = NULL;
	new_one->child = NULL;
	while (get_token(plex->current) != stop)
	{
		if (stop == ';')
		{
			if (get_token(plex->current) == ')')
			{
				error(plex, get_token(plex->current));
			}
		}
		else
		{
			if (get_token(plex->current) == ';')
			{
				error(plex, get_token(plex->current));
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
			if (check(ID, get_token(plex->current)) == 1)
			{
				error(plex, get_token(plex->current));
			}
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
		if (get_token(plex->current) == '*')
		{
			create_operator('*', new_one, i, plex);
			i++;
		}
		if (get_token(plex->current) == '/')
		{
			create_operator('/', new_one, i, plex);
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
	if (check(ID, get_token(get_next(plex->current))) == 1)
	{
		error(plex, get_token(get_next(plex->current)));
	}
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
	if (check(ID, get_token(get_next(plex->current))) == 1)
	{
		error(plex, get_token(get_next(plex->current)));
	}
	new_one->value = strdup(get_value(plex->current));
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
	Branch * new_one;
	now->child = (Branch **)realloc(now->child, (i + 1)*sizeof(Branch));
	if (now->child == NULL)
	{
		printf("Not enough memory for kids.\n");
		now->child[i] = NULL;
		return;
	}
	if (check(OPER,get_token(get_next(plex->current))) == 1)
	{
		error(plex, get_token(get_next(plex->current)));
	}
	new_one = (Branch *)malloc(sizeof (Branch));
	if (new_one == NULL)
	{
		printf("Not enough memory.\n");
		now->child[i] = NULL;
		return;
	}
	new_one->child = NULL;
	new_one->mean = token;
	new_one->nchild = 0;
	new_one->value = NULL;
	plex->current = get_next(plex->current);
	now->child[i] = new_one;
}

void error(Dump * plex, int nexpected)
{
	plex->correct = 0;
	fprintf(plex->errors_file, "Error occured while parsing:\n");
	fprintf(plex->errors_file,"Token %d is unexpected there.\n",nexpected);
	plex->current = get_next(plex->current);
}

int check(int token_b, int token_curr)
{
	if (token_b == ID)
	{
		int res = 0;
		int arr[] = { '+', '-', '*', '/', ASSIGN, ';',')'};
		int i;
		for (i = 0; i < 7; i++)
		{
			if (token_curr == arr [i])
			{
				res++;
			}
		}
		if (res == 0)
		{
			return 1;
		}
		return 0;
	}
	if (token_b == OPER)
	{
		int res = 0;
		int arr[] = {NUM, ID, '('};
		int i;
		for (i = 0; i < 3; i++)
		{
			if (token_curr == arr[i])
			{
				res++;
			}
		}
		if (res == 0)
		{
			return 1;
		}
		return 0;
	}
	if (token_b == ASSIGN)
	{
		int res = 0;
		int arr[] = {NUM, ID, '('};
		int i;
		for (i = 0; i < 3; i++)
		{
			if (token_curr == arr[i])
			{
				res++;
			}
		}
		if (res == 0)
		{
			return 1;
		}
		return 0;
	}
}