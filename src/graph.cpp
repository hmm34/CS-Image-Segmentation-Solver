/*
	@copydoc Graph.hpp
*/

#include "Graph.hpp"
#include <iostream>
#include <sstream>

Graph::Graph() {}

Graph::~Graph() {}

bool Graph::addNode(int id)
{
	sNodes.insert(id);

	// Only add if this node ID doesn't already exists in the list
	if (adjList.find(id) == adjList.end())
	{
		std::map<int, vertex> emptyNeighbors;
		adjList[id] = emptyNeighbors;
		return true;
	}
	return false;
}

bool Graph::addNeighbor(int fromID, vertex neighborNode)
{
	sNodes.insert(neighborNode.id);

	// Only add if this neighboring node ID doesn't already exists in the list
	if (adjList[fromID].find(neighborNode.id) == adjList[fromID].end())
	{
		adjList[fromID][neighborNode.id] = neighborNode;
		return true;
	}
	return false;
}

int Graph::nodes()
{
	return sNodes.size();
}

void Graph::print()
{
	std::map<int, std::map<int, vertex> >::iterator adjItr = adjList.begin();
	std::map<int, std::map<int, vertex> >::iterator adjEnd = adjList.end();
	while (adjItr != adjEnd)
	{
		// Current vertex
		std::cerr << (*adjItr).first;
		std::map<int, vertex>::iterator neighborsItr = (*adjItr).second.begin();
		std::map<int, vertex>::iterator neighborsEnd = (*adjItr).second.end();

		// Neighboring vertices
		while (neighborsItr != neighborsEnd)
		{	
			std::cerr << " --(" << (*neighborsItr).second.weight << ")--> " << (*neighborsItr).second.id;
			++neighborsItr;
		}
		std::cerr << std::endl;
		++adjItr;
	}
}

