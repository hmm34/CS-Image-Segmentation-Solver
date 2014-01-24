/*
	@brief Graph object G = (V,E) to contain vertices and weighted edges, as 
		well as perform operations on those objects.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#pragma once

#include <string>
#include <vector>

//! @brief The graph is a list of connected vertices
struct vertex
{
	int number;	//! Represents which vertex is held
	int weight; //! The capacity or weight of this connected node
};

//! @brief Graph object to contain vertices and weighted edges in adjacency list format.
class graph
{
public:
	//! @brief Basic constructor
	//! @param fileName The text file containing the graph
	graph(std::string fileName);

	//! @brief Basic destructor
	~graph();

	//! @brief Prints the graph out in adjacency list format
	void print();

	//! @brief Performs a breadth first search on the graph to obtain the shortest path
	//! @param start Starting node
	//! @param end Ending node. The search will be stopped once this is reached
	//! @retval The shortest path (in terms of number of edges - not the weights of the edges) from
	//!		starting node to ending node
	//! @retval An empty vector if the start or end nodes were invalid, or the end node could not be
	//!		found.
	std::vector<int> breadthFirstSearch(int start, int end);

private:
	//! @brief Adjacency list representation of the directed graph.
	std::vector< std::vector<vertex> > adjacencyList;
};
