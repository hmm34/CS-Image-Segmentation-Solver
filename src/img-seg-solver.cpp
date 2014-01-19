/*
	@brief Demonstrates the ability to solve the image segmentation problem
		using the Ford-Fulkerson algorithm.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#include <stdio.h>	
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>


//! @brief Entry point to execute the img-seg-solver program
//! @param argc Number of arguments
//! @param argv List of arguments
//! @return Error or success code
int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "No options found!\n";
		return 1;
	}

	// Process CLI ARGs
	while(true) {
		int option = getopt(argc,argv, "bif");

		if (option == -1)
			break;

		// BFS Option
		if (option == 'b') {     
			// Check for at most three additional options
			if (optind + 2 >= argc) {
				std::cerr << "Invalid use of option -b\n";
				std::cerr << "Usage: -b [input file] [start vertex] [end vertex]\n";
				return 1;
			}
		
			// This will go to BFS Function
			std::string bfsInputGraphFileName = argv[optind];
			int startVertex = atoi(argv[optind+1]);
			int endVertex = atoi(argv[optind+2]);
		}

		// Ford-Fulkerson Option
		if (option == 'f') {
			// Check for at most one additional option
			if (optind >= argc) {
				std::cerr << "Invalid use of option -f\n";
				std::cerr << "Usage: -f [input file]\n";
				return 1;
			}

			// This will go to Ford Fulkerson Function
			std::string ffInputGraphFileName = argv[optind];
		}

		// Image Segmentation Option
		if (option == 'i') {
			// Check for at most two additional options
			if (optind + 1 >= argc) {
				std::cerr << "Invalid use of option -i\n";
				std::cerr << "Usage: -i [input file] [input file]\n";
				return 1;
			}

			// This will got to the Image Segmentation Function
			std::string inputImageFileA = argv[optind];
			std::string inputImageFileB = argv[optind+1];
		}
	}		

	return 0;
}