/*
	@copydoc pgm.hpp
*/

#include "pgm.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

pgm::pgm() : xMax(0), yMax(0), pixMax(0), threshold(0) {

}

pgm::~pgm() {}

bool pgm::fromFile(const char* file)
{
	std::ifstream input;
	input.open(file);
	if (!input)
	{
		std::cerr << "Could not open file: " << file << "\n";
		return false;
	}

	std::string line;
	getline(input, line);	// "P2"
	getline(input, line);	// "#Created by Irfan View"

	std::stringstream ss;
	ss << input.rdbuf();
	ss >> xMax >> yMax;		// X, Y
	ss >> pixMax;			// Max

	for (int yPos = 0; yPos < yMax; ++yPos)
		for (int xPos = 0; xPos < xMax; ++xPos)
			ss >> matrix[xPos][yPos];
	input.close();

	return true;
}

int pgm::calculateThreshold()
{
	long int nodeSum = 0;
	for (int xPos = 0; xPos < xMax; ++xPos)
		for (int yPos = 0; yPos < yMax; ++yPos)
			nodeSum += matrix[xPos][yPos];
	
	threshold = std::abs( pixMax - (nodeSum / (xMax * yMax)) );
	return threshold;
}

void pgm::addPaths()
{
	for (int xPos = 0; xPos < xMax; ++xPos)
	{
		for (int yPos = 0; yPos < yMax; ++yPos)
		{
			int currentID = (xMax * yPos) + xPos;
			g.addNode(currentID);

			if (xPos > 0)		// [xPos - 1][yPos] - Left
			{
				int weight = std::abs( pixMax - std::abs( matrix[xPos - 1][yPos] - matrix[xPos][yPos]) );
				if (weight >= threshold)
				{
					vertex lNeighbor; 
					lNeighbor.id 	 = (xMax * yPos) + (xPos - 1);
					lNeighbor.weight = weight;
					g.addNeighbor(currentID, lNeighbor);
				}
			}
			if (xPos < (xMax - 1))	// [xPos + 1][yPos] - Right
			{
				int weight = std::abs( pixMax - std::abs( matrix[xPos + 1][yPos] - matrix[xPos][yPos]) );
				if (weight >= threshold)
				{
					vertex rNeighbor;
					rNeighbor.id 	 = (xMax * yPos) + (xPos + 1);
					rNeighbor.weight = weight;
					g.addNeighbor(currentID, rNeighbor);
				}
			}
			if (yPos > 0)		// [xPos][yPos - 1] - Top
			{
				int weight = std::abs( pixMax - std::abs( matrix[xPos][yPos - 1] - matrix[xPos][yPos]) );
				if (weight >= threshold)
				{
					vertex tNeighbor; 	
					tNeighbor.id 	 = (xMax * (yPos - 1) + xPos);
					tNeighbor.weight = weight;
					g.addNeighbor(currentID, tNeighbor);
				}
			}
			if (yPos < (yMax - 1))	// [xPos][yPos + 1] - Bottom
			{
				int weight = std::abs( pixMax - std::abs( matrix[xPos][yPos + 1] - matrix[xPos][yPos]) );
				if (weight >= threshold)
				{
					vertex bNeighbor;
					bNeighbor.id 	 = (xMax * (yPos + 1) + xPos);
					bNeighbor.weight = weight;
					g.addNeighbor(currentID, bNeighbor);
				}
			}
		}
	}
}

void pgm::addSuperNodes(int sourceID, int sinkID)
{
	g.addNode(sourceID);
	g.addNode(sinkID);
	for (int xPos = 0; xPos < xMax; ++xPos)
	{
		for (int yPos = 0; yPos < yMax; ++yPos)
		{
			if (std::abs( pixMax - matrix[xPos][yPos]) >= threshold)
			{	
				vertex fromS;
				fromS.id = (xMax * yPos) + xPos;
				fromS.weight = std::abs( pixMax - matrix[xPos][yPos]);
				g.addNeighbor(sourceID, fromS);
			}

			if (matrix[xPos][yPos] >= threshold)
			{
				vertex toT;
				toT.id = sinkID;
				toT.weight = matrix[xPos][yPos];
				int fromID = (xMax * yPos) + xPos;
				g.addNeighbor(fromID, toT);
			}
		}
	}
}


bool pgm::write(const char* file, int sourceID)
{
	std::ofstream output;
	output.open(file);
	if (!output)
	{
		std::cerr << "Could not open file: " << file << "\n";
		return false;
	}

	output << "P2\n";
	output << "# Created by IrfanView\n";
	output << xMax << " " << yMax << "\n";
	output << pixMax << "\n";

	for (int yPos = 0; yPos < yMax; yPos++)
	{
		for (int xPos = 0; xPos < xMax; xPos++)
		{
			int nodeID = (xMax * yPos) + xPos;
			if (g.adjList[sourceID].find(nodeID) == g.adjList[sourceID].end())
				output << pixMax << " ";
			else
				output << matrix[xPos][yPos] << " ";
		}
		output << "\n";
	}
	output.close();
	return true;
}

