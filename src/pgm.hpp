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

	//! @brief Gets the threshold for the file
	//! @param The average of all nodes - constituting the threshold
	int calculateThreshold();

	//! @brief Add paths between all pixels
	void addPaths();


	int **matrix;	// Matrix constructed of all pixels
	graph g;		// Graph of all nodes representing the pixels
	int xMax;		// Maximum x value
	int yMax;		// Maximum y value
	int pixMax;		// Maximum pixel value
	int threshold;	// Threshold value for the min cut
};
