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

int tablesize;
static char table[2048][32];

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

unsigned int linearprobe(unsigned int hash)
{
	while (table[hash][0] != 0) //empty slot
	{
		if (hash > tablesize) hash = 0;
		else hash++;
	}
	return hash;
}

unsigned int quadraticprobe(unsigned int hash)
{
	unsigned int i = 1, a = hash, b = a;
	if (!table[hash][0]) return hash;
	while (1)
	{
		a = hash + (i * i);
		b = hash - (i * i);
		while (b > tablesize) a -= tablesize;
		while (b > tablesize) b -= tablesize;
		if (!table[a][0]) return a;
		if (!table[b][0]) return b;
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

void insertwords(char *file)
{
	FILE *f = fopen(file, "r");
	char *tmpw = (char *)malloc(180), *sp = tmpw, *insw;
	int  *wc = (int *)malloc(sizeof(int)), uc = 0;
	*wc = 0;
	memset(tmpw, 0, 120);
	memset(table, 0, tablesize * 32); //set everything to NULL
	while (fscanf(f, "%c", sp++) != EOF)
	{
		*(sp - 1) = toupper(*(sp - 1));
		if (!isalnum(*(sp - 1)) && *(sp - 1) != '\'' && *(sp - 1) != '-')
		{
			*(sp - 1) = 0;
			if ((insw = strcheck(tmpw)) != NULL)
				if (!isintable(insw))
				{
					++uc;
					if (getenv("probe")[0] == '0')
						strcpy(table[linearprobe(hash(insw))], insw);
					else
						strcpy(table[quadraticprobe(hash(insw))], insw);
				}
			++*wc;
			memset(tmpw, 0, 120);
			sp = tmpw;
		}
	}
	printf("WC: %d\tUC: %d\n", *wc, uc);
}

int main(int argc, char * * argv)
{
	int i;
	tablesize = atoi(getenv("tsize"));
	insertwords(argv[1]);
	for (i = 0; i < tablesize; i++)
		printf("%3d : %s\n", i, table[i]);
}

