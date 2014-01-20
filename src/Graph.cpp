/*
	@copydoc Graph.h
*/

#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>

Graph::Graph(std::string fileName)
{
	/* 
		@note The file should be in the format of an adjacency list.
		For example:
			1 3 2 6 3 8
			2 2
			3 1
		Here, we have four vertices 0-3. The first line represents the vertices
		that vertex 0 is connected to, with their edge weights. Line 2 shows the
		vertices that vertex 1 is conected to, with their weights - and so on.
		The first line shows that vertex 0 is connected to vertex 1 with edge
		weight 3, vertex 2 with edge weight 6, and vertex 3 with edge weight 8.
	*/

	// Open the file containing the graph information
	std::ifstream input;
	input.open(fileName);
	if (!input)
	{
		std::cerr << "Could not open " << fileName << "!\n";
	}

	// For each line in the file
	std::string line;
	while (getline(input, line))
	{
		// Obtain the list of connected vertices and their edge weights
		std::list<vertex> connectedVertices = std::list<vertex>();
		std::stringstream ss(line);
		while (ss)
		{
			vertex u;
			ss >> u.number; // The first # is the vertex number for u
			if (!ss) break; // If there's no second number, we're finished here
			ss >> u.weight; // The second # is the capacity/weight from v to u
			connectedVertices.push_back(u);
		}
		adjacencyList.push_back(connectedVertices);
	}

	// Clean up
	input.close();
}

Graph::~Graph()
{
	adjacencyList.clear();
}

void Graph::print()
{
	// Traverse each vertex u in the graph
	for (int u=0; u < adjacencyList.size(); ++u)
	{
		std::cout << u; // Current vertex
		std::list<vertex>::iterator current = adjacencyList.at(u).begin();
		std::list<vertex>::iterator end     = adjacencyList.at(u).end();
		
		// Traverse each vertex v to which u is connected
		while (current != end)
		{
			std::cout << " --(" << (*current).weight;
			std::cout << ")--> " << (*current).number;
			++current;
		}
		std::cout << std::endl;
	}
}