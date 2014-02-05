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

int main(int argc, char* argv[])
{
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
				std::cerr << "Usage: -b [input file] [start vertex] [end vertex 1] [end vertex 2] ...\n";
				return 1;
			}
		
			// This will go to BFS Function
			int startVertex = atoi(argv[optind + 1]);

			// Obtain the potential multiple end points specified
			int optOffset = 2;
			std::vector<int> endPoints;
			while((optind + optOffset) < argc)
			{
				endPoints.push_back(atoi(argv[optind + optOffset]));
				++optOffset;
			}

			// Perform the BFS on the graph created from the input file
			graph g;
			tools::graphFromFile( argv[optind], g );
			std::cerr << "Number of nodes is: " << g.nodes() << std::endl;
			for (unsigned int i = 0; i < endPoints.size(); ++i)
			{
				std::pair< std::vector<int>, int > searchResult;
				searchResult = tools::breadthFirstSearch(g, startVertex, endPoints[i]);
				std::vector<int> shortestPath = searchResult.first;	// Shortest path p along graph G
				int minCapacity = searchResult.second;				// Minimum capacity along p
				unsigned int numEdges = shortestPath.size() - 1; 	// Edges = Nodes - 1

				//! @note Again - testing purposes!
				std::cerr << "Found shortest path from " << startVertex << " to "
					<< endPoints[i] << " to be: " << numEdges << std::endl;
				std::cerr << "Minimum capacity is: " << minCapacity << std::endl;
			}

			g.print();
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
			graph ffg;
			tools::graphFromFile( argv[optind], ffg );
			graph resid = ffg;
			
			// Hard coded....will fix once number of nodes works
			int source = 0;
			int sink   = resid.sNodes.size() - 1;
			int maxFlow = tools::fordFulkerson(resid, source, sink);
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
