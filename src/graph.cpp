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
graph::graph(std::string fileName)
{
	// Open the file containing the graph information
	std::ifstream input;
	input.open(fileName.c_str());
	if (!input)
	{
		std::cerr << "Could not open " << fileName << "!\n";
	}

	std::set<int> totalNodes;	// Unique list of all nodes added
	std::string line;
	int count = 0;
	while (getline(input, line))
	{
		// Obtain the list of connected vertices and their edge weights
		std::vector<vertex> connectedVertices = std::vector<vertex>();
		std::stringstream ss(line);
		while (ss)
		{
			vertex u;
			ss >> u.number; // The first # is the vertex number for u
			if (!ss) break; // If there's no second number, we're finished here
			ss >> u.weight; // The second # is the capacity/weight from v to u
			connectedVertices.push_back(u);
			totalNodes.insert(u.number);
		}
		adjacencyList.push_back(connectedVertices);
		totalNodes.insert(count++);
	}
	numNodes = totalNodes.size();
	input.close();
}

graph::~graph()
{
	adjacencyList.clear();
}

void graph::print()
{
	// Traverse each vertex u in the graph
	for (unsigned int u=0; u < adjacencyList.size(); ++u)
	{
		std::cout << u; // Current vertex
		std::vector<vertex>::iterator current = adjacencyList.at(u).begin();
		std::vector<vertex>::iterator end     = adjacencyList.at(u).end();
		
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

//! @note Right now this is taking only one end vertex. If we want to enable multiple end vertices, we could still
//!		keep essentially the same algorithm but consider the last vertex to be the maximum of all listed end vertices.
//!		For each given end vertex, we would reverse iterate through the list of preceding nodes. We would then have a 
//!		list of a list of the shortest paths to each given end vertex, for each given end vertex.
std::pair< std::vector<int>, int> graph::breadthFirstSearch(int start, int end)
{
	const int INFINITY = std::numeric_limits<int>::max();
	std::vector<int> shortestPath;	// Nodes from start to end with the shortest path
	int minCapacity = INFINITY;		// Minimum weight (capacity) along the shortest path

	// Verify that the start node and end node are within acceptable ranges
	if ( ((start < 0) || (start > numNodes)) || ((end < 0) || (end > numNodes)) )
		return std::make_pair(shortestPath, minCapacity);

	// Assign the shortest distance predecessor for all nodes (except our starting point - source) to be infinity.
	int paths[numNodes];
	std::fill_n(paths, numNodes, INFINITY);
	paths[start] = -1;

	// Use breadth first search to find the end vertex with the shortest path
	//! @note Chose a queue for the nodes that still need visited in order to use the FIFO behavior of BFS.
	//! @note Chose a vector for the return value (path from shortest to end) in order to easily reverse the order
	//!		once the computation is complete.
	std::queue<int> nodesToVisit;		// Nodes that need visited in the BFS
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
		if (currentNode >= (int) adjacencyList.size())
			continue;
		std::vector<vertex> neighbors = adjacencyList.at(currentNode);
		for (unsigned int i = 0; i < neighbors.size(); ++i)
		{
			int neighbor = neighbors.at(i).number;

			// Keep track of how we got to these neighbors for the shortest path, but DON'T
			// OVER-WRITE if it's already been found! This preserves the minimal path in terms of
			// number of edges. 
			if (paths[neighbor] == INFINITY) {
				
				// Set the neighbor with the node that
				paths[neighbor] = currentNode;
				
				// Add the neighbors to the bottom of the queue to visit later
				nodesToVisit.push(neighbor);
			}
		}
	}

	// Find path to the end node by back-track through the paths list until we find the given start node.
	int currentNode = end;
	int oldNode = currentNode;
	while (true)
	{
		shortestPath.push_back(currentNode);
		if (currentNode == start)
		{
			// Check if this edge is smaller than our minimum capacity
			std::vector<vertex> connectedNodes = adjacencyList.at(start);
			std::vector<vertex>::iterator itr = connectedNodes.begin();
			while (itr != connectedNodes.end())
			{
				if ((itr->number == oldNode) && (itr->weight < minCapacity))
				{
					minCapacity = itr->weight;
					break;
				}
				++itr;
			}
			break;
		}
		oldNode = currentNode;
		currentNode = paths[currentNode];

		// Check if this edge is smaller than our minimum capacity
		std::vector<vertex> connectedNodes = adjacencyList.at(currentNode);
		std::vector<vertex>::iterator itr = connectedNodes.begin();
		while (itr != connectedNodes.end())
		{
			if ((itr->number == oldNode) && (itr->weight < minCapacity))
			{
				minCapacity = itr->weight;
				break;
			}
			++itr;
		}
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
