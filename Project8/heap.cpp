#include "heap.h"
#include <string.h>

heapItem::heapItem(void) {
	name = "";
	priority = 0;
}

heapItem::heapItem(string n, int p) {
	name = n;
	priority = p;
}

heapItem::heapItem(const heapItem &old) {
	name = old.getName();
	priority = old.getPriority();
}

void heapItem::setName(string n) {
	name = n;
}

void heapItem::setPriority(int p) {
	priority = p;
}

string heapItem::getName(void) const {
	return name;
}

int heapItem::getPriority(void) const {
	return priority;
}


char operator>  (const heapItem &lhs, const heapItem &rhs) {
	return (lhs.getPriority() > rhs.getPriority());
}

char operator<  (const heapItem &lhs, const heapItem &rhs) {
	return (lhs.getPriority() < rhs.getPriority());
}

char operator!= (const heapItem &lhs, const heapItem &rhs) {
	return !(strcmp(lhs.getName().c_str(), rhs.getName().c_str()) &&
		    lhs.getPriority() == rhs.getPriority());
}

heap::heap(void) { //default constructor
	heapItem tmp;
	int      i;
	for (i = 0; i < HEAP_SIZE; i++) {
		tmp.setName("DOES NOT EXIST");
		tmp.setPriority(0);
		heapData[i] = tmp;
	}
}

void heap::enqueue(string n, int p) { //adds an element to the queue (heap)
	heapItem el;
	int eli = nitems;
	el.setName(n);
	el.setPriority(p);
	if (nitems != HEAP_SIZE)
		heapData[nitems] = el;
	if (nitems > 0)
		while (eli > 0 && heapData[eli] > heapData[(eli - 1) / 2]) {
			swap(eli, (eli - 1) / 2);
			eli = (eli - 1) / 2;
		}
	if (nitems < HEAP_SIZE - 1) nitems++;
}


int heap::dequeue(string &n) {
	int p, eli = 0;
	n = heapData[0].getName();
	p = heapData[0].getPriority();
	if (nitems > 0) {
		swap(eli, nitems - 1);
		--nitems;
	}
	//for some reason, this function does not work correctly, it only dequeues a few
	//elements before it starts hanging
	while (
		  (heapData[eli] < heapData[(2 * eli) + 1] ||
		  heapData[eli] < heapData[(2 * eli) + 2])
		  && eli < nitems) {
		if (heapData[( 2 * eli) + 1] > heapData[(2 * eli) + 2]) {
			swap(eli, (2 * eli) + 1);
			eli = (2 * eli) + 1;
		}
		if (heapData[( 2 * eli) + 2] > heapData[(2 * eli) + 1]) {
			swap(eli, (2 * eli) + 2);
			eli = (2 * eli) + 2;
		}

	}
	return p;
}

void heap::swap(int a, int b) { //performs a quick swap
	heapItem tmp;
	          tmp = heapData[a];
	heapData[a]   = heapData[b];
	heapData[b]   = tmp;
}
