/*
	@brief Executes a series of tests on CS-Image-Segmentation-Solver. This includes verifiation of results and
	 timing metrics on the breadth first search algorithm, Ford-Fulkerson, and the image segmentation solution.
	@author Drew Guarnera, Bharath Bogadamidi, Heather Michaud
	@version 0.9
	@copyright Copyright 2014 Guarnera, Bogadamidi, Michaud. All rights reserved.
*/

#include <assert.h>

//! @brief Q&D implementation of Marsaglia's xorshift random number generator. Rand() is way too slow to obtain
//!	 random numbers rapidly and in repetition. See http://en.wikipedia.org/wiki/Xorshift
//! @retval Pseudo-random number that passes the diehard tests.
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

//! @brief Entry point to test suite execution
//! @retval Error or success code
int main()
{

	return 0;
}