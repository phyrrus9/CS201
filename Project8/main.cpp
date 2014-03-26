#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "heap.h"
int main() {
	const
	string names[6] = {
		"Tom Boger",
		"Alex Freds",
		"Twig",
		"Dr. B",
		"Ethan Laur",
		"Parker McCauley"
	}; //static list of constant names
	int    i,
	       prio;
	heap   h;
	string tmp;
	srand(time(NULL));
	for (i = 0; i < 10; i++) {
		int r1= rand() % 6, r2= rand() % 20 + 1;
		printf("Adding %s with %d\n", names[r1].c_str(), r2);
		h.enqueue(names[r1], r2); //insert random ones
	}
	for (i = 0; i < 5; i++) {
		prio = h.dequeue(tmp);
		printf("%-30s was deququed with %d years of experience\n", tmp.c_str(), prio);
	} //dequeue them
		
}