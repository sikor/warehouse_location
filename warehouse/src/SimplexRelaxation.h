/*
 * SimplexRelaxation.h
 *
 *  Created on: 17 sie 2013
 *      Author: sikor
 */

#ifndef SIMPLEXRELAXATION_H_
#define SIMPLEXRELAXATION_H_

#include <coin/ClpSimplex.hpp>
#include "ProblemData.h"

class SimplexRelaxation {
public:
	SimplexRelaxation(ProblemData * problemData);
	void initializeSimplexModel();
	void initializeCustomizeSimplexModel(vector<bool> * isWarehouseOpen,
			vector<int> * warehousesCapacity);
	double calculateRelaxedValue();
	void setVariable(int index, int value);
	void unsetVariable(int index);
	int getVariableIndex(int warehouse, int customer);
	bool areAllInteger();
	int getVarialbesCount();
	int getFirstNonInteger();
	const double * solution;
	int variablesCount;
private:

	ProblemData * data;
	ClpSimplex model;
	int wc;
	bool allIntegers;


	int firstNonInteger;
};

#endif /* SIMPLEXRELAXATION_H_ */
