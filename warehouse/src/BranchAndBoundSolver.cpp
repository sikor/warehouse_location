/*
 * BranchAndBoundSolver.cpp
 *
 *  Created on: 17 sie 2013
 *      Author: sikor
 */

#include "BranchAndBoundSolver.h"
#include "constants.h"

using namespace std;

#define DEBUG 0


BranchAndBoundSolver::BranchAndBoundSolver(ProblemData* data, SimplexRelaxation* relaxation, Timer timer, double targetValue) {
	this->data = data;
	this->relaxation = relaxation;
	this->bestSolutionCost = 999999999999999998888.0;
	this->bestSolution.resize(this->data->customersCount);
	this->timer = timer;
	isOptimal = false;
	foundAnySolution = false;
	this->targetValue = targetValue;
	this->nodesCounter = 0;
	currentDepth = 0;
}




void BranchAndBoundSolver::solve() {
	timer.startTimer();
	isOptimal = true;
	solveTimed();
}

void BranchAndBoundSolver::solveTimed() {
	++nodesCounter;
	if(timer.tle() && foundAnySolution){
		isOptimal = false;
		return;
	}
	Timer timer;
	timer.startTimer();
	double relaxedValue = relaxation->calculateRelaxedValue();
#if DEBUG
	if(timer.elapsedTime() > 0){
		cout<<"relax > 1 seconds, visited nodes = "<<nodesCounter
				<<" depth = "<<currentDepth<<endl;
	}
#endif

	if(relaxedValue >= bestSolutionCost || relaxedValue > targetValue + 1){
		return;
	}else if(relaxation->areAllInteger()){
		if(!foundAnySolution){
			foundAnySolution = true;
#if DEBUG
			cout<<"Bab found feasible solution!!"<<endl;
#endif
		}
		bestSolutionCost = relaxedValue;
		int counter = 0;
#if DEBUG
			cout<<"found new best solution "<<relaxedValue<<endl;
#endif
		for(int i=data->warehousesCount; i<relaxation->getVarialbesCount(); ++i){
			if(1.0 - relaxation->solution[i] < DELTA){
				++counter;
				int index = i - data->warehousesCount;
				int warehouse = index/data->customersCount;
				int customer = index%data->customersCount;
				bestSolution[customer] = warehouse;
#if DEBUG
//				cout<<"Bab new best solution"<<endl;
//				printBestSolution();
#endif
			}
		}
		if(counter != data->customersCount){
			cout<<"!!!!!! ERRRRROOOORRRRRRRR !!!!!"<<endl;
			cout<<"counter = "<<counter<<endl;
			cout<<data->warehousesCount<<" "<<relaxation->getVarialbesCount()<<endl;
			for(int i=data->warehousesCount; i<relaxation->getVarialbesCount(); ++i){
					int index = i - data->warehousesCount;
					int warehouse = index/data->customersCount;
					int customer = index%data->customersCount;
					cout<<"var "<<i<<" value "<<relaxation->solution[i]<<" "<<warehouse<<" "<<customer<<endl;
			}
			exit(1);
		}
	}else{
		int firstNonInteger = relaxation->getFirstNonInteger();
		int preferedValue = relaxation->solution[firstNonInteger] < 0.5 ? 1 : 0;
		relaxation->setVariable(firstNonInteger, preferedValue);
		currentDepth++;
		solveTimed();
		relaxation->setVariable(firstNonInteger, (preferedValue + 1)%2);
		solveTimed();
		relaxation->unsetVariable(firstNonInteger);
		currentDepth--;
	}
}

void BranchAndBoundSolver::printBestSolution() {
	cout.precision(20);
	cout<<bestSolutionCost<<" "<<isOptimal<<endl;
	set<int> diffWr;
	for(int i=0; i<data->customersCount; ++i){
		cout<<bestSolution[i]<<" ";
		diffWr.insert(bestSolution[i]);
	}
	cout<<endl;
	cout<<"different whs = "<<diffWr.size()<<endl;
	cout<<endl;
}
