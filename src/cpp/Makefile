COMPILER= g++
FLAGS= -std=c++11 -g -Wall -Wsign-compare -Wreturn-type -pedantic
LIBS= -lm -lasound -ljack -lportaudio
INCLUDE= -I headers -I private-headers

TEST_INCLUDE= -I tests
TIMING= time

.PHONY: all
all : AudioEngine.o test

AudioEngine.o : AudioEngine.cpp headers/*hpp private-headers/*hpp
	$(COMPILER) $(FLAGS) $(INCLUDE) -c AudioEngine.cpp $(LIBS) -o AudioEngine.o

.PHONY: test
test: test.out
	# Everything must pass:
	./test.out
	# Everything not tagged performance must pass in valgrind,
	# and there can be no memory errors
	$(TIMING) valgrind --leak-check=full ./test.out

test.out: AudioEngine.o tests/*.hpp tests/*.cpp
	$(COMPILER) $(FLAGS) $(INCLUDE) $(TEST_INCLUDE) AudioEngine.o tests/*.cpp -o test.out $(LIBS)

.PHONY: clean
clean:
	rm test.out AudioEngine.o
