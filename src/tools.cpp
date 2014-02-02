/*
	@copydoc tools.hpp
*/

#include "tools.hpp"
#include <queue>
#include <sstream>
#include <fstream>
#include <iostream>

namespace tools {

	void graphFromFile(const char* file, graph& g) {
	 	// Open the file containing the graph information
		std::ifstream input;
		input.open(file);
		if (!input)
			std::cerr << "Could not open file: " << file << "\n";

		std::string line;
		int count = -1;
		while (getline(input, line)) {
			g.addNode(++count);

			// Obtain the list of connected vertices and their edge weights
			std::stringstream ss(line);
			while (ss) {
				vertex u;
				ss >> u.id; 	// The first # is the vertex id for u
				if (!ss) break; // If there's no second number, we're finished here
				ss >> u.weight; // The second # is the capacity/weight from v to u
				g.addNeighbor(count, u);
			}
		}
		input.close();
	}

	void graphFromPGM(const char* file, graph& g) {
		std::ifstream input;
		input.open(file);
		if (!input)
			std::cerr << "Could not open file: " << file << "\n";

		int x = 0, y = 0, max = 0;
		std::string line;

		getline(input, line);	// "P2"
		getline(input, line);	// "#Created by Irfan View"
		getline(input, line);	// X, Y
		std::stringstream ssXY(line);
		ssXY >> x >> y;
		int matrix[x][y];

		getline(input, line);	// Max
		std::stringstream ssMax(line);
		ssMax >> max;

		int xPos = 0, yPos = 0;
		while (getline(input, line)) {
			std::stringstream ss(line);
			for (xPos = 0; xPos < x; ++xPos) {
				ss >> matrix[xPos][yPos];
				g.addNode( (x * yPos) + xPos );	// Unique ID, ranging [0, X*Y]
			}
			++yPos;
		}
		input.close();

		// Add paths between nodes
		for (xPos = 0; xPos < x; ++xPos) {
			for (yPos = 0; yPos < y; ++yPos) {
				int currentID = (x * yPos) + xPos;

				// [xPos - 1][yPos] - Left
				if (xPos > 0) {
					vertex lNeighbor; 
					lNeighbor.id 	 = (x * yPos) + (xPos - 1);
					lNeighbor.weight = std::abs( max - std::abs( matrix[xPos - 1][yPos] - matrix[xPos][yPos]) );
					g.addNeighbor(currentID, lNeighbor);
				}

				// [xPos + 1][yPos] - Right
				if (xPos < (x - 1)) {
					vertex rNeighbor;
					rNeighbor.id 	 = (x * yPos) + (xPos + 1);
					rNeighbor.weight = std::abs( max - std::abs( matrix[xPos + 1][yPos] - matrix[xPos][yPos]) );
					g.addNeighbor(currentID, rNeighbor);
				}

				// [xPos][yPos - 1] - Top
				if (yPos > 0) {
					vertex tNeighbor; 	
					tNeighbor.id 	 = (x * (yPos - 1) + xPos);
					tNeighbor.weight = std::abs( max - std::abs( matrix[xPos][yPos - 1] - matrix[xPos][yPos]) );
					g.addNeighbor(currentID, tNeighbor);
				}

				// [xPos][yPos + 1] - Bottom
				if (yPos < (y - 1)) {
					vertex bNeighbor;
					bNeighbor.id 	 = (x * (yPos + 1) + xPos);
					bNeighbor.weight = std::abs( max - std::abs( matrix[xPos][yPos + 1] - matrix[xPos][yPos]) );
					g.addNeighbor(currentID, bNeighbor);
				}
			}
		}
	}

