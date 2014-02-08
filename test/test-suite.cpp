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
#include "../src/Graph.hpp"
#include "../src/Tools.hpp"
#include "../src/Pgm.hpp"

const char* TEMP_GRAPH = "test/graphs/temp.txt"; // Location of temp graph file

//! @brief Q&D implementation of Marsaglia's xorshift random number generator. Rand() is way too slow to obtain
//!	 random numbers rapidly and in repetition. See http://en.wikipedia.org/wiki/Xorshift
//! @retval Pseudo-random number that passes the diehard tests.
uint32_t xorshift() {
	static uint32_t x = 123456789;
  	static uint32_t y = 362436069;
  	static uint32_t z = 521288629;
  	static uint32_t w = 88675123;
  	uint32_t t;
 
  	t = x ^ (x << 11);
  	x = y; y = z; z = w;
  	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

//! @brief Generates a random graph with the given number of edges and vertices
//! @param file Name of the file where the graph will be placed
//! @param e Number of edges
//! @param v Number of vertices
void generateRandomGraph(const char* file, int e, int v) {
	static const int MAX_NODE_WEIGHT = 9; // Arbitrarily chosen for simplicity

	int matrix[v][v];
	memset(&matrix, 0, sizeof(matrix));

	// First off - gaurauntee that the graph will be connected s -> t given s = 0 and t = e. This will be the worst
	// case scenario, when BFS has to travel through every node within the graph. With the pseudo-randomly generated
	// nodes below, and an e value greater than v, this shouldn't be an issue.
	int previous = 0;
	int nodeWeight = (xorshift() % MAX_NODE_WEIGHT) + 1;
	for (int i = 1; i < v ; ++i) {
		matrix[previous][i] = nodeWeight;
		previous = i;
	}	

	// Randomly allot remaining edges
	for (int i = 0; i <= e - v; ++i) {
		int x = 0, y = 0;
		nodeWeight = (xorshift() % MAX_NODE_WEIGHT) + 1;

		// Brute force until edge meets requirements
		bool alreadyExists = true, selfReferences = true, isSymmetric = true;		
		do {
			x = xorshift() % v;
			y = xorshift() % v;
			alreadyExists 	= (matrix[x][y] != 0);	// Don't over-write an existing randomly generated edge
			selfReferences	= (x >= y);				// Prevent edges (u, u) and keep matrix upper triangular
			isSymmetric  	= (matrix[y][x] != 0);	// Prevent (u, v) if (v, u) exists
		} while (alreadyExists || selfReferences || isSymmetric); 
		matrix[x][y] = nodeWeight;
	}

	// Write to file
	std::ofstream temp;
	temp.open(file);
	for (int i = 0; i < v; ++i) {
		for (int j = 0; j < v; ++j) {
			if (matrix[i][j] != 0)
				temp << j << " " << matrix[i][j] << " ";
		}
		temp << std::endl;
	}
	temp.close();
}

//! @param e Number of edges to randomly generate
//! @param v Number of vertices to randomly generate
//! @retval Time in seconds to complete the BFS
//! @retval -1 if BFS is unsuccessful
double timeBFS(int e, int v) {
	// Read random Graph as an adjacency list and perform BFS
	generateRandomGraph(TEMP_GRAPH, e, v);
	Graph g;
	Tools::graphFromFile(TEMP_GRAPH, g);
	time_t start = time(0);
	double seconds = -1;
	std::pair< std::vector<int>, int> result = Tools::breadthFirstSearch(g, 0, v-1);

	// BFS had better complete...
	if ((result.first.size() > 0) && (result.second > 0))
		seconds = difftime(start, time(0));

	// Clean up temporary graph text file created when graph was generated
	remove(TEMP_GRAPH);

	return seconds;
}

//! @brief Runs a breadth first search given a particular test file
//! @param file The input text file containing the Graph to perform the BFS on
//! @param start Starting point of search
//! @param end Ending point of search
//! @param minCapacity The expected minimum capacity of the shortest path
//! @param shortestPath The expected shortest path in vector format
//! @retval true if successful, false if unsuccessful
bool testBFS(const char* file, int start, int end, int minCapacity, std::vector<int> shortestPath)
{
	// Create the graph from the given file
	Graph g;
	Tools::graphFromFile(file, g);

	// Perform the search and obtain results
	std::pair< std::vector<int>, int > searchResult = Tools::breadthFirstSearch(g, start, end);
	std::vector<int> shortestPathResult 			= searchResult.first;	// Shortest path p along graph G
	int minCapacityResult 							= searchResult.second;	// Minimum capacity along p

	// Compare expected results to actual results
	if (minCapacity != minCapacityResult) {
		std::cerr << "Minimum capacity was incorrect. Expected " << minCapacity 
				  << ", but returned " << minCapacityResult << std::endl;
		return false;
	}
	if (shortestPath != shortestPathResult) {
		std::cerr << "Shortest path was incorrect. Expected ";
		for (unsigned int i = 0; i < shortestPath.size(); ++i)
			std::cerr << shortestPath.at(i) << " ";
		std::cerr << ", but returned ";
		for (unsigned int i = 0; i < shortestPathResult.size(); ++i)
			std::cerr << shortestPathResult.at(i) << " ";
		std::cerr << std::endl;
		return false;
	}

	return true;
}


int main() {

	/* --------------------------------------------- Timing Metrics ------------------------------------------- */
	// Breadth First Search
	std::cout << "Timing metrics for breadth first search: " << std::endl;
	std::cout << std::left << std::setw(7) << "V + E" << std::right << std::setw(10) << "seconds" << std::endl;
	for (int totalVE = 100; totalVE <= 4000; totalVE += 100) {
		int edges = 2 * totalVE / 3 + 3;
		int vertices = totalVE - edges;
		double seconds = timeBFS(edges, vertices);
		std::cout << std::left << std::setw(7) << totalVE << std::right << std::setw(10) << seconds << std::endl;
	}

	/* --------------------------------------------- Unit Testing --------------------------------------------- */
	/* --------------------------------------------- Unit Testing: Breadth First Search ----------------------- */
	std::cout << "Breadth first search tests: " << std::endl;

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
				"test/graphs/testcase9.txt"  };

	int expectedSP1[] = {0, 1, 5};	// or {0, 2 5}
	std::vector<int> expectedShortestPath1(expectedSP1, expectedSP1 + sizeof(expectedSP1) / sizeof(int));

	int expectedSP2[] = {0, 1, 3, 5};	// or {0, 2, 4, 5}
	std::vector<int> expectedShortestPath2(expectedSP2, expectedSP2 + sizeof(expectedSP2) / sizeof(int));

	std::vector< std::vector<int> > expectedShortestPaths;
	expectedShortestPaths.push_back(expectedShortestPath1);
	expectedShortestPaths.push_back(expectedShortestPath2);

	int expectedMinCapacities[] = {
						5,		// or 3
						12 		// or 4
					};

	for (int i = 0; i < 1; ++i)
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

	/* --------------------------------------------- Unit Testing: Ford Fulkerson ----------------------------- */
	// Ford Fulkerson - Expected max flow for each of the testcase#[1-10].pgm
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

	
	return 0;
}