//
//  PoissonRandom.h
//  Project4
//
//  Created by Ethan Laur on 2/18/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef Project4_PoissonRandom_h
#define Project4_PoissonRandom_h

#include <climits>
#include <cmath>

class PoissonRandom { //class for the poissonrandom generator
protected:
	double seed;
	int poissonRandom(double expectedValue) {
		int n = 0; //counter of iteration
		double limit;
		double x;  //pseudo random number
		limit = exp(-expectedValue);
		x = (1.0*rand()) / INT_MAX;
		while (x > limit) {
			n++;
			x *= (1.0*rand()) / INT_MAX;
		}
		return n;
	}
public:
	PoissonRandom(double _seed) {
		seed = _seed;
	}
	int next() {
		return poissonRandom(seed);
	}
};

#endif
