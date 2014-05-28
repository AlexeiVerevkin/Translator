#include "code_translator.h"
char * create_string(Branch * root);
void translate(Branch * root, char * filename)
{
	FILE * out;
	out = fopen(filename, "w");
	if (out == NULL)
	{
		printf("Sorry, but we can't create file %s.\n", filename);
		return;
	}
	if (root == NULL)
	{
		fprintf(out, "/*Invalid code was translated. Please try again later.*/");
	}
	else
	{
		fprintf(out, "%s", create_string(root));
	}
	fclose(out);
}
char * create_string(Branch * root)
{
	char str[3000];
	int mean = get_mean(root);
	if (mean == STMT)
	{
		strcpy(str,create_string(kids(root)[0]));
		strcat(str, " = ");
		strcat(str, create_string(kids(root)[1]));
	}
	if (mean == LEXPR)
	{
		strcpy(str,create_string(kids(root)[0]));
	}
	if (mean == ID)
	{
		strcpy(str, (t_value(root)));
	}
	if (mean == EXPR)
	{
		strcpy(str,create_string(kids(root)[0]));
		strcat(str, ";");
	}
	if (mean == ARITH)
	{
		int i;
		strcpy(str,"(");
		for (i = 0; i < child_n(root); i++)
		{
			strcat(str, create_string(kids(root)[i]));
		}
		strcat(str, ")");
	}
	if (mean == NUM)
	{
		strcpy(str, t_value(root));
	}
	if (mean == '+')
	{
		strcpy(str," + ");
	}
	if (mean == '-')
	{
		strcpy(str, " - ");
	}
	if (mean == '*')
	{
		strcpy(str, " * ");
	}
	if (mean == '/')
	{
		strcpy(str, " / ");
	}
}