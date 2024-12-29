#include <gtest/gtest.h>

#include <iostream>

#include "Q1_Trie.h"
#include "Q2_Debug.h"

int main(int argc, char **argv) {
	if (false)	// Set to false to run unit-tests
	{
		// Debug section: Place your debugging code here
	} else {
		int x = -1;
		int *arr = NULL;

		// Example 1: Checking if x is positive
		DEBUG_LOG(x > 0, "ERROR", "x should be positive");

		// Example 2: Checking for null pointers
		DEBUG_LOG(arr != NULL, "WARNING", "Array pointer is NULL.");

		// Example 3: Logical error
		DEBUG_LOG(1 == 0, "ERROR", "This condition should never be true.");

		::testing::InitGoogleTest(&argc, argv);
		std::cout << "RUNNING TESTS ..." << std::endl;
		int ret{RUN_ALL_TESTS()};
		if (!ret)
			std::cout << "<<<SUCCESS>>>" << std::endl;
		else
			std::cout << "FAILED" << std::endl;
	}
	return 0;
}