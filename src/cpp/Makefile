COMPILER= g++
FLAGS= -std=c++11 -g -Wall -Wsign-compare -Wreturn-type -pedantic
LIBS= -lm -lportaudio
INCLUDE= -I headers -I private-headers

TEST_INCLUDE= -I tests
TIMING= time

OUTPUT= ../../lib/

.PHONY: all
all : $(OUTPUT)AudioEngine.o test $(OUTPUT)main-cpp

$(OUTPUT)main-cpp : $(OUTPUT)AudioEngine.o main.cpp
	$(COMPILER) $(FLAGS) $(INCLUDE) $(OUTPUT)AudioEngine.o main.cpp $(LIBS) -o $(OUTPUT)main-cpp

$(OUTPUT)AudioEngine.o : AudioEngine.cpp headers/*hpp private-headers/*hpp
	$(COMPILER) $(FLAGS) $(INCLUDE) -c AudioEngine.cpp $(LIBS) -o $(OUTPUT)AudioEngine.o

.PHONY: test
test: $(OUTPUT)test.o
	# Must pass:
	$(TIMING) $(OUTPUT)test.o
	# Can't produce memory warnings:
	valgrind --leak-check=full --error-exitcode=2 $(OUTPUT)test.o --ignore-failed

$(OUTPUT)test.o: $(OUTPUT)AudioEngine.o tests/*.hpp tests/*.cpp
	$(COMPILER) $(FLAGS) $(INCLUDE) $(TEST_INCLUDE) $(OUTPUT)AudioEngine.o tests/*.cpp -o $(OUTPUT)test.o $(LIBS)

.PHONY: clean
clean:
	rm $(OUTPUT)test.o $(OUTPUT)AudioEngine.o $(OUTPUT)main-cpp
