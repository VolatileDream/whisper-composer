#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "portaudio.h"

#include "RingBuffer-struct.h"
#include "AudioEngine-struct.h"
#include "Sound.h"


WC_Sound* as(){

	const unsigned int size = 44100;
	const float PI = 3.14159265358979323846;
	printf("pi: %f\n", PI);

	WC_Sound* sound = malloc( sizeof(WC_Sound) );
	sound->length = size;
	sound->audioData = malloc(size * sizeof(float));

	for(unsigned int i=0; i < size; i++){
		float pos =  i* 1.0/400.0 * 2*PI;
		sound->audioData[i] = sin(pos);
	}

	return sound;
}



WC_Sound* getNextSound( WC_AudioEngine* engine, unsigned long timeout ){
	WC_Sound* out = NULL;

	while( timeout > 0 && (out=WC_GetFinishedSound(engine)) == NULL ){
		Pa_Sleep(10);
		timeout -= 10;
	}

	return out;
}

void ae(){

	WC_Sound* soundOne = as();
	WC_Sound* soundTwo = as();

	WC_AudioEngine* engine = malloc( sizeof(WC_AudioEngine) );
	WC_Init(engine, NULL);

	WC_AddSound( engine, soundOne, true );
	WC_AddSound( engine, soundTwo, false );

	unsigned long timeout = 1000;
	WC_Sound* sound = getNextSound(engine, timeout);

	if( sound == soundTwo ){
		printf("Error, managed to retrieve SoundTwo\n");
	}

	sound = getNextSound(engine, timeout);

	if( sound == soundTwo ){
		printf("Error, managed to retrieve SoundTwo\n");
	}

	// tell the engine to stop BEFORE we free the sounds
	WC_Dispose(engine);

	free( soundOne->audioData );
	free( soundOne );
	free( soundTwo->audioData );
	free( soundTwo );
}

void ae2(){

	WC_Sound* soundOne = as();

	WC_AudioEngine* engine = WC_Allocate();
	WC_Init(engine, NULL);

	const unsigned int stepSize = 10;
	unsigned long timeout = 5000;
	unsigned int playCount = 0;

	WC_Sound* tmp = NULL;

	WC_AddSound( engine, soundOne, true );
	
	while( playCount < 20 && timeout > 0 ){

		while( timeout > 0 && (tmp=WC_GetFinishedSound(engine)) == NULL ){
			Pa_Sleep(stepSize);
			timeout -= stepSize;
		}

		WC_AddSound(engine, soundOne, true);
		playCount++;
	}

	printf("Waiting...\n");
	Pa_Sleep(1000);

	WC_Dispose(engine);
	
	free( soundOne->audioData );
	free( soundOne );
}

void crb(){
	unsigned int size = 16;
	
	WC_RingBuffer* crb = malloc(sizeof(WC_RingBuffer));
	WC_RB_Create(crb, sizeof(int), size);

	int index = 0, tmp = 0;
	while( WC_RB_Add(crb, &index) ){
		index ++;
	}

	printf("Max writes from empty for size %d was: %d\n", size, index);

	index = 0;
	while( WC_RB_Remove(crb, &tmp) ){
		index++;
	}

	printf("Max reads after full for size %d was: %d\n", size, index);
	WC_RB_Dispose(crb);
	free(crb);
}


void crb2(){
	unsigned int size = 16;
	
	WC_RingBuffer* crb = malloc(sizeof(WC_RingBuffer));
	WC_RB_Create(crb, sizeof(WC_Sound), size);

	int index = 0;
	WC_Sound sound;
	sound.audioData = (float*)index;
	sound.length = index;
	
	while( WC_RB_Add(crb, &sound) ){
		printf("Write: %d %d\n", (int)sound.audioData, sound.length);
		index ++;
		sound.audioData = (float*)index;
		sound.length = index;
	}

	printf("Max writes from empty for size %d was: %d\n", size, index);

	index = 0;
	while(WC_RB_Remove(crb, &sound)){
		index++;
		printf("Read: %d %d\n", (int)sound.audioData, sound.length);
	}

	printf("Max reads after full for size %d was: %d\n", size, index);
	WC_RB_Dispose(crb);
	free(crb);
}

int main(){
	
	crb();
	crb2();

	//ae();
	ae2();
}