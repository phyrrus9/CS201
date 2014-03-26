//
//  CustomerQueue.h
//  Project4
//
//  Created by Ethan Laur on 2/18/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef Project4_CustomerQueue_h
#define Project4_CustomerQueue_h

#include "Customer.h"

struct CustomerQueueNode {
	Customer c;
	CustomerQueueNode *next;
}; //node for the Queue linked list

class CustomerQueue
{
protected:
	int numitems, qnum;
	CustomerQueueNode *list;
	CustomerQueueNode *tail;
public:
	CustomerQueue() { //default constructor
		numitems = 0;
		list = tail = NULL;
	}
	void enqueue(Customer &insert) { //insert into the queue
		CustomerQueueNode *ins = new CustomerQueueNode;
		ins->c = insert;
		ins->next = NULL;
		if (list == NULL) { //find out if it is the first in the queue
			list = tail = ins;
		}
		else{
			tail->next = ins;
			tail = ins;
		}
		numitems += 1;
	}
	Customer dequeue() { //removes a customer and returns it
		Customer q;
		if (numitems <= 0) {
			q.num = -1;
		} else {
			q = list->c;
			CustomerQueueNode *newhead = list->next;
			delete list;
			list = newhead;
			numitems--;
		}
		return q;
	}
	Customer first() { //returns the first in thr queue
		if (numitems <= 0) {
			Customer q;
			q.num = -1;
			return q;
		}
		return list->c;
	}
	int customerCount() {
		return numitems;
	}
	void setQueueNum(int q) {
		qnum = q;
	}
	int getQueueNum() {
		return qnum;
	}
};

#endif
