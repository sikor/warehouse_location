//============================================================================
// Name        : warehouse.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <coin/ClpSimplex.hpp>
#include "ProblemData.h"
#include "includeStl.h"
#include "SimplexRelaxation.h"
#include "BranchAndBoundSolver.h"
#include "GreedySolver.h"
#include "Solution.h"
#include "LargeNeighborhoodSolver.h"

using namespace std;

void runExample(){
	ClpSimplex model;
	int numberRows = 5; //number of quotations
	int numberColumns = 8;	//number of variables
	int numberElements = 14;	//number of non zero parameters in quotations
	// matrix data - column ordered
	CoinBigIndex start[9] = {0, 2, 4, 6, 8, 10, 11, 12, 14}; //indices in rows and elements arrays for column
							//whether
	int length[8] = {2, 2, 2, 2, 2, 1, 1, 2}; //number of elements in each column
	int rows[14] = {0, 4, 0, 1, 1, 2, 0, 3, 0, 4, 2, 3, 0, 4}; //rows for elements
	double elements[14] = {3, 5.6, 1, 2, 1.1, 1, -2, 2.8, -1, 1, 1, -1.2, -1, 1.9}; //parameters in quotations
	CoinPackedMatrix matrix(true, numberRows, numberColumns, numberElements, elements, rows, start, length);

	// rim data
	double objective[8] = {1, 0, 0, 0, 2, 0, 0, -1}; //parameters in objective for each variable
	double rowLower[5] = {2.5, -COIN_DBL_MAX, 4, 1.8, 3}; //lower bound for each equotation
	double rowUpper[5] = {COIN_DBL_MAX, 2.1, 4, 5, 15}; //upper bound for each equotation
	double colLower[8] = {2.5, 0, 0, 0, 0.5, 0, 0, 0}; //lower bound for each variable
	double colUpper[8] = {COIN_DBL_MAX, 4.1, 1, 1, 4, COIN_DBL_MAX, COIN_DBL_MAX, 4.3}; //upper bound for each variable
	// load problem
	model.loadProblem(matrix, colLower, colUpper, objective,
				   rowLower, rowUpper);
	// mark integer (really for Cbc/examples/modify.cpp
	model.setInteger(2);
	model.setInteger(3);

	// Solve
	model.initialSolve();

	 // Solution
	     const double * solution = model.primalColumnSolution();
	     int i;
	     for (i = 0; i < numberColumns; i++)
	          if (solution[i])
	               printf("Column %d has value %g\n", i, solution[i]);

	     // Change objective
	        double * objective2 = model.objective();
	        objective2[0] = -100.0;

	        // Solve - primal as primal feasible
	        model.primal(1);

	        // Solution (array won't have changed)
	        for (i = 0; i < numberColumns; i++)
	             if (solution[i])
	                  printf("Column %d has value %g\n", i, solution[i]);

	        // Add constraint
	        int column[8] = {0, 1, 2, 3, 4, 5, 6, 7}; //indices of variables
	        double element2[8] = {1, 1, 1, 1, 1, 1, 1, 1}; //parameters in variables
	        model.addRow(8, column, element2, 7.8, COIN_DBL_MAX);

	        // Solve - dual as dual feasible
	        model.dual();

	        /* Solution
	           This time we have changed arrays of solver so -
	           array won't have changed as column array and we added a row
	           - but be on safe side
	        */
	        solution = model.primalColumnSolution();
	        for (i = 0; i < numberColumns; i++)
	             if (solution[i])
	                  printf("Column %d has value %g\n", i, solution[i]);
}
/*
 *
 *
 * /data/wl_16_1s, 25_2, 50_1, 100_4, 200_1, 500_1, 1000_1, 2000_1
 *
 */
int main() {
	std::ios_base::sync_with_stdio(false);
	cout.precision(20);
	ProblemData data;
	data.readFromStdIn();
//	SimplexRelaxation relaxation(&data);
//	relaxation.initializeSimplexModel();
//	BranchAndBoundSolver solver(&data, &relaxation, Timer(6000));
//	solver.solve();
//	solver.printBestSolution();
//	Solution solution(&data);
//	GreedySolver greedySolver(&data, &solution);
//	greedySolver.solve();
//	solution.printSolution();
	Solution solution(&data);
	LargeNeighborhoodSolver lnSolver(&data, &solution, Timer(3600*1.5));
	lnSolver.custProb = 0.9;
	lnSolver.baBTimeLimit = 180;
	lnSolver.neighborhoodSize = 5;
	lnSolver.selectingProbability = 0.02;
	lnSolver.openedNeighborhoodSize = 3;
	lnSolver.solve();
	solution.printSolution();

	return 0;
}
