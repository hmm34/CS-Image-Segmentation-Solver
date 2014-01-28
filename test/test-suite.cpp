/*
	@brief Executes a series of tests on CS-Image-Segmentation-Solver. This includes verifiation of results and
	 timing metrics on the breadth first search algorithm, Ford-Fulkerson, and the image segmentation solution.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#include <assert.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "../src/graph.hpp"

const char* TEMP_GRAPH = "test/graphs/temp.txt"; // Location of temp graph file

//! @brief Q&D implementation of Marsaglia's xorshift random number generator. Rand() is way too slow to obtain
//!	 random numbers rapidly and in repetition. See http://en.wikipedia.org/wiki/Xorshift
//! @retval Pseudo-random number that passes the diehard tests.
uint32_t xorshift()
{
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
void generateRandomGraph(const char* file, int e, int v)
{
	static const int MAX_NODE_WEIGHT = 9; // Arbitrarily chosen for simplicity

	int matrix[v][v];
	memset(&matrix, 0, sizeof(matrix));

	// First off - gaurauntee that the graph will be connected s -> t given s = 0 and t = e. This will be the worst
	// case scenario, when BFS has to travel through every node within the graph. With the pseudo-randomly generated
	// nodes below, and an e value greater than v, this shouldn't be an issue.
	int previous = 0;
	int nodeWeight = (xorshift() % MAX_NODE_WEIGHT) + 1;
	for (int i = 1; i < v ; ++i)
	{
		matrix[previous][i] = nodeWeight;
		previous = i;
	}	

	// Randomly allot remaining edges
	for (int i = 0; i <= e - v; ++i)
	{
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
	for (int i = 0; i < v; ++i)
	{
		for (int j = 0; j < v; ++j)
		{
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
double timeBFS(int e, int v)
{
	// Read random graph as an adjacency list and perform BFS
	generateRandomGraph(TEMP_GRAPH, e, v);
	graph g(TEMP_GRAPH);
	time_t start = time(0);
	double seconds = -1;
	std::pair< std::vector<int>, int> result = g.breadthFirstSearch(0, v-1);

	// BFS had better complete...
	if ((result.first.size() > 0) && (result.second > 0))
		seconds = difftime(start, time(0));

	// Clean up temporary graph text file created when graph was generated
	remove(TEMP_GRAPH);

	return seconds;
}

//! @brief Entry point to test suite execution
//! @retval Error or success code
int main()
{
	std::cout << "Timing metrics for breadth first search: " << std::endl;
	std::cout << std::left << std::setw(7) << "V + E" << std::right << std::setw(10) << "seconds" << std::endl;
	for (int totalVE = 100; totalVE <= 4000; totalVE += 100)
	{
		int edges = 2 * totalVE / 3 + 3;
		int vertices = totalVE - edges;
		double seconds = timeBFS(edges, vertices);
		std::cout << std::left << std::setw(7) << totalVE << std::right << std::setw(10) << seconds << std::endl;
	}

	return 0;
}