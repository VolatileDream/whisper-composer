COMPILER= g++
FLAGS= -std=c++11 -g -Wall -Wsign-compare -Wreturn-type -pedantic
LIBS= -lm -lasound -ljack -lportaudio
INCLUDE= -I headers -I private-headers

TEST_INCLUDE= -I tests
TIMING= time

OUTPUT= ../../lib/

.PHONY: all
all : $(OUTPUT)AudioEngine.o test $(OUTPUT)main

$(OUTPUT)main : $(OUTPUT)AudioEngine.o main.cpp
	$(COMPILER) $(FLAGS) $(INCLUDE) AudioEngine.o main.cpp $(LIBS) -o $(OUTPUT)main

$(OUTPUT)AudioEngine.o : AudioEngine.cpp headers/*hpp private-headers/*hpp
	$(COMPILER) $(FLAGS) $(INCLUDE) -c AudioEngine.cpp $(LIBS) -o $(OUTPUT)AudioEngine.o

.PHONY: test
test: $(OUTPUT)test.o
	# Must pass:
	$(OUTPUT)test.o
	# Can't produce memory warnings:
	$(TIMING) valgrind --leak-check=full $(OUTPUT)test.o

$(OUTPUT)test.o: $(OUTPUT)AudioEngine.o tests/*.hpp tests/*.cpp
	$(COMPILER) $(FLAGS) $(INCLUDE) $(TEST_INCLUDE) $(OUTPUT)AudioEngine.o tests/*.cpp -o $(OUTPUT)test.o $(LIBS)

.PHONY: clean
clean:
	rm $(OUTPUT)test.o $(OUTPUT)AudioEngine.o $(OUTPUT)main
