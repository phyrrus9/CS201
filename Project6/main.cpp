#include "BST.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * * argv)
{
	int *list = new int[argc - 1];
	BST<int> tree;
	for (int i = 1; i < argc; i++)
	{
		list[i - 1] = atoi(argv[i]);
		tree.insert(list[i - 1]);
	}
	printf("inorder:\n");
	tree.inorder();
	printf("\npreorder:\n");
	tree.preorder();
	printf("\npostorder:\n");
	tree.postorder();
	printf("\nbreadthFirst:\n");
	tree.breadthFirst();
	putchar(0x0A);
}
