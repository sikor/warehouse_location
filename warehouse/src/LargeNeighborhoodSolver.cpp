/*
 * LargeNeighborhoodSolver.cpp
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#include "LargeNeighborhoodSolver.h"
#include "GreedySolver.h"
#include "BranchAndBoundSolver.h"
#include "Timer.h"
#include "SimplexRelaxation.h"
#include "ProblemData.h"

#define DEBUG 0


LargeNeighborhoodSolver::LargeNeighborhoodSolver(ProblemData* data, Solution * solution, Timer timer, int baBTimeLimit, int neighborhoodSize) {
	this->data = data;
	this->timer = timer;
	this->solution = solution;
	this->baBTimeLimit = baBTimeLimit;
	this->neighborhoodSize = neighborhoodSize;
	custProb = 0.9;
	selectingProbability = 0.1;
	openedNeighborhoodSize = 2;
}


Solution * LargeNeighborhoodSolver::solve() {
	GreedySolver greedySolver(data, solution);
	greedySolver.solve();
	solution->recalculateVariables();

	double bestValue = solution->cost;
	Solution bestSolution(data);
	timer.startTimer();

	while(!timer.tle()){
		double vote = (double)rand()/(double)RAND_MAX;
		if(vote <custProb){
			searchCustomersNeighborhood(solution);
		}else{
			searchWarehousesNeighborhood(solution);
		}
		if(solution->cost < bestValue){
			solution->printSolution();
			bestValue = solution->cost;
			bestSolution = *solution;
		}else if(solution->cost > bestValue){
//			cout<<"worse Solution, current = "<<solution->cost<<" best = "<<bestValue<<endl;
			solution->printSolution();
			exit(0);
		}else{
//			cout<<"same value"<<endl;
		}
	}
	return solution;
}

void LargeNeighborhoodSolver::searchCustomersNeighborhood(Solution* solution) {
	ProblemData selectedData;
	vector<int> warehousesCapacity;

	selectedData.warehouses = data->warehouses;
	selectedData.warehousesCount = data->warehousesCount;
	selectedData.customersCount = 0;
	double selectedDataInitialCost = 0;
	vector<bool> openedWarehouses = solution->isOpen;
	for(int i=0; i<data->warehousesCount; ++i){
		warehousesCapacity.push_back(data->warehouses[i]->capacity);
		if(solution->isOpen[i]){
			list<int>::iterator it;
			bool allCustomersTaken = true;
			for(it = solution->warehousesCustomers[i].begin(); it!=solution->warehousesCustomers[i].end(); ++it){
				Customer * customer = data->customers[*it];
				double vote = (double)rand()/(double)RAND_MAX;
				if(vote < selectingProbability){
					selectedData.customers.push_back(customer);
					selectedData.customersCount++;
					selectedDataInitialCost+=customer->costs[i];
				}else{
					allCustomersTaken = false; //warehouse has to stay opened
					warehousesCapacity[i]-=customer->demand; //warehouse has to be able to serve customer
				}
			}
			if(!allCustomersTaken){
				selectedDataInitialCost += data->warehouses[i]->setupCost; //if at least one customer remain, warehouse has to stay opened.
			}else{
				openedWarehouses[i] = false;
//				cout<<"all customers taken!!"<<endl;
			}
		}else{
				//?
		}
	}
	if(selectedData.customersCount == 0){
		return;
	}


	SimplexRelaxation relaxation(&selectedData);
	relaxation.initializeCustomizeSimplexModel(&(openedWarehouses), &warehousesCapacity);
	improveSelectedData(&selectedData, selectedDataInitialCost, solution, &relaxation);
}

void LargeNeighborhoodSolver::improveSelectedData(ProblemData * selectedData,
		double selectedDataInitialCost, Solution* solution, SimplexRelaxation * relaxation) {
#if DEBUG
	cout << "selected customers count = " << selectedData->customersCount
			<<" selectedDataInitial Cost = "<<selectedDataInitialCost
			<<" selected warehouses = "<<selectedData->warehousesCount
			<<" opened warehouses = "<<solution->openedWarehouses.size()
			<< endl;
#endif
	BranchAndBoundSolver babSolver(selectedData, relaxation,
			Timer(baBTimeLimit), selectedDataInitialCost);
	babSolver.solve();
#if DEBUG
	cout << "Bab Solved !! optimal = " << babSolver.isOptimal <<" best cost = "<<babSolver.bestSolutionCost
			<<" visited nodes = "<<babSolver.nodesCounter<<endl;
#endif
	for (int i = 0; i < selectedData->customersCount; ++i) {
		solution->customerToWarehouse[selectedData->customers[i]->index] =
				selectedData->warehouses[babSolver.bestSolution[i]]->index;
	}
	solution->recalculateVariables();
}

void LargeNeighborhoodSolver::searchWarehousesNeighborhood(Solution* solution){
	int warehouses =neighborhoodSize;
	set<int> wids;
	for(int i=0; i<10 && wids.size()<=openedNeighborhoodSize; ++i){
		int w1id = solution->openedWarehouses[rand()%solution->openedWarehouses.size()];
		wids.insert(w1id);
	}
//	cout<<"opened whs = "<<solution->openedWarehouses.size()<<endl;
	int counter = wids.size();

	while(counter < warehouses){
		int nwid = getRandomWarehouseId();
		if(wids.count(nwid)){
			continue;
		}else{
			counter++;
			wids.insert(nwid);
		}
	}

	ProblemData selectedData;
	double selectedDataInitialCost = 0;
	set<int>::iterator wit;
	for(wit = wids.begin(); wit != wids.end(); ++wit){
		Warehouse * warehouse = data->warehouses[*wit];
		selectedData.warehouses.push_back(warehouse);
		if(solution->isOpen[*wit]){
			selectedDataInitialCost+=warehouse->setupCost;
		}
	}

	selectedData.warehousesCount = selectedData.warehouses.size();
	selectedData.customersCount = 0;
	for(int i=0; i<selectedData.warehousesCount; ++i){
		list<int>::iterator it;
		int warehouseIndex = selectedData.warehouses[i]->index;
		for(it = solution->warehousesCustomers[warehouseIndex].begin(); it!=solution->warehousesCustomers[warehouseIndex].end(); ++it){
			Customer * customer = data->customers[*it];
			selectedData.customers.push_back(customer);
			selectedData.customersCount++;
			selectedDataInitialCost+=customer->costs[warehouseIndex];
		}
	}

	SimplexRelaxation relaxation(&selectedData);
	relaxation.initializeSimplexModel();
	improveSelectedData(&selectedData, selectedDataInitialCost, solution, &relaxation);
}



int LargeNeighborhoodSolver::getRandomWarehouseId() {
	return rand()%this->data->warehousesCount;
}




