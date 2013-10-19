COMPILER= g++
FLAGS= -std=c++11 -g -Wall -Wsign-compare -Wreturn-type -pedantic
LIBS= -lm -lportaudio -lstdc++
INCLUDE= -I ../cpp/headers/ -I ../cpp/private-headers/ -I headers/

TEST_INCLUDE= -I tests
TIMING= time

OUTPUT= ../../lib/

C_AUDIO_ENGINE=$(OUTPUT)WC_AudioEngine.o

.PHONY: all
all : $(C_AUDIO_ENGINE) lib main

$(OUTPUT)main-c: $(OUTPUT)libAudioEngine.a main.c
	$(COMPILER) -g -std=c99 -pedantic -Wall $(INCLUDE) main.c -L . -lAudioEngine \
	$(LIBS) -o $(OUTPUT)main-c


$(OUTPUT)libAudioEngine.a: $(C_AUDIO_ENGINE) $(OUTPUT)AudioEngine.o
	$(COMPILER) -g -std=c99 -pedantic -Wall $(C_AUDIO_ENGINE) $(OUTPUT)AudioEngine.o \
	$(LIBS) -shared -o $(OUTPUT)libAudioEngine.a
	ln -s $(OUTPUT)libAudioEngine.a libAudioEngine.a

$(C_AUDIO_ENGINE): $(OUTPUT)AudioEngine.o WC_AudioEngine.c
	$(COMPILER) $(FLAGS) $(INCLUDE) \
	 -c WC_AudioEngine.c \
	 -o $(C_AUDIO_ENGINE)

$(OUTPUT)AudioEngine.o: 
	cd ../cpp ; make AudioEngine.o

.PHONY: clean
clean:
	rm $(OUTPUT)libAudioEngine.a $(C_AUDIO_ENGINE) $(OUTPUT)main-c libAudioEngine.a

# Proxy Rules

.PHONY: main
main: $(OUTPUT)main-c

.PHONY: lib
lib: $(OUTPUT)libAudioEngine.a
