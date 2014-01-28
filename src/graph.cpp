/*
	@copydoc graph.hpp
*/

#include "graph.hpp"
#include <queue>
#include <set>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

graph::graph()
{ }

graph::~graph()
{ }

void graph::addNode(int id)
{
	// Only add if this node ID doesn't already exists in the list
	if (adjList.find(id) == adjList.end())
	{
		std::map<int, vertex> emptyNeighbors;
		adjList[id] = emptyNeighbors;
		numNodes++;
	}
}

void graph::addNeighbor(int fromID, vertex neighborNode)
{
	// Only add if this neighboring node ID doesn't already exists in the list
	if (adjList[fromID].find(neighborNode.id) == adjList[fromID].end())
		adjList[fromID][neighborNode.id] = neighborNode;
}

void graph::print()
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

//! @note Right now this is taking only one end vertex. If we want to enable multiple end vertices, we could still
//!	 keep essentially the same algorithm but consider the last vertex to be the maximum of all listed end vertices.
//!	 For each given end vertex, we would reverse iterate through the list of preceding nodes. We would then have a 
//!	 list of a list of the shortest paths to each given end vertex, for each given end vertex.
std::pair< std::vector<int>, int> graph::breadthFirstSearch(int start, int end)
{
	const int INFINITY = std::numeric_limits<int>::max();
	std::vector<int> shortestPath;	// Nodes from start to end with the shortest path
	int minCapacity = INFINITY;		// Minimum weight (capacity) along the shortest path

	// Verify that the start node and end node are within acceptable ranges
	if ( ((start < 0) || (start > numNodes)) || ((end < 0) || (end > numNodes)) )
		return std::make_pair(shortestPath, minCapacity);

	// Assign the shortest distance predecessor for all nodes (except our starting point - source) to be infinity. The 
	// edge weights within the shortest paths is contained within pathWeights.
	int paths[numNodes];
	int pathWeights[numNodes];
	std::fill_n(paths, numNodes, INFINITY);
	std::fill_n(pathWeights, numNodes, INFINITY);
	paths[start] = -1;
	pathWeights[start] = 0;

	// Use breadth first search to find the end vertex with the shortest path
	//! @note Chose a queue for the nodes that still need visited in order to use the FIFO behavior of BFS.
	//! @note Chose a vector for the return value (path from shortest to end) in order to easily reverse the order
	//!		once the computation is complete.
	std::queue<int> nodesToVisit; // Nodes that still need visited in the BFS
	nodesToVisit.push(start);
	while (true)
	{	
		// If we can't find the end vertex, return an empty list
		if (nodesToVisit.empty())
			return std::make_pair(shortestPath, minCapacity);

		// Get the top-most element from the queue
		int currentNode = nodesToVisit.front();
		nodesToVisit.pop();

		// Stop searching if the desired end vertex has been found
		if (currentNode == end)
			break;

		// Obtain all of the current node's neighbors. If it has no neighbors, skip to the next node in the queue
		if (currentNode >= (int) adjList.size())
			continue;
		std::map<int, vertex> neighbors = adjList[currentNode];
		for (unsigned int i = 0; i < neighbors.size(); ++i)
		{
			int neighbor = neighbors[i].id;

			// Keep track of how we got to these neighbors for the shortest path, but DON'T OVER-WRITE if it's already 
			// been found! This preserves the minimal path in terms of number of edges. 
			if (paths[neighbor] == INFINITY) {
				
				// Set the neighbor with the node that connects it, including the edge weight
				paths[neighbor] = currentNode;
				pathWeights[neighbor] = neighbors.at(i).weight;
				
				// Add the neighbors to the bottom of the queue to visit later
				nodesToVisit.push(neighbor);
			}
		}
	}

	// Find path to the end node by back-track through the paths list until we find the given start node.
	int currentNode = end;
	while (true)
	{
		// Loop has completed if we're back at the starting point
		shortestPath.push_back(currentNode);
		if (currentNode == start)
			break;

		// Check if this edge is smaller than our minimum capacity
		if (pathWeights[currentNode] < minCapacity)
			minCapacity = pathWeights[currentNode];

		currentNode = paths[currentNode];
	}
	std::reverse(shortestPath.begin(), shortestPath.end());

	//! @note Used this for testing just to make sure it was correct :) This can be removed after we add test cases
	/*
	std::cout << "Shortest path obtained:" << std::endl;
	for (unsigned int i = 0; i < shortestPath.size(); ++i)
	{
		if (i == shortestPath.size() - 1)
			std::cout << shortestPath.at(i) << std::endl;
		else
			std::cout << shortestPath.at(i) << " --> ";
	}
	*/
	return std::make_pair(shortestPath, minCapacity);	
}

int graph::fordFulkerson()
{
	int maxFlow = 0;

	while (true) {
		std::pair< std::vector<int>, int>  bfsResult = breadthFirstSearch(0, numNodes-1);

		// FF is done!
		if (bfsResult.first.empty()) {
			break;
		}
	}

	return maxFlow;
}