/*
	@brief Graph object G = (V,E) to contain vertices, weighted edges, and perform operations on those objects.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#pragma once

#include <string>
#include <vector>
#include <map>

//! @brief The graph is a list of connected vertices
struct vertex
{
	int id;		//! Unique ID representing which vertex is held
	int weight; //! The capacity or weight of this connected node
};

//! @brief Graph object to contain vertices and weighted edges in adjacency list format.
class graph
{
public:
	//! @brief Basic constructor
	graph();

	//! @brief Basic destructor
	~graph();

	//! @brief Adds a vertex (node) to the adjacency list
	//! @param id The ID of the vertex (node)
	//! @retval true if successful, false if the node already exists
	bool addNode(int id);

	//! @brief Adds a neighboring vertex (node) to the given vertex in the adjacency list
	//! @brief fromID The ID of the vertex whose neighbor will be added
	//! @param neighborNode The neighboring node with an ID and weight of the connected edge
	//! @retval true if successful, false if the node already exists
	bool addNeighbor(int fromID, vertex neighborNode);

	//! @brief Prints the graph out in adjacency list format
	void print();

	//! @brief Performs a breadth first search on the graph to obtain the shortest path and minimum capacity in the path
	//! @param start Starting node
	//! @param end Ending node. The search will be stopped once this is reached
	//! @retval A pair containing the shortest path (in terms of number of edges - not the weights of the edges) from
	//!	 starting node to ending node, and the minimum capacity (weight) along that path
	//! @retval An empty vector if the start or end nodes were invalid, or the end node could not be found, with a 
	//!  negative minimum capacity
	std::pair< std::vector<int>, int> breadthFirstSearch(int start, int end);

	int fordFulkerson();

private:
	std::map<int, std::map<int, vertex> > adjList;	  //!< Adjacency list representation of the directed graph
	int numNodes;									  //!< Number of nodes
};