	uint32_t xorshift() {
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
	std::pair< std::vector<int>, int> breadthFirstSearch(graph& g, int start, int end) {
		const int INFINITY = std::numeric_limits<int>::max();
		std::vector<int> shortestPath;	// Nodes from start to end with the shortest path
		int minCapacity = INFINITY;		// Minimum weight (capacity) along the shortest path

		// Verify that the start node and end node are within acceptable ranges
		int numNodes = g.nodes();
		if ( ((start < 0) || (start > numNodes)) || ((end < 0) || (end > numNodes)) ) {
			g.print();
			return std::make_pair(shortestPath, minCapacity);
		}

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
		while (true) {	
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
			if (currentNode >= g.adjList.size())
				continue;
			
			std::map<int, vertex> neighbors = g.adjList[currentNode];
			for (unsigned int i = 0; i < neighbors.size(); ++i) {
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
		while (true) {
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
		std::cerr << "Shortest path obtained:" << std::endl;
		for (unsigned int i = 0; i < shortestPath.size(); ++i)
		{
			if (i == shortestPath.size() - 1)
				std::cerr << shortestPath.at(i) << std::endl;
			else
				std::cerr << shortestPath.at(i) << " --> ";
		}
		*/
		return std::make_pair(shortestPath, minCapacity);	
	}

	int fordFulkerson(graph& g, int source, int sink) {
		int maxFlow  = 0;
		while (true) {

			// This needs to be fixed
			std::pair< std::vector<int>, int>  bfsResult = breadthFirstSearch(g, source, sink);

			// FF is done!
			if (bfsResult.first.empty()) {
				break;
			}

			for (unsigned int i = 0; i < bfsResult.first.size() - 1; ++i) {
				int sNode = bfsResult.first[i];
				int nNode = bfsResult.first[i+1];

				if (g.adjList[sNode][nNode].weight - bfsResult.second > 0) {				
					g.adjList[sNode][nNode].weight = g.adjList[sNode][nNode].weight - bfsResult.second;
					vertex v;
					v.id = i;
					v.weight = bfsResult.second;
					g.adjList[nNode][sNode] = v;
				}
				else {
					vertex v;
					v.id = sNode;
					v.weight = g.adjList[sNode][nNode].weight;
					g.adjList[nNode][sNode] = v;
					g.adjList[sNode].erase(nNode);
				}
			}

			maxFlow = maxFlow + bfsResult.second;

			/* Debuggin to Show Graph States
			g.print();
			std::cerr << "----------\n";
			*/
			
		}

		std::pair< std::set<int>, std::set<int> > cut = minCut(g, source, sink);
		return maxFlow;
	}

	void reverse(graph& g) {
		graph reverse;

		std::map< int, std::map<int, vertex> >::iterator adjListItr = g.adjList.begin();
		std::map< int, std::map<int, vertex> >::iterator adjListEnd = g.adjList.end();
		while (adjListItr != adjListEnd) {
			vertex fromNode;
			fromNode.id = (*adjListItr).first;

			std::map<int, vertex>::iterator neighborItr = (*adjListItr).second.begin();
			std::map<int, vertex>::iterator neighborEnd = (*adjListItr).second.end();
			while (neighborItr != neighborEnd) {
				vertex neighbor = (*neighborItr).second;
				fromNode.weight = neighbor.weight;
				reverse.addNeighbor(neighbor.id, fromNode);
				++neighborItr;
			}
			++adjListItr;
		}
		g = reverse;
	}

	std::pair< std::set<int>, std::set<int> > minCut(graph& g, int source, int sink) {
		// Obtain the nodes within the sets S and T afer the minimum cut
		std::set<int> S; // All nodes within the set S; nodes u such that u is reachable by source s
		std::set<int> T; // All nodes within the set T; nodes u such that u is not reachable by source s

		graph reverse = g;
		tools::reverse(reverse);

		//! @note We don't care about the shortest path here, so using BFS does more than we care for - but it checks
		//!	if a path exists.
		for (int i = 0; i <= reverse.sNodes.size(); ++i) {
			if (i == source)
				S.insert(i);
			if (i == sink)
				T.insert(i);

			std::vector<int> shortestPath = breadthFirstSearch(reverse, source, i).first;
			if (shortestPath.size() > 0)
				S.insert(i);
			else
				T.insert(i);
		}

		/* Used for debugging purposes */
		std::cerr << "Elements of S: {";
		for (std::set<int>::iterator itr = S.begin(); itr != S.end(); ++itr)
			std::cerr << (*itr) << ",";	
		std::cerr << "}\n";
		std::cerr << "Elements of T: {";
		for (std::set<int>::iterator itr = T.begin(); itr != T.end(); ++itr)
			std::cerr << (*itr) << ",";
		std::cerr << "}\n";

		return std::make_pair< std::set<int>, std::set<int> >(S,T);
	}

	void segmentImage(const char* file, const char* cut) {
		// Read PGM from file as a graph.
		graph g;
		graphFromPGM(file, g);
		g.print();

		// Obtain the average of all path weights. For each path, accumulate the sum of the weights. Divide by the
		//	number of paths - I think this will end up being (X - 1) * (Y - 1)
		// Probability that any given pixel is within the foreground or background is how it relates to the average
		//	of all weights

		// Add source and sink
		// 	s = 0 (source value)
		// 	t = maximum from PGM, 255 (sink value)

		// Something with ford fulkerson here ??

		// Obtain min cut
		//	Everything within S (is reachable from s) is in the foreground
		//	Everything within T (not reachable from s) is in the background
		//	@note - Are we checking if it's reachable from s given a minimum threshold (ie, the probability)

		// Open the output file, represented by name in cut parameter
		//	Write header PGM for X,Y,Max values
		//	Iterate through all nodes
		//		If that node is within S, write source value (s = 0)
		//		Else, write sink value (t = max, 255)
		// Close output file
	}
}

