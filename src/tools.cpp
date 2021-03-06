/*
	@copydoc tools.hpp
*/

#include "tools.hpp"
#include <stdint.h>
#include <limits>
#include <queue>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace Tools 
{
	void graphFromFile(const char* file, Graph& g) 
	{
	 	// Open the file containing the graph information
		std::ifstream input;
		input.open(file);
		if (!input) {
			std::cerr << "Could not open file: " << file << "\n";
			exit(1);
		}

		std::string line;
		int count = -1;
		while (getline(input, line))
		{
			g.addNode(++count);

			// Obtain the list of connected vertices and their edge weights
			std::stringstream ss(line);
			while (ss)
			{
				vertex u;
				ss >> u.id; 	// The first # is the vertex id for u
				if (!ss) break; // If there's no second number, we're finished here
				ss >> u.weight; // The second # is the capacity/weight from v to u
				g.addNeighbor(count, u);
			}
		}
		input.close();
	}

	//! @note Right now this is taking only one end vertex. If we want to enable multiple end vertices, we could still
	//!	 keep essentially the same algorithm but consider the last vertex to be the maximum of all listed end vertices.
	//!	 For each given end vertex, we would reverse iterate through the list of preceding nodes. We would then have a 
	//!	 list of a list of the shortest paths to each given end vertex, for each given end vertex.
	std::pair< std::vector<int>, int> breadthFirstSearch(Graph& g, int start, int end)
	{
		static int infinity = std::numeric_limits<int>::max();
		std::vector<int> shortestPath;	// Nodes from start to end with the shortest path
		int minCapacity = infinity;		// Minimum weight (capacity) along the shortest path
		int numNodes = g.nodes();

		// Start and end node are the same? Capacity is zero and shortest path is itself.
		if ( start == end) {
			shortestPath.push_back(start);
			return std::make_pair(shortestPath, 0);
		}

		// Verify valid start and end is given
		if ( ((start < 0) || (start > numNodes)) || ((end < 0) || (end > numNodes)) )
  			return std::make_pair(shortestPath, minCapacity);

		// Assign the shortest distance predecessor for all nodes (except our starting point - source) to be infinity. The 
		// edge weights within the shortest paths is contained within pathWeights.
		int paths[numNodes];
		int pathWeights[numNodes];
		std::fill_n(paths, numNodes, infinity);
		std::fill_n(pathWeights, numNodes, infinity);
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
			
			std::map<int, vertex> neighbors = g.adjList[currentNode];
			std::map<int, vertex>::iterator neighborsItr = neighbors.begin();
			std::map<int, vertex>::iterator neighborsEnd = neighbors.end();
			while (neighborsItr != neighborsEnd)
			{
				int neighbor = (*neighborsItr).first;
				if (paths[neighbor] == infinity)
				{
					// Set the neighbor with the node that connects it, including the edge weight
					paths[neighbor] = currentNode;
					pathWeights[neighbor] = (*neighborsItr).second.weight;
					
					// Add the neighbors to the bottom of the queue to visit later
					nodesToVisit.push(neighbor);
				}
				++neighborsItr;
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
		return std::make_pair(shortestPath, minCapacity);	
	}

	int fordFulkerson(Graph& g, int source, int sink)
	{
		// Starting flow is zero.
		int maxFlow  = 0;

		// Get shortest path for processing with Forf Fulkerson.
		// 	Loop until a shortest path cannot be found.
		std::pair< std::vector<int>, int>  bfsResult = breadthFirstSearch(g, source, sink);
		
		// Empty indicates no shortest path from S to T (source to Sink)
		while (!bfsResult.first.empty() && bfsResult.second != std::numeric_limits<int>::max())
		{
			// Examine each node in the path and adjust the edges of the residual graph
			for (unsigned int i = 0; i < bfsResult.first.size() - 1; ++i)
			{
				// Take nodes in pairs to adjust the edges between them.
				int startNode = bfsResult.first[i]; // start node in pair
				int endNode = bfsResult.first[i+1]; // end node in pair

				if (g.adjList[startNode][endNode].weight - bfsResult.second > 0)
				{				
					// perform the difference between the min capacity and the edge weight
					// 	update the weight with the result
					g.adjList[startNode][endNode].weight = g.adjList[startNode][endNode].weight - bfsResult.second;

					// add/update graph back edge
					g.adjList[endNode][startNode].id = i;
					g.adjList[endNode][startNode].weight = g.adjList[endNode][startNode].weight + bfsResult.second;
				}
				else
				{
					// add/update graph back edge
					g.adjList[endNode][startNode].id = startNode;
					g.adjList[endNode][startNode].weight = g.adjList[endNode][startNode].weight + g.adjList[startNode][endNode].weight;

					// rempve the maxed out edge
					g.adjList[startNode].erase(endNode);
				}
			}

			// Accumulate the flow to return the maximum flow
			maxFlow = maxFlow + bfsResult.second;

			// Perform next BFS operation to find next shortest path in the residual graph
			bfsResult  = breadthFirstSearch(g, source, sink);
		}
		return maxFlow;
	}

	void segmentImage(const char* file, const char* cut)
	{
		Pgm p;

		if (!p.fromFile(file))
			return;

		p.calculateThreshold();
		p.addPaths();

		int sourceID = -1;
		int sinkID   = p.xMax * p.yMax;
		p.addSuperNodes(sourceID, sinkID);

		// Run Ford Fulkerson on the pgm graph
		fordFulkerson(p.g, sourceID, sinkID);

		// Write to output file
		p.write(cut, sourceID);
	}
}

