/*
	@copydoc pgm.hpp
*/

#include "pgm.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

pgm::pgm() {}

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

int pgm::threshold()
{
	long int nodeSum = 0;
	for (int xPos = 0; xPos < xMax; ++xPos)
		for (int yPos = 0; yPos < yMax; ++yPos)
			nodeSum += matrix[xPos][yPos];
	
	return std::abs( pixMax - (nodeSum / (xMax * yMax)) );
}
