/*
 * BranchAndBoundSolver.h
 *
 *  Created on: 17 sie 2013
 *      Author: sikor
 */

#ifndef BRANCHANDBOUNDSOLVER_H_
#define BRANCHANDBOUNDSOLVER_H_

#include "includeStl.h"
#include "ProblemData.h"
#include "SimplexRelaxation.h"
#include "Timer.h"

using namespace std;

class BranchAndBoundSolver {
public:
	BranchAndBoundSolver(ProblemData * data, SimplexRelaxation * relaxation, Timer timer, double targetValue = 9999999999999999999.0);
	void solve();
	vector<int> bestSolution;
	double bestSolutionCost;
	void solveTimed();
	bool isOptimal;
	void printBestSolution();
	long long int nodesCounter;
	long long int currentDepth;
private:
	ProblemData* data;
	SimplexRelaxation* relaxation;
	Timer timer;
	bool foundAnySolution;
	double targetValue;

};

#endif /* BRANCHANDBOUNDSOLVER_H_ */
