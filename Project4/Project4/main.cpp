//
//  main.cpp
//  Project4
//
//  Created by Ethan Laur on 2/11/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "Customer.h"
#include "CustomerQueue.h"
#include "PoissonRandom.h"

#define SWAIT() if (getenv("FAST") != NULL) { usleep(300); } else { sleep(1); }

using namespace std;

char names[128][30];

unsigned int timer = 0, stop_time;
CustomerQueue *queue;
bool printing = false;
bool verbose_mode;
int max_wait = 0;
int mean_wait = 0;
int num_exited = 0;

//prevents race conditions on output
#define printme(a)\
if (verbose_mode) { while (printing); while (printing); printing = true; a printing = false; }

void *queueparser(void *q2) {
	int index = *(int *)q2;
	Customer cust;
	while (timer < stop_time) {
		cust = queue[index].dequeue();
		if (cust.num < 0) continue; //customer not in queue
		cust.setDepartureTime(timer + cust.getArrivalTime() + cust.getServiceTime());
		while (timer < cust.getDepartureTime()); //sleep loop
		int waited = (timer) - cust.getServiceTime() - cust.getArrivalTime();
		waited = waited > 1 << 10 ? 0 : waited;
		printme(printf("\t%s leaves after waiting %d minutes\n", cust.getCustomerName().c_str(), waited);)
		if (waited > max_wait) max_wait = waited;
		mean_wait += waited;
		++num_exited;
		usleep(100);
	}
	return NULL;
}

int find_shortest(CustomerQueue *queue, int num_queue) {
	int shortest = 200; //no more than 200 cust per queue
	int shortestindex = -1;
	for (int j = 0; j < num_queue; j++) {
		if (queue[j].customerCount() < shortest) {
			shortest = queue[j].customerCount();
			shortestindex = j;
		}
	}
	return shortestindex;
}

void read_names(const char *file) {
	FILE *f = fopen(file, "r");
	for (int i = 0; i < 128; i++) {
		fscanf(f, "%s", names[i]);
	}
	fclose(f);
}

int main(int argc, char * * argv) {
	PoissonRandom       * random1, * random2;
	int                   time_minutes, k = 0, num = 0, qnum = 0, num_queues;
	double                arrival_minute, mean_service_time;
	pthread_t             threadid; //sort of unused, just for starting threads
	time_minutes      =   atoi(argv[1]);
	arrival_minute    =   atof(argv[2]);
	num_queues        =   atoi(argv[3]);
	mean_service_time =   atoi(argv[4]);
	verbose_mode      =   atoi(argv[5]);
	stop_time         =   time_minutes;
	
	read_names("names");
	queue = new CustomerQueue[num_queues];
	random1 = new PoissonRandom(arrival_minute);
	random2 = new PoissonRandom(mean_service_time);
	
	//start the threads
	for (int i = 0; i < num_queues; i++) {
		int *qnum = new int;
		*qnum = i;
		queue[i].setQueueNum(i);
		pthread_create(&threadid, NULL, &queueparser, (void *)qnum);
	}
	
	Customer c, cust2;
	for (timer = 0; timer < time_minutes; timer++) {
		num = random1->next();
		printme(cout << "Minute " << timer + 1 << ":" << endl;)
		for (int i = 0; i < num; i++, k++) {
			c.setArrivalTime(timer);
			c.setServiceTime(random2->next());
			c.setCustomerName(names[k % 128]);
			c.num = k; //debugging
			qnum = find_shortest(queue, num_queues);
			queue[qnum].enqueue(c);
			printme(printf("\t %s Arrives\n", c.getCustomerName().c_str());)
		}
		SWAIT()
	}
	cout << "Max wait: " << max_wait << " Mean wait: " << (double)((double)mean_wait / (double)num_exited) << endl;
	exit(0);
	return 0;
}
