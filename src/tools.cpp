/*
	@copydoc tools.hpp
*/

#include "tools.hpp"
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

	int fordFulkerson(graph& g, int source, int sink) {
		int maxFlow  = 0;

		while (true) {

			// This needs to be fixed
			std::pair< std::vector<int>, int>  bfsResult = g.breadthFirstSearch(source, sink);

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
			print();
			std::cerr << "----------\n";
			*/
		}

		std::cerr << "Max Flow: " << maxFlow << "\n";
		return maxFlow;
	}
}