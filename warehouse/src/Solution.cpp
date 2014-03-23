/*
 * Solution.cpp
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#include "Solution.h"

void Solution::resetSolution() {
	freeCapacity.resize(data->warehousesCount);
	isOpen.resize(data->warehousesCount);
	for(int i=0; i<data->warehousesCount; ++i){
		freeCapacity[i] = data->warehouses[i]->capacity;
		isOpen[i] = false;
	}
	cost =0;
	warehousesCustomers.clear();
	for(int i=0; i<data->warehousesCount; ++i){
		warehousesCustomers.push_back(list<int>());
	}
}

void Solution::printSolution() {
	cout.precision(20);
	cout<<cost<<" "<<isOptimal<<endl;
	for(unsigned int i=0; i<this->customerToWarehouse.size(); ++i){
		cout<<customerToWarehouse[i]<<" ";
	}
	cout<<endl;
}

Solution::Solution(ProblemData* data) {
	this->customerToWarehouse.resize(data->customersCount);
	this->freeCapacity.resize(data->warehousesCount);
	this->isOpen.resize(data->warehousesCount);
	this->cost = 0;
	this->isOptimal = false;
	this->data = data;
	for(int i=0; i<data->warehousesCount; ++i){
		warehousesCustomers.push_back(list<int>());
	}
}

void Solution::recalculateVariables() {
	warehousesCustomers.clear();
	freeCapacity.clear();
	openedWarehouses.clear();
	for(int i=0; i<data->warehousesCount; ++i){
		isOpen[i] = false;
		warehousesCustomers.push_back(list<int>());
		freeCapacity.push_back(data->warehouses[i]->capacity);
	}
	cost =0;
	for(unsigned int i=0; i<customerToWarehouse.size(); ++i){
		int warehouseId = customerToWarehouse[i];
		freeCapacity[warehouseId]-=data->customers[i]->demand;
		if(freeCapacity[warehouseId] < 0){
			printSolution();
			cout<<"capacity violated!! warehouse = "<<warehouseId<<" cap = "<<freeCapacity[warehouseId]<<endl;
			exit(0);
		}
		if(!isOpen[warehouseId]){
			isOpen[warehouseId] = true;
			cost +=data->warehouses[warehouseId]->setupCost;
			openedWarehouses.push_back(warehouseId);
		}
		cost+=data->customers[i]->costs[warehouseId];
		warehousesCustomers[warehouseId].push_back(i);
	}
}
