/*
 * ProblemData.h
 *
 *  Created on: 08-08-2013
 *      Author: pawel
 */

#ifndef PROBLEMDATA_H_
#define PROBLEMDATA_H_

#include "includeStl.h"
#include "model.h"

using namespace std;



class ProblemData {
public:
	void readFromStdIn();

	int warehousesCount,customersCount;
	vector<Warehouse *> warehouses;
	vector<Customer *> customers;
};

#endif /* PROBLEMDATA_H_ */
