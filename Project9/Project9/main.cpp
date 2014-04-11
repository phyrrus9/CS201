//
//  main.cpp
//  Project9
//
//  Created by Ethan Laur on 4/6/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int tablesize, listsize;
static char table[2048][32];
static int linearprobeavg, quadraticprobeavg;
static int twc, tuc;

unsigned int hash(char *w)
{
	unsigned int temp = 0, temp2, i;
	for (i = 0; i < strlen(w); i++)
	{
		temp <<= 1;
		temp2 = w[i];
		temp ^= temp2;
	}
	return temp % tablesize;
}

unsigned int linearprobe(unsigned int hash, unsigned int *comparisons = NULL, char *sv = NULL)
{
	while (1) //empty slot
	{
		if (comparisons == NULL)
		{
			if (table[hash][0] == 0) return hash;
		}
		else
		{
			if (strcmp(table[hash], sv) == 0) return hash;
			++(*comparisons);
		}
		if (hash > tablesize) hash = 0;
		else hash++;
	}
	return hash; //never gets here
}

unsigned int quadraticprobe(unsigned int hash, unsigned int *comparisons = NULL, char *sv = NULL)
{
	unsigned int i = 1, a = hash, b = a;
	if (!table[hash][0]) return hash;
	while (1)
	{
		a = hash + (i * i);
		b = hash - (i * i);
		while (b > tablesize) a -= tablesize;
		while (b > tablesize) b -= tablesize;
		if (comparisons == NULL)
		{
			if (!table[a][0]) return a;
			if (!table[b][0]) return b;
		}
		else
		{
			if (strcmp(table[hash], sv) == 0) return hash;
			if (strcmp(table[a], sv) == 0) return a;
			if (strcmp(table[b], sv) == 0) return b;
			*comparisons += 2;
		}
		++i;
	}
	return hash; //never gets here
}

char * strcheck(char *str)
{
	int len = (int)strlen(str), i;
	for (i = 0; i < len; i++)
		if (isalnum(str[i]))
			return &str[i];
	return NULL;
}

char isintable(char *w)
{
	int i;
	for (i = 0; i < tablesize; i++)
		if (strcmp(w, table[i]) == 0)
			return 1;
	return 0;
}

void init_table()
{
	memset(table, 0, tablesize * 32); //set everything to NULL
}

void insertwords(char *file)
{
	FILE *f = fopen(file, "r");
	char *tmpw = (char *)malloc(180), *sp = tmpw, *insw;
	int  *wc = (int *)malloc(sizeof(int)), uc = 0, cmp;
	unsigned int hk;
	*wc = 0;
	memset(tmpw, 0, 120);
	while (fscanf(f, "%c", sp++) != EOF)
	{
		*(sp - 1) = toupper(*(sp - 1));
		cmp = 0;
		if (!isalnum(*(sp - 1)) && *(sp - 1) != '\'' && *(sp - 1) != '-')
		{
			*(sp - 1) = 0;
			if ((insw = strcheck(tmpw)) != NULL)
				if (!isintable(insw) || getenv("search")[0] == '1')
				{
					++uc;
					if (strcmp(getenv("search"), "0") == 0)
					{
						if (getenv("probe")[0] == '0')
							strcpy(table[linearprobe(hash(insw))], insw);
						else
							strcpy(table[quadraticprobe(hash(insw))], insw);
					}
					else
					{
						if (getenv("probe")[0] == '0')
						{
							//searching using lprobe
							cmp = 1; //first compare, duh
							hk = hash(insw);
							hk = linearprobe(hk, (unsigned int *)&cmp, insw);
							linearprobeavg += cmp;
						}
						else
						{
							//quadratic probe search
							cmp = 1;
							hk = hash(insw);
							hk = quadraticprobe(hk, (unsigned int *)&cmp, insw);
							quadraticprobeavg += cmp + 1; //sort of a hack, less than one
						}
					}
				}
			++*wc;
			sp = tmpw;
		}
	}
	fclose(f);
	//printf("WC: %d\tUC: %d\n", *wc, uc);
	twc = *wc;
	tuc = uc;
}

int main(int argc, char * * argv, char * * envp)
{
	linearprobeavg = 0;
	quadraticprobeavg = 0;
	listsize = tablesize = atoi(getenv("tsize"));
	init_table();
	setenv("search", "0", 1);
	insertwords(argv[1]);
	setenv("search", "1", 1);
	insertwords(argv[1]);
	printf("%s\tTable size: %s\n", getenv("probe")[0] - 48 ? "Quadratic" : "Linear", getenv("tsize"));
	printf("Total words in speech: %d\n", twc);
	printf("Total unique words in speech: %d\n", tuc);
	printf("Mean search comparisons: %2.2lf\n\n",
		  (double)((double)(getenv("probe")[0] - 48 ? quadraticprobeavg : linearprobeavg)) / (double)twc);
	if (tablesize == 611)
		setenv("tsize", "711", 1);
	else if (tablesize == 711)
		setenv("tsize", "1235", 1);
	else
	{
		if (strcmp(getenv("probe"), "0") == 0)
		{
			setenv("tsize", "611", 1);
			setenv("probe", "1", 1);
		}
		else return 0;
	}
	execve(argv[0], argv, envp); //re-exec
}

