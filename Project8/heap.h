#ifndef heap_h
#define heap_h

#define HEAP_SIZE 0x0F //constant size for the heap

#include <string>
using std::string;

class heapItem {
protected:
	string name;
	int    priority;
public:
		    heapItem   (void);
	         heapItem   (string, int);
	         heapItem   (const heapItem &);
	  void   setName    (string);
	  void   setPriority(int);
	  string getName    (void) const;
	  int    getPriority(void) const;
	//maybe operator= isnt being gen'd from the cc right
friend char   operator>  (const heapItem &lhs, const heapItem &rhs);
friend char   operator<  (const heapItem &lhs, const heapItem &rhs);
friend char   operator!= (const heapItem &lhs, const heapItem &rhs);
};

class heap {
protected:
	heapItem heapData[HEAP_SIZE];
	int      nitems;
public:
	     heap   (void);
	void swap   (int, int);
	void enqueue(string, int);
	int  dequeue(string &); //returns prio, name is copied
};

#endif
