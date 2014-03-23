/*
 * SimplexRelaxation.cpp
 *
 *  Created on: 17 sie 2013
 *      Author: sikor
 */

#include "SimplexRelaxation.h"
#include "includeStl.h"
#include <coin/ClpSimplex.hpp>
#include "constants.h"


#define DEBUG 0


using namespace std;

SimplexRelaxation::SimplexRelaxation(ProblemData* problemData) {
	this->data = problemData;
	this->wc = problemData->warehousesCount*problemData->customersCount;
	this->variablesCount =  wc  + data->warehousesCount;
	allIntegers = false;
	solution = NULL;
	firstNonInteger = -1;
}

void SimplexRelaxation::initializeCustomizeSimplexModel(vector<bool> * isWarehouseOpen,
		vector<int> * warehousesCapacity) {

	model.setLogLevel(0);
	int numberRows = wc + data->customersCount + data->warehousesCount;
			//only to open warehouse, each client has one, demand is ok;
			//number of quotations

	int numberColumns = variablesCount;	//number of variables
	int numberElements = 2*wc + wc + wc;	//number of non zero parameters in quotations
		// matrix data - column ordered
	CoinBigIndex start[numberColumns+1]; //indices in rows and elements arrays for column
	int length[numberColumns]; //number of elements in each column
	int rows[numberElements]; //rows for elements
	double elements[numberElements]; //parameters in quotations


	for(int i=0; i<data->warehousesCount; ++i){
		length[i] = data->customersCount;
	}
	for(int i=data->warehousesCount; i<numberColumns; ++i){
		length[i] = 3;
	}
	start[0] = 0;
	for(int i=1; i<numberColumns+1; ++i){
		start[i] = start[i-1]+length[i-1];
	}
	for(int i = 0; i<wc; ++i){
		rows[i] = i;
	}

	int elementNumber = wc - 1;
	for(int i=0; i<wc; ++i){
		rows[++elementNumber] = i;
		rows[++elementNumber] = wc + (i%data->customersCount);
		rows[++elementNumber] = wc + data->customersCount + i/data->customersCount;
	}

	for(int i=0; i<wc; ++i){
		elements[i] = 1;
	}
	elementNumber = wc - 1;
	for(int i=0; i<wc; ++i){
		elements[++elementNumber] = -1;
		elements[++elementNumber] = 1;
		int demand = data->customers[i%data->customersCount]->demand;
		elements[++elementNumber] = demand;
	}
	CoinPackedMatrix matrix(true, numberRows, numberColumns, numberElements, elements, rows, start, length);

		// rim data
	double objective[numberColumns]; //parameters in objective for each variable
	double rowLower[numberRows]; //lower bound for each equotation
	double rowUpper[numberRows]; //upper bound for each equotation
	double colLower[numberColumns]; //lower bound for each variable
	double colUpper[numberColumns]; //upper bound for each variable


	//warehouses can be opened or not.
	for(int i=0; i<data->warehousesCount; ++i){
		objective[i] = data->warehouses[i]->setupCost;
		if((*isWarehouseOpen)[i]){
			colLower[i] = 1;
		}else{
			colLower[i] = 0;
		}
		colUpper[i] = 1;
	}

	int variableIndex = data->warehousesCount;
	for(int i=0; i<data->warehousesCount; ++i){
		for(int j=0; j<data->customersCount; ++j){
			objective[variableIndex] = data->customers[j]->costs[data->warehouses[i]->index];
			colLower[variableIndex] = 0;
			colUpper[variableIndex] = 1;
			++variableIndex;
		}
	}

	for(int i=0; i<wc; ++i){
		rowLower[i] = 0;
		rowUpper[i] = COIN_DBL_MAX;
	}
	for(int i=wc; i<wc+data->customersCount; ++i){
		rowLower[i] = 1;
		rowUpper[i] = 1;
	}
	int whn =0;
	for(int i=wc+data->customersCount; i<numberRows; ++i){
		rowLower[i] = 0;
		rowUpper[i] = (*warehousesCapacity)[whn];
		++whn;
	}

		// load problem
	model.loadProblem(matrix, colLower, colUpper, objective,
					   rowLower, rowUpper);
		// mark integer (really for Cbc/examples/modify.cpp
		// Solve


	for(int i=0; i<numberColumns; ++i){
		model.setInteger(i);
	}
}

