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

namespace tools 
{
	void graphFromFile(const char* file, graph& g) 
	{
	 	// Open the file containing the graph information
		std::ifstream input;
		input.open(file);
		if (!input)
			std::cerr << "Could not open file: " << file << "\n";

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

	uint32_t xorshift() 
	{
		static uint32_t x = 123456789;
	  	static uint32_t y = 362436069;
	  	static uint32_t z = 521288629;
	  	static uint32_t w = 88675123;
	  	uint32_t t;
	 
	  	t = x ^ (x << 11);
	  	x = y; y = z; z = w;
	  	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
	}

	//! @note Right now this is taking only one end vertex. If we want to enable multiple end vertices, we could still
	//!	 keep essentially the same algorithm but consider the last vertex to be the maximum of all listed end vertices.
	//!	 For each given end vertex, we would reverse iterate through the list of preceding nodes. We would then have a 
	//!	 list of a list of the shortest paths to each given end vertex, for each given end vertex.
	std::pair< std::vector<int>, int> breadthFirstSearch(graph& g, int start, int end)
	{
		static int infinity = std::numeric_limits<int>::max();
		std::vector<int> shortestPath;	// Nodes from start to end with the shortest path
		int minCapacity = infinity;		// Minimum weight (capacity) along the shortest path

		// Verify that the start node and end node are within acceptable ranges
		int numNodes = g.nodes();
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

			// Obtain all of the current node's neighbors. If it has no neighbors, skip to the next node in the queue
			if ((unsigned int)currentNode >= g.adjList.size())
				continue;
			
			std::map<int, vertex> neighbors = g.adjList[currentNode];
			for (unsigned int i = 0; i < neighbors.size(); ++i)
			{
				int neighbor = neighbors[i].id;

				// Keep track of how we got to these neighbors for the shortest path, but DON'T OVER-WRITE if it's already 
				// been found! This preserves the minimal path in terms of number of edges. 
				if (paths[neighbor] == infinity)
				{
					
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
		return std::make_pair(shortestPath, minCapacity);	
	}

	int fordFulkerson(graph& g, int source, int sink)
	{
		int maxFlow  = 0;

		std::pair< std::vector<int>, int>  bfsResult = breadthFirstSearch(g, source, sink);
		while (!bfsResult.first.empty())
		{
			for (unsigned int i = 0; i < bfsResult.first.size() - 1; ++i)
			{
				int sNode = bfsResult.first[i];
				int nNode = bfsResult.first[i+1];

				if (g.adjList[sNode][nNode].weight - bfsResult.second > 0)
				{				
					// perform the difference between the min capacity and the edge weight
					g.adjList[sNode][nNode].weight = g.adjList[sNode][nNode].weight - bfsResult.second;

					// update graph back edge
					g.adjList[nNode][sNode].id = i;
					g.adjList[nNode][sNode].weight = g.adjList[nNode][sNode].weight + bfsResult.second;
				}
				else
				{
					// update graph back edge
					g.adjList[nNode][sNode].id = sNode;
					g.adjList[nNode][sNode].weight = g.adjList[nNode][sNode].weight + g.adjList[sNode][nNode].weight;

					// rempve the maxed out edge
					g.adjList[sNode].erase(nNode);
				}
			}

			maxFlow = maxFlow + bfsResult.second;
			bfsResult = breadthFirstSearch(g, source, sink);
		}
		return maxFlow;
	}

	void segmentImage(const char* file, const char* cut)
	{
		// Read PGM file as a graph
		std::ifstream input;
		input.open(file);
		if (!input)
			std::cerr << "Could not open file: " << file << "\n";

		int x = 0, y = 0, max = 0;
		std::string line;
		getline(input, line);	// "P2"
		getline(input, line);	// "#Created by Irfan View"

		std::stringstream ss;
		ss << input.rdbuf();
		ss >> x >> y;			// X, Y
		ss >> max;				// Max

		int matrix[x][y];
		for (int yPos = 0; yPos < y; ++yPos)
			for (int xPos = 0; xPos < x; ++xPos)
				ss >> matrix[xPos][yPos];
		input.close();

		// Get the average of all edges.
		long int nodeSum = 0;
		for (int xPos = 0; xPos < x; ++xPos)
			for (int yPos = 0; yPos < y; ++yPos)
				nodeSum += matrix[xPos][yPos];
		int threshold = std::abs( max - (nodeSum / (x*y)) );

		// Add paths between nodes
		graph g;
		for (int xPos = 0; xPos < x; ++xPos)
		{
			for (int yPos = 0; yPos < y; ++yPos)
			{
				int currentID = (x * yPos) + xPos;
				g.addNode(currentID);

				if (xPos > 0)		// [xPos - 1][yPos] - Left
				{
					int weight = std::abs( max - std::abs( matrix[xPos - 1][yPos] - matrix[xPos][yPos]) );
					if (weight >= threshold)
					{
						vertex lNeighbor; 
						lNeighbor.id 	 = (x * yPos) + (xPos - 1);
						lNeighbor.weight = weight;
						g.addNeighbor(currentID, lNeighbor);
					}
				}

				if (xPos < (x - 1))	// [xPos + 1][yPos] - Right
				{
					int weight = std::abs( max - std::abs( matrix[xPos + 1][yPos] - matrix[xPos][yPos]) );
					if (weight >= threshold)
					{
						vertex rNeighbor;
						rNeighbor.id 	 = (x * yPos) + (xPos + 1);
						rNeighbor.weight = weight;
						g.addNeighbor(currentID, rNeighbor);
					}
				}
				
				if (yPos > 0)		// [xPos][yPos - 1] - Top
				{
					int weight = std::abs( max - std::abs( matrix[xPos][yPos - 1] - matrix[xPos][yPos]) );
					if (weight >= threshold)
					{
						vertex tNeighbor; 	
						tNeighbor.id 	 = (x * (yPos - 1) + xPos);
						tNeighbor.weight = weight;
						g.addNeighbor(currentID, tNeighbor);
					}
				}

				if (yPos < (y - 1))	// [xPos][yPos + 1] - Bottom
				{
					int weight = std::abs( max - std::abs( matrix[xPos][yPos + 1] - matrix[xPos][yPos]) );
					if (weight >= threshold)
					{
						vertex bNeighbor;
						bNeighbor.id 	 = (x * (yPos + 1) + xPos);
						bNeighbor.weight = weight;
						g.addNeighbor(currentID, bNeighbor);
					}
				}
			}
		}

		// Add source and sink, where s = 0 (source value), and t = maximum from PGM, usually 255 (sink value)
		int sourceID = -1;
		int sinkID   = x*y;
		g.addNode(sourceID);
		g.addNode(sinkID);
		for (int xPos = 0; xPos < x; ++xPos)
		{
			for (int yPos = 0; yPos < y; ++yPos)
			{
				if (std::abs( max - matrix[xPos][yPos]) >= threshold)
				{	
					vertex fromS;
					fromS.id = (x * yPos) + xPos;
					fromS.weight = std::abs( max - matrix[xPos][yPos]);
					g.addNeighbor(sourceID, fromS);
				}

				if (matrix[xPos][yPos] >= threshold)
				{
					vertex toT;
					toT.id = sinkID;
					toT.weight = matrix[xPos][yPos];
					int fromID = (x * yPos) + xPos;
					g.addNeighbor(fromID, toT);
				}
			}
		}

		// Run Ford Fulkerson on the pgm graph
		fordFulkerson(g, sourceID, sinkID);

		// Write to output file
		std::ofstream output;
		output.open(cut);
		if (!output)
			std::cerr << "Could not open file: " << cut << "\n";

		output << "P2\n# Created by IrfanView\n" << x <<" " << y << "\n" << max << "\n";
		for (int yPos = 0; yPos < y; yPos++)
		{
			for (int xPos = 0; xPos < x; xPos++)
			{
				int nodeID = (x * yPos) + xPos;
				if (g.adjList[sourceID].find(nodeID) == g.adjList[sourceID].end())
					output << max << " ";
				else
					output << matrix[xPos][yPos] << " ";
			}
			output << "\n";
		}
		output.close();
	}
}

