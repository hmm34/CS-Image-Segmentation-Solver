/*
	@brief Demonstrates the ability to solve the image segmentation problem using the Ford-Fulkerson algorithm.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "Tools.hpp"
#include "Graph.hpp"
#include "Pgm.hpp"

int main(int argc, char* argv[])
{
	Graph inputGraph;

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
			return 0;

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
			Tools::graphFromFile(argv[optind], inputGraph);
			
			std::pair< std::vector<int>, int > searchResult = Tools::breadthFirstSearch(inputGraph, startVertex, endPoint);
			std::vector<int> shortestPath = searchResult.first;	// Shortest path p along graph G
			unsigned int numEdges = shortestPath.size() - 1; 	// Edges = Nodes - 1

			// Output
			std::cerr << "Found shortest path from " << startVertex << " to "
				<< endPoint << " to be: " << numEdges << " edge(s).\n";

			std::cout << "Path: ";
			for (unsigned int i = 0; i < shortestPath.size(); ++i)
			{
				std::cout << shortestPath[i];

				if (i + i < shortestPath.size())
					std::cout << " --> ";
			}

			std::cout << "\n";
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
			Tools::graphFromFile(argv[optind], inputGraph);

			int source = 0;
			int sink   = inputGraph.sNodes.size() - 1;
			int maxFlow = Tools::fordFulkerson(inputGraph, source, sink);
			std::cerr << "Max flow is " << maxFlow << "\n";
		}

		// Image Segmentation Option
		if (option == 'i')
		{
			// Check for at most two additional options
			if (optind + 1 >= argc)
			{
				std::cerr << "Invalid use of option -i\n";
				std::cerr << "Usage: -i [input file] [ouput file]\n";
				return 1;
			}
			Tools::segmentImage(argv[optind], argv[optind+1]);
		}
	}		
	return 0;
}
