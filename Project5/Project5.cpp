//Project 5
//Ethan Laur
//CSCI 201

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

void nopunct(char *dst)
{
	char *src = dst;
	while (*src) { if (!isalpha((unsigned char)*src)) src++; else *dst++ = *src++; }
	*dst = 0;
}

char *minimize(char *obj)
{
	char * buf = (char *)malloc(strlen(obj));
	strncpy(buf, obj + 1, strlen(obj) - 2);
	return buf;
}

char ispalindrome(char *str)
{
	nopunct(str);
	if (strlen(str) <= 1) return 0x01;
	return (str[0] == str[strlen(str) - 1]) && ispalindrome(minimize(str)); //tail recursion
}

double frac2(int n)
{
	int mul = (n % 2 == 1) ? -1 : 1;
	printf(" %c 1/%d", mul < 0 ? '-' : '+', n);
	if (n <= 1) return 1;
	return frac2(n - 1) + mul * 1/(double)n; //for tail recursion
}

int main(int argc, char * * argv)
{
	if (argc < 3)
	{
		printf("Usage: %s <sentence> <number>\n", argv[0]);
		return 1;
	}
	printf("Palindrome (%s): %d\n", argv[1], ispalindrome(argv[1]));
	printf("\n%.5lf\n", frac2(atoi(argv[2])));
	return 0;
}
