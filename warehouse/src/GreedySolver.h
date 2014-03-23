/*
 * GreedySolver.h
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#ifndef GREEDYSOLVER_H_
#define GREEDYSOLVER_H_

#include "Solution.h"
#include "ProblemData.h"
#include "WarehouseVote.h"


class GreedySolver {
public:
	GreedySolver(ProblemData * data, Solution * solution);
	Solution * solve();

	ProblemData * data;
	Solution * solution;
	vector< vector<Cost> * > customersCostSorted;
	vector<WarehouseVote> bestWarehouses;

private:
	void countSortedCosts();
	void createWarehouseRanking();
};

#endif /* GREEDYSOLVER_H_ */
