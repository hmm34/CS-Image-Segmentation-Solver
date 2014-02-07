/*
	@brief Demonstrates the ability to solve the image segmentation problem using the Ford-Fulkerson algorithm.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "tools.hpp"
#include "graph.hpp"
#include "pgm.hpp"

int main(int argc, char* argv[])
{
	graph inputGraph;

	if (argc < 2)
	{
		std::cerr << "No options found!\n";
		return 1;
	}

	// Process CLI ARGs
	while(true)
	{
		int option = getopt(argc,argv, "bif");

		if (option == -1)
			break;

		// BFS Option
		if (option == 'b')
		{     
			// Check for at least three additional options
			if (optind + 2 >= argc)
			{
				std::cerr << "Invalid use of option -b\n";
				std::cerr << "Usage: -b [input file] [start vertex] [end vertex]\n";
				return 1;
			}
		
			// This will go to BFS Function
			int startVertex = atoi(argv[optind + 1]);

			// Obtain the potential multiple end points specified
			int optOffset = 2;
			int endPoint = atoi(argv[optind + optOffset]);

			// Generate graph from file
			tools::graphFromFile(argv[optind], inputGraph);
			std::cerr << "Number of nodes is: " << inputGraph.nodes() << std::endl;
			
			std::pair< std::vector<int>, int > searchResult = tools::breadthFirstSearch(inputGraph, startVertex, endPoint);
			std::vector<int> shortestPath = searchResult.first;	// Shortest path p along graph G
			int minCapacity = searchResult.second;				// Minimum capacity along p
			unsigned int numEdges = shortestPath.size() - 1; 	// Edges = Nodes - 1

			//! @note Again - testing purposes!
			std::cerr << "Found shortest path from " << startVertex << " to "
				<< endPoint << " to be: " << numEdges << std::endl;
			std::cerr << "Minimum capacity is: " << minCapacity << std::endl;

			inputGraph.print();
		}

		// Ford-Fulkerson Option
		if (option == 'f')
		{
			// Check for at most one additional option
			if (optind >= argc)
			{
				std::cerr << "Invalid use of option -f\n";
				std::cerr << "Usage: -f [input file]\n";
				return 1;
			}

			// This will go to Ford Fulkerson Function
			tools::graphFromFile(argv[optind], inputGraph);

			int source = 0;
			int sink   = inputGraph.sNodes.size() - 1;
			int maxFlow = tools::fordFulkerson(inputGraph, source, sink);
			std::cerr << "Max flow is: " << maxFlow << "\n";
		}

		// Image Segmentation Option
		if (option == 'i')
		{
			// Check for at most two additional options
			if (optind + 1 >= argc)
			{
				std::cerr << "Invalid use of option -i\n";
				std::cerr << "Usage: -i [input file] [input file]\n";
				return 1;
			}
			tools::segmentImage(argv[optind], argv[optind+1]);
		}
	}		
	return 0;
}
