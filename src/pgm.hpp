#pragma once

#include "graph.hpp"

//! @brief Container for a PGM image
class pgm
{
public:
	//! @brief Basic constructor
	pgm();

	//! @brief Basic destructor
	~pgm();

	//! @brief Construct from a file
	//! @param file Path to the PGM file
	//! @retval true if successful, false otherwise
	bool fromFile(const char* file);


	int **matrix;	// Matrix constructed of all pixels
	graph g;		// Graph of all nodes representing the pixels
	int xMax;		// Maximum x value
	int yMax;		// Maximum y value
	int pixMax;		// Maximum pixel value
};
