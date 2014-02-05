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

	//! @brief Adds super source and super sink
	//! @param sourceID The node ID of the source
	//! @param sinkID	The node ID of the sink
	void addSuperNodes(int sourceID, int sinkID);

	//! @brief Write a cut PGM given a source ID to start the segmentation
	//! @param file The path to the file that will be written to
	//! @param sourceID The source of the PGM
	//! @retval true if successful, false otherwise
	bool write(const char* file, int sourceID);


	int **matrix;	// Matrix constructed of all pixels
	graph g;		// Graph of all nodes representing the pixels
	int xMax;		// Maximum x value
	int yMax;		// Maximum y value
	int pixMax;		// Maximum pixel value
	int threshold;	// Threshold value for the min cut
};
