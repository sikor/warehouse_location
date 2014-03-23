/*
 * model.h
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "includeStl.h"

using namespace std;

class Cost{
public:
	Cost(int warehouseId, double cost){
		this->warehouseId = warehouseId;
		this->cost = cost;
	}
	double cost;
	int warehouseId;

	static bool cmp(const Cost &c1, const Cost &c2){
		return c1.cost < c2.cost;
	}
};

class Warehouse{
public:
	int index;
	double capacity;
	double setupCost;
};

class Customer{
public:
	int index;
	int demand;
	vector<double> costs;
};

#endif /* MODEL_H_ */
