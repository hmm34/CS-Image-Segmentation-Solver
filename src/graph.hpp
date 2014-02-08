/*
	@brief graph object G = (V,E) to contain vertices, weighted edges, and perform operations on those objects.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#pragma once
	
#include <set>
#include <map>

//! @brief The graph is a list of connected vertices
struct vertex
{
	int id;		//! Unique ID representing which vertex is held
	int weight; //! The capacity or weight of this connected node
};

//! @brief Graph object to contain vertices and weighted edges in adjacency list format.
class Graph
{

	public:
		//! @brief Basic constructor
		Graph();

		//! @brief Basic destructor
		~Graph();

		//! @brief Adds a vertex (node) to the adjacency list
		//! @param id The ID of the vertex (node)
		//! @retval true if successful, false if the node already exists
		bool addNode(int id);

		//! @brief Adds a neighboring vertex (node) to the given vertex in the adjacency list
		//! @brief fromID The ID of the vertex whose neighbor will be added
		//! @param neighborNode The neighboring node with an ID and weight of the connected edge
		//! @retval true if successful, false if the node already exists
		bool addNeighbor(int fromID, vertex neighborNode);

		//! @brief Get the number of nodes contained within this graph
		//! @retval The number of unique nodes in the graph
		int nodes();

		//! @brief Prints the graph out in adjacency list format
		void print();

		std::map<int, std::map<int, vertex> > adjList;	//!< Adjacency list representation of the directed graph
		std::set<int> sNodes;							//!< Unique set of all nodes
};
