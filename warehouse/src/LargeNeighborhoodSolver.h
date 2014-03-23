/*
 * LargeNeighborhoodSolver.h
 *
 *  Created on: 18 sie 2013
 *      Author: sikor
 */

#ifndef LARGENEIGHBORHOODSOLVER_H_
#define LARGENEIGHBORHOODSOLVER_H_
#include "includeStl.h"
#include "ProblemData.h"
#include "Timer.h"
#include "Solution.h"
#include "SimplexRelaxation.h"

class ProblemData;

class LargeNeighborhoodSolver {
public:
	LargeNeighborhoodSolver(ProblemData * data, Solution * solution, Timer timer, int baBTimeLimit = 10, int neighborhoodSize = 3);
	Solution * solve();
	int getRandomWarehouseId();
	void searchWarehousesNeighborhood(Solution * solution);
	void searchCustomersNeighborhood(Solution * solution);

	ProblemData * data;
	Timer timer;
	Solution * solution;
	int neighborhoodSize;
	int baBTimeLimit;
	double custProb;
	double selectingProbability;
	int openedNeighborhoodSize;

private:
	void improveSelectedData(ProblemData * selectedData,
			double selectedDataInitialCost, Solution* solution, SimplexRelaxation * relaxation);
};

#endif /* LARGENEIGHBORHOODSOLVER_H_ */
