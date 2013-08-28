#include <iostream>
#include <time.h>

#include <functional>

/// returns the number of milliseconds that it took to run the function
double test_time( std::function<void (void)> action ){

	timespec start, end;
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	action();
	clock_gettime(CLOCK_MONOTONIC, &end);

	// seconds * 1000 = milliseconds
	// nano seconds / 1000000 = milliseconds
	return (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

/*
	std::cout << "clock tick / second: " << CLOCKS_PER_SEC << std::endl;
	clock_t start = clock();
	action();
	clock_t end = clock();
	return (end - start)*1.0/CLOCKS_PER_SEC;

*/
}