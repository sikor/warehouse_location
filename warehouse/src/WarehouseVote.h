/*
 * WarehouseVote.h
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#ifndef WAREHOUSEVOTE_H_
#define WAREHOUSEVOTE_H_

class WarehouseVote {
public:
	WarehouseVote(int id, int votes){
		warehouseId = id;
		this->votes = votes;
	}
	int votes;
	int warehouseId;

	static bool cmp(const WarehouseVote &w1, const WarehouseVote &w2){
		return w1.votes > w2.votes;
	}
};




#endif /* WAREHOUSEVOTE_H_ */
