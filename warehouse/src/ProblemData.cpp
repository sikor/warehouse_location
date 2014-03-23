/*
 * ProblemData.cpp
 *
 *  Created on: 08-08-2013
 *      Author: pawel
 */

#include "ProblemData.h"



void ProblemData::readFromStdIn()
{
	cin>>warehousesCount>>customersCount;
	for(int i =0; i<warehousesCount; ++i){
		Warehouse * warehouse = new Warehouse();
		double cap, cost;
		cin>>cap>>cost;
		warehouse->capacity = cap;
		warehouse->setupCost = cost;
		warehouse->index = i;
		warehouses.push_back(warehouse);
	}
	for(int i=0; i<customersCount; ++i){
		Customer * customer = new Customer();
		int demand;
		cin>>demand;
		customer->demand = demand;
		customer->index = i;
		for(int j =0; j< warehousesCount; ++j){
			double cost;
			cin>>cost;
			customer->costs.push_back(cost);
		}
		customers.push_back(customer);
	}
//	cout<<"read from stdin"<<endl;
}



