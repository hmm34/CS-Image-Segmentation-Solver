/*
	@brief Executes a series of tests on CS-Image-Segmentation-Solver. This includes verifiation of results and
	 timing metrics on the breadth first search algorithm, Ford-Fulkerson, and the image segmentation solution.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include "../src/Graph.hpp"
#include "../src/Tools.hpp"
#include "../src/Pgm.hpp"

const char* TEMP_GRAPH = "test/graphs/temp.txt"; // Location of temp graph file

//! @brief Generates a graph with the given number of edges and vertices
//! @param file Name of the file where the graph will be placed
//! @param e Number of edges
//! @param v Number of vertices
void generateGraph(const char* file, int e, int v) {
	std::ofstream temp;
	temp.open(file);
	for (int vertexNo = 1; vertexNo <= v; ++vertexNo)
	{
		temp << vertexNo << " 1 \n";
	}
	temp.close();
}

//! @brief Executes the timing metrics for breadth first search algorithm
void runBfsTimingMetrics()
{
	std::ofstream bfsTimingOutput;
	bfsTimingOutput.open("test/results/bfs-timing-metrics.csv");
	bfsTimingOutput << "total vertices and edges, number of vertices, number of edges, millseconds to complete\n";

	std::cout << "Timing metrics for breadth first search: " << std::endl;
	std::cout << std::left << std::setw(7) << "V + E" << std::right << std::setw(20) << "milliseconds" << std::endl;
	for (int totalVE = 100; totalVE <= 2000; totalVE += 50) {
		int vertices = (totalVE / 2) + 1;
		int edges = vertices - 1;

		generateGraph(TEMP_GRAPH, edges, vertices);
		Graph g;
		Tools::graphFromFile(TEMP_GRAPH, g);
		std::clock_t start = std::clock();
		std::pair< std::vector<int>, int> result = Tools::breadthFirstSearch(g, 0, vertices-1);
		std::clock_t end   = std::clock();

		if ((result.first.size() > 0) && (result.second > 0))
		{
			double milliseconds =  1000.0 * (end - start) / CLOCKS_PER_SEC;
			std::cout << std::left << std::setw(7) << totalVE << std::right << std::setw(20) 
				  << std::fixed << std::setprecision(6) << milliseconds << std::endl;
			bfsTimingOutput << totalVE << ", " << vertices << ", " << edges << ", " << milliseconds << "\n";
		}
		remove(TEMP_GRAPH);
	}
	bfsTimingOutput.close();
}

//! @brief Executes the timing metrics for ford fulkerson algorithm
void runFfTimingMetrics()
{
	std::ofstream ffTimingOutput;
	ffTimingOutput.open("test/results/ff-timing-metrics.csv");
	ffTimingOutput << "total vertices and edges, number of vertices, number of edges, milliseconds to complete\n";

	std::cout << "Timing metrics for Ford Fulkerson: " << std::endl;
	std::cout << std::left << std::setw(7) << "V + E" << std::right << std::setw(20) << "milliseconds" << std::endl;
	for (int totalVE = 100; totalVE <= 2000; totalVE += 50) {
		int vertices = (totalVE / 2) + 1;
		int edges = vertices - 1;

		generateGraph(TEMP_GRAPH, edges, vertices);
		Graph g;
		Tools::graphFromFile(TEMP_GRAPH, g);
		std::clock_t start = std::clock();
		int result = Tools::fordFulkerson(g, 0, vertices - 3);
		std::clock_t end   = std::clock();

		if (result > 0)
		{
			double milliseconds =  1000.0 * (end - start) / CLOCKS_PER_SEC;
			std::cout << std::left << std::setw(7) << totalVE << std::right << std::setw(20) 
				  << std::fixed << std::setprecision(6) << milliseconds << std::endl;
			ffTimingOutput << totalVE << ", " << vertices << ", " << edges << ", " << milliseconds << "\n";
		}
		else
		{
			std::cerr << "Couldn't find. Result = " << result << "\n";
		}
		remove(TEMP_GRAPH);
	}
	ffTimingOutput.close();
}

//! @brief Executes the timing metrics for the image segmentation algorithm
void runIsegTimingMetrics()
{
	std::ofstream isegTimingOutput;
	isegTimingOutput.open("test/results/iseg-timing-metrics.csv");
	isegTimingOutput << "total pixels, number of columns, number of rows, milliseconds to complete\n";

	std::cout << "Timing metrics for Image Segmentation: \n";
	std::string iSegTestCases[] = {
					"test/pgm/FEEP.pgm",
					"test/pgm/2DGel-2.pgm",
					"test/pgm/apollonian_gasket.ascii.pgm",
					"test/pgm/baboon.ascii.pgm",
					"test/pgm/balloons.ascii.pgm",
					"test/pgm/balloons_noisy.ascii.pgm",
					"test/pgm/barbara.ascii.pgm",
					"test/pgm/body1.ascii.pgm",
					"test/pgm/body2.ascii.pgm",
					"test/pgm/body3.ascii.pgm",
					"test/pgm/brain_398.ascii.pgm",
					"test/pgm/brain_492.ascii.pgm",
					"test/pgm/brain_508.ascii.pgm",
					"test/pgm/brain_604.ascii.pgm",
					"test/pgm/casablanca.ascii.pgm",
					"test/pgm/coins.ascii.pgm",
					"test/pgm/columns.ascii.pgm",
					"test/pgm/columns.pgm",
					"test/pgm/dla.ascii.pgm",
					"test/pgm/dragon.ascii.pgm",
					"test/pgm/f14.ascii.pgm",
					"test/pgm/feep.ascii.pgm",
					"test/pgm/fractal_tree.ascii.pgm",
					"test/pgm/gator.ascii.pgm",
					"test/pgm/glassware_noisy.ascii.pgm",
					"test/pgm/hands.ascii.pgm",
					"test/pgm/handsmat.ascii.pgm",
					"test/pgm/lena.ascii.pgm",
					"test/pgm/marcie.ascii.pgm",
					"test/pgm/mona_lisa.ascii.pgm",
					"test/pgm/mother_daughter.ascii.pgm",
					"test/pgm/mountain.ascii.pgm",
					"test/pgm/pbmlib.ascii.pgm",
					"test/pgm/pepper.ascii.pgm",
					"test/pgm/roi_14.ascii.pgm",
					"test/pgm/saturn.ascii.pgm",
					"test/pgm/screws.ascii.pgm",
					"test/pgm/snap.ascii.pgm",
					"test/pgm/surf.ascii.pgm",
					"test/pgm/totem.ascii.pgm",
					"test/pgm/tracks.ascii.pgm",
					"test/pgm/tracks.pgm",
					"test/pgm/venus1.ascii.pgm",
					"test/pgm/venus2.ascii.pgm",
					"test/pgm/x31_f18.ascii.pgm"
	};

	int numIsegTestCases = 10;
	for (int i = 0; i < numIsegTestCases; ++i)
	{
		std::ifstream input;
		input.open(iSegTestCases[i].c_str());
		assert (input);

		int numColumns = 0, numRows = 0;
		std::string line;		// Ignore first two lines.
		getline(input, line);	// "P2"
		getline(input, line);	// "#Created by Irfan View"

		std::stringstream ss;
		ss << input.rdbuf();
		ss >> numColumns >> numRows;	// X, Y

		std::string filename = iSegTestCases[i].substr(iSegTestCases[i].find("pgm/")+4);
		filename.insert(0, "test/pgm/CUT_");

		std::clock_t start = std::clock();
		Tools::segmentImage(iSegTestCases[i].c_str(), filename.c_str());
		std::clock_t end   = std::clock();
		input.close();

		double milliseconds =  1000.0 * (end - start) / CLOCKS_PER_SEC;
		std::cout << std::left << std::setw(35) << iSegTestCases[i].substr(iSegTestCases[i].find("pgm/")+4);
		std::cout << std::left << std::setw(7) << numColumns << std::right << std::setw(5) << numRows << std::right << std::setw(20) 
			  << std::fixed << std::setprecision(6) << milliseconds << std::endl;
		isegTimingOutput << numColumns * numRows << ", " << numColumns << ", " << numRows << ", " << milliseconds << "\n";
	}
	isegTimingOutput.close();
}

//! @brief Executes the unit tests for breadth first search algorithm
void runBfsUnitTests()
{
	std::cout << "Breadth first search tests: " << std::endl;

	int numBfsTestCases = 10;
	std::string bfsTestCases[] = {
				"test/graphs/testcase1.txt",
				"test/graphs/testcase2.txt",
				"test/graphs/testcase3.txt",
				"test/graphs/testcase4.txt",
				"test/graphs/testcase5.txt",
				"test/graphs/testcase6.txt",
				"test/graphs/testcase7.txt",
				"test/graphs/testcase8.txt",
				"test/graphs/testcase9.txt",
				"test/graphs/testcase10.txt"  };

	int expectedSP1[] = {0, 1, 5};	// or {0, 2 5}
	int minCapacity1  = 5;			// or 3
	std::vector<int> expectedShortestPath1(expectedSP1, expectedSP1 + sizeof(expectedSP1) / sizeof(int));

	int expectedSP2[] = {0, 1, 3, 5};	// or {0, 2, 4, 5}
	int minCapacity2  = 12;				// or 4
	std::vector<int> expectedShortestPath2(expectedSP2, expectedSP2 + sizeof(expectedSP2) / sizeof(int));

	int expectedSP3[] = {0, 1, 4, 7}; 	// or {0, 1, 6, 7},	{0, 2, 5, 7},	{0, 3, 6, 7}
	int minCapacity3  = 9;				// or 10,			5, 				10
	std::vector<int> expectedShortestPath3(expectedSP3, expectedSP3 + sizeof(expectedSP3) / sizeof(int));

	int expectedSP4[] = {0, 1, 3, 5};	// or {0, 2, 4, 5},	{0, 2, 3, 5}
	int minCapacity4  = 4;				// or 4, 			10
	std::vector<int> expectedShortestPath4(expectedSP4, expectedSP4 + sizeof(expectedSP4) / sizeof(int));
	
	int expectedSP5[] = {0, 1, 3};	// or {0, 2, 3}
	int minCapacity5  = 100;			// or 100
	std::vector<int> expectedShortestPath5(expectedSP5, expectedSP5 + sizeof(expectedSP5) / sizeof(int));

	int expectedSP6[] = {0, 1, 3, 5};	// or {0, 2, 4, 5}
	int minCapacity6  = 12;				// or 4
	std::vector<int> expectedShortestPath6(expectedSP6, expectedSP6 + sizeof(expectedSP6) / sizeof(int));

	int expectedSP7[] = {0, 1, 3, 5, 7};	// or {0, 2, 3, 5, 7}, 	{0, 2, 4, 6, 7}
	int minCapacity7  = 20;					// or 10, 				10
	std::vector<int> expectedShortestPath7(expectedSP7, expectedSP7 + sizeof(expectedSP7) / sizeof(int));

	int expectedSP8[] = {0, 1, 3, 5};	// or {0, 1, 4, 5}, 	{0, 2, 4, 5}
	int minCapacity8  = 4;				// or 8,				9
	std::vector<int> expectedShortestPath8(expectedSP8, expectedSP8 + sizeof(expectedSP8) / sizeof(int));

	int expectedSP9[] = {0,1,6,9,11};	// or {0,2,6,9,11}, {0,3,6,9,11}, {0,3,7,9,11}, {0,3,8,10,11}, {0,4,8,10,11}, {0,5,8,10,11}
	int minCapacity9  = 5;				// or 20, 		    10, 		  10, 	   	     10  			20 			  10
	std::vector<int> expectedShortestPath9(expectedSP9, expectedSP9 + sizeof(expectedSP9) / sizeof(int));

	int expectedSP10[] = {0,1,6,10,13}; // or {0,2,6,10,13}, {0,2,7,10,13}, {0,3,7,10,13}
	int minCapacity10  = 3;
	std::vector<int> expectedShortestPath10(expectedSP10, expectedSP10 + sizeof(expectedSP10) / sizeof(int));

	std::vector< std::vector<int> > expectedShortestPaths;
	expectedShortestPaths.push_back(expectedShortestPath1);
	expectedShortestPaths.push_back(expectedShortestPath2);
	expectedShortestPaths.push_back(expectedShortestPath3);
	expectedShortestPaths.push_back(expectedShortestPath4);
	expectedShortestPaths.push_back(expectedShortestPath5);
	expectedShortestPaths.push_back(expectedShortestPath6);
	expectedShortestPaths.push_back(expectedShortestPath7);
	expectedShortestPaths.push_back(expectedShortestPath8);
	expectedShortestPaths.push_back(expectedShortestPath9);
	expectedShortestPaths.push_back(expectedShortestPath10);

	int expectedMinCapacities[] = {
						minCapacity1,
						minCapacity2,
						minCapacity3,
						minCapacity4,
						minCapacity5,
						minCapacity6,
						minCapacity7,
						minCapacity8,
						minCapacity9,
						minCapacity10 };

	for (int i = 0; i < numBfsTestCases; ++i)
	{
		std::string nameOfFile = bfsTestCases[i];
		std::cerr << nameOfFile << "...";
		Graph bfsTestCase;
		Tools::graphFromFile(nameOfFile.c_str(), bfsTestCase);

		int start = 0;
		int end   = bfsTestCase.sNodes.size() - 1;
		std::pair< std::vector<int>, int > searchResult = Tools::breadthFirstSearch(bfsTestCase, start, end);

		std::vector<int> expectedShortestPath = expectedShortestPaths.at(i);
		int expectedMinCapacity = expectedMinCapacities[i];

		if (searchResult.first != expectedShortestPath)
		{
			std::cerr << "Expected shortest path: ";
			for (int j = 0; j < expectedShortestPath.size(); ++j)
				std::cerr << expectedShortestPath.at(i) << ", ";

			std::cerr << "Received shortest path: ";
			for (int j = 0; j < searchResult.first.size(); ++j)
				std::cerr << searchResult.first.at(j) << ", ";

			assert(false);
		}

		if (searchResult.second != expectedMinCapacity)
		{
			std::cerr << "Expected minimum capacity: " << expectedMinCapacity << ", ";
			std::cerr << "Received minimum capacity: " << searchResult.second << "\n";
			assert (false);
		}

		std::cerr << "\n";
	}
}

//! @brief Executes the unit tests for ford fulkerson algorithm
void runFfUnitTests()
{
	std::cerr << "Ford Fulkerson tests: " << std::endl;
	std::pair<std::string, int> maxFlowTestCases[] = {	
				std::make_pair<std::string, int>( "test/graphs/testcase1.txt", 14 ),
				std::make_pair<std::string, int>( "test/graphs/testcase2.txt", 23 ),
				std::make_pair<std::string, int>( "test/graphs/testcase3.txt", 29 ),
				std::make_pair<std::string, int>( "test/graphs/testcase4.txt", 14 ),
				std::make_pair<std::string, int>( "test/graphs/testcase5.txt", 200 ),
				std::make_pair<std::string, int>( "test/graphs/testcase6.txt", 23 ),
				std::make_pair<std::string, int>( "test/graphs/testcase7.txt", 40 ),
				std::make_pair<std::string, int>( "test/graphs/testcase8.txt", 19 ),
				std::make_pair<std::string, int>( "test/graphs/testcase9.txt", 65 ),
				std::make_pair<std::string, int>( "test/graphs/testcase10.txt", 16 ) };

	int numTestCases = 10;
	for (int i = 0; i < numTestCases; ++i) {
		Graph g;
		Tools::graphFromFile( maxFlowTestCases[i].first.c_str() , g );
		int resultMaxFlow = Tools::fordFulkerson( g, 0, g.sNodes.size() - 1 );

		std::cerr << maxFlowTestCases[i].first << "... ";
		if (resultMaxFlow != maxFlowTestCases[i].second )
		{
			std::cerr << "Expected: " << maxFlowTestCases[i].second << ", Received: " << resultMaxFlow << "\n";
			assert( false );
		}
		std::cerr << std::endl;
	}
}

int main() {

	//runBfsTimingMetrics();
	//runFfTimingMetrics();
	runIsegTimingMetrics();

	//runBfsUnitTests();
	//runFfUnitTests();

	return 0;
}
