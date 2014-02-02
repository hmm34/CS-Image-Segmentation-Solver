/*
	@brief Various tools and methods used on graphs in order to solve the image segmentation problem
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#pragma once

#include "graph.hpp"

namespace tools {

	//! @brief Reads from a text file
	//! @param file The name of the file to be read
	//! @retval The adjacency list representation of the graph from the file
	/* 
	 	@note The file should be in the format of an adjacency list. For example:
			1 3 2 6 3 8
			2 2
			3 1
	 	Here, we have four vertices 0-3. The first line represents the vertices that vertex 0 is connected to, with 
	 	their edge weights. Line 2 shows the vertices that vertex 1 is conected to, with their weights - and so on. The 
	 	first line shows that vertex 0 is connected to vertex 1 with edge weight 3, vertex 2 with weight 6, and vertex 3 
	 	with edge weight 8.
	 	
	 	@note This allocates memory for the returned graph. The user must free memory after use.
	*/
	 void graphFromFile(const char* file, graph& g);

	//! @brief Ford fulkerson algorithm used to obtain the maximum flow and minimum cut
	//! @param g The graph on which to perform the algorithm
	//! @param source 
	//! @param sink
	//! @retval The maximum flow for the given graph
	int fordFulkerson(graph& g, int source, int sink);

	//! @brief Performs a breadth first search on the graph to obtain the shortest path and minimum capacity in the path
	//! @param g The graph to perform the breadth first search on
	//! @param start Starting node
	//! @param end Ending node. The search will be stopped once this is reached
	//! @retval A pair containing the shortest path (in terms of number of edges - not the weights of the edges) from
	//!	 starting node to ending node, and the minimum capacity (weight) along that path
	//! @retval An empty vector if the start or end nodes were invalid, or the end node could not be found, with a 
	//!  negative minimum capacity
	std::pair< std::vector<int>, int> breadthFirstSearch(graph& g, int start, int end);
}