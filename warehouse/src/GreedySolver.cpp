/*
 * GreedySolver.cpp
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#include "GreedySolver.h"


using namespace std;


GreedySolver::GreedySolver(ProblemData* data, Solution * solution) {
	this->data = data;
	this->solution = solution;
}



void GreedySolver::countSortedCosts() {
	//Find preferred warehouses for each customer.
	for (int i = 0; i < data->customersCount; ++i) {
		Customer* customer = data->customers[i];
		vector<Cost>* customerCostsSorted = new vector<Cost>();
		for (int k = 0; k < data->warehousesCount; ++k) {
			customerCostsSorted->push_back(Cost(k, customer->costs[k]));
		}
		sort(customerCostsSorted->begin(), customerCostsSorted->end(),
				Cost::cmp);
		customersCostSorted.push_back(customerCostsSorted);
	}
}

void GreedySolver::createWarehouseRanking() {
	//make warehouses ranking
	for (int i = 0; i < data->warehousesCount; ++i) {
		bestWarehouses.push_back(WarehouseVote(i, 0));
	}
	for (int i = 0; i < data->customersCount; ++i) {
		Customer* customer = data->customers[i];
		double bestCost = 999999999999999999.0;
		int bestWarehouse = -1;
		for (int j = 0; j < data->warehousesCount; ++j) {
			if (customer->costs[j] < bestCost) {
				bestWarehouse = j;
			}
		}
		bestWarehouses[bestWarehouse].votes++;
	}
	sort(bestWarehouses.begin(), bestWarehouses.end(), WarehouseVote::cmp);
}

Solution * GreedySolver::solve() {

	solution->resetSolution();
	for(int i=0; i<data->customersCount; ++i){
		Customer * customer = data->customers[i];
		int bestWarehouse = -1;
		double bestTotalCost = 999999999999999999.0;
		for(int j=0; j<data->warehousesCount; ++j){
			double currentCost = 0;
			if(solution->freeCapacity[j] < customer->demand){
				continue;
			}
			if(!solution->isOpen[j]){
				currentCost+=data->warehouses[j]->setupCost;
			}
			currentCost+=customer->costs[j];
			if(currentCost < bestTotalCost){
				bestTotalCost = currentCost;
				bestWarehouse = j;
			}

		}
		solution->customerToWarehouse[i] = bestWarehouse;
		solution->warehousesCustomers[bestWarehouse].push_back(i);
		solution->cost+=customer->costs[bestWarehouse];
		solution->freeCapacity[bestWarehouse] -= customer->demand;
		if(!solution->isOpen[bestWarehouse]){
			solution->cost+=data->warehouses[bestWarehouse]->setupCost;
			solution->isOpen[bestWarehouse] = true;
		}
	}
	return solution;
}