void SimplexRelaxation::initializeSimplexModel() {


	model.setLogLevel(0);
	int numberRows = wc + data->customersCount + data->warehousesCount;
			//only to open warehouse, each client has one, demand is ok;
			//number of quotations

	int numberColumns = variablesCount;	//number of variables
	int numberElements = 2*wc + wc + wc;	//number of non zero parameters in quotations
		// matrix data - column ordered
	CoinBigIndex start[numberColumns+1]; //indices in rows and elements arrays for column
	int length[numberColumns]; //number of elements in each column
	int rows[numberElements]; //rows for elements
	double elements[numberElements]; //parameters in quotations


	for(int i=0; i<data->warehousesCount; ++i){
		length[i] = data->customersCount;
	}
	for(int i=data->warehousesCount; i<numberColumns; ++i){
		length[i] = 3;
	}
	start[0] = 0;
	for(int i=1; i<numberColumns+1; ++i){
		start[i] = start[i-1]+length[i-1];
	}
	for(int i = 0; i<wc; ++i){
		rows[i] = i;
	}

	int elementNumber = wc - 1;
	for(int i=0; i<wc; ++i){
		rows[++elementNumber] = i;
		rows[++elementNumber] = wc + (i%data->customersCount);
		rows[++elementNumber] = wc + data->customersCount + i/data->customersCount;
	}

	for(int i=0; i<wc; ++i){
		elements[i] = 1;
	}
	elementNumber = wc - 1;
	for(int i=0; i<wc; ++i){
		elements[++elementNumber] = -1;
		elements[++elementNumber] = 1;
		int demand = data->customers[i%data->customersCount]->demand;
		elements[++elementNumber] = demand;
	}
	CoinPackedMatrix matrix(true, numberRows, numberColumns, numberElements, elements, rows, start, length);

		// rim data
	double objective[numberColumns]; //parameters in objective for each variable
	double rowLower[numberRows]; //lower bound for each equotation
	double rowUpper[numberRows]; //upper bound for each equotation
	double colLower[numberColumns]; //lower bound for each variable
	double colUpper[numberColumns]; //upper bound for each variable

	for(int i=0; i<data->warehousesCount; ++i){
		objective[i] = data->warehouses[i]->setupCost;
		colLower[i] = 0;
		colUpper[i] = 1;
	}

	int variableIndex = data->warehousesCount;
	for(int i=0; i<data->warehousesCount; ++i){
		for(int j=0; j<data->customersCount; ++j){
			objective[variableIndex] = data->customers[j]->costs[data->warehouses[i]->index];
			colLower[variableIndex] = 0;
			colUpper[variableIndex] = 1;
			++variableIndex;
		}
	}

	for(int i=0; i<wc; ++i){
		rowLower[i] = 0;
		rowUpper[i] = COIN_DBL_MAX;
	}
	for(int i=wc; i<wc+data->customersCount; ++i){
		rowLower[i] = 1;
		rowUpper[i] = 1;
	}
	int whn =0;
	for(int i=wc+data->customersCount; i<numberRows; ++i){
		rowLower[i] = 0;
		rowUpper[i] = data->warehouses[whn]->capacity;
		++whn;
	}

		// load problem
	model.loadProblem(matrix, colLower, colUpper, objective,
					   rowLower, rowUpper);
		// mark integer (really for Cbc/examples/modify.cpp
		// Solve

	for(int i=0; i<numberColumns; ++i){
		model.setInteger(i);
	}

//	model.initialSolve();
}

double SimplexRelaxation::calculateRelaxedValue() {
//	model.setMaximumSeconds(2);
	firstNonInteger = -1;
	model.primal();
	if(model.isProvenPrimalInfeasible()){
#if DEBUG
		cout<<"not feasible"<<endl;
#endif
		return COIN_DBL_MAX;
	}
	this->solution = model.primalColumnSolution();

	allIntegers = true;

	double smallest = 2;
	for(int i=0; i<variablesCount; ++i){
		double value = solution[i];
		if( 1.0 - value > DELTA and value > DELTA){
			allIntegers = false;
			if(abs(0.5 - value) < smallest){
				firstNonInteger = i;
			}
		}
	}
	return model.objectiveValue();
}

void SimplexRelaxation::setVariable(int index, int value) {
	model.setColBounds(index, value, value);
}

void SimplexRelaxation::unsetVariable(int index) {
	model.setColBounds(index, 0, 1);
}

int SimplexRelaxation::getVariableIndex(int warehouse, int customer) {
	return data->warehousesCount+warehouse*data->customersCount + customer;
}

bool SimplexRelaxation::areAllInteger() {
	return allIntegers;
}

int SimplexRelaxation::getVarialbesCount() {
	return variablesCount;
}



int SimplexRelaxation::getFirstNonInteger() {
	return firstNonInteger;
}
