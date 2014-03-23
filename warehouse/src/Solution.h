/*
 * Solution.h
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "includeStl.h"
#include "ProblemData.h"

using namespace std;

class Solution {
public:
	Solution(ProblemData * data);
	vector<int> customerToWarehouse;
	vector<int> freeCapacity;
	vector<bool> isOpen;
	vector< list<int> > warehousesCustomers;
	vector<int> openedWarehouses;
	double cost;
	bool isOptimal;
	void resetSolution();
	void printSolution();
	void recalculateVariables();
	ProblemData * data;
};

#endif /* SOLUTION_H_ */
