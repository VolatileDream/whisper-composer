#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <cstring>

int main( int argc, char* const argv[] )
{
	// global setup...

	// there are some arguments we want to
	// strip out, because they belong to us
	// and not to the Catch test suite.

	// this arg tells us to ignore failed tests
	const char* ignoreArg = "--ignore-failed";
	bool ignoringFailed = false;

	char** newArgs = new char*[argc];
	int  newIndex=0;

	for(int index=0; index < argc; index++){

		if( strcmp( ignoreArg, argv[index] ) == 0 ){
			ignoringFailed = true;
			continue;
		}

		newArgs[newIndex] = argv[index];
		newIndex++;
	}

	int result = Catch::Session().run( newIndex, newArgs );
	
	// global clean-up...

	delete [] newArgs;

	if( ignoringFailed ){
		// ignore any failed tests because
		// we're doing a memory test
		return 0;
	}

	return result;
}