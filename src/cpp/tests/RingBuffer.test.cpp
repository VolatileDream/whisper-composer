#include "catch.hpp"

#include "RingBuffer.hpp"

TEST_CASE( "RingBuffer allocate", "[create]" ){
	Whisper::RingBuffer<int> buffer(4);
}

TEST_CASE( "RingBuffer insert/remove", "[use]" ){
	int operationCount = 0;
	unsigned int size = 4;

	Whisper::RingBuffer<int> buffer(size);

	while( buffer.add(operationCount) ){
		operationCount++;
	}

	REQUIRE( operationCount == size - 1 );

	operationCount = 0;
	int tmp = 0;
	while( buffer.remove(tmp) ){
		operationCount++;
	}

	REQUIRE( operationCount == size - 1 );
}
