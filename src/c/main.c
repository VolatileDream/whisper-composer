#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "portaudio.h"

#include "RingBuffer-struct.h"
#include "AudioEngine-struct.h"
#include "Sound.h"


WC_Sound* as(){

	unsigned int size = 256;
	float PI = 4*acos(1.0);

	WC_Sound* sound = malloc( sizeof(WC_Sound) );
	sound->length = size;
	sound->audioData = malloc(sizeof(float) * size);

	for(unsigned int i=0; i < size; i++){
		sound->audioData[i] = sin( i/PI ) * sin(4*i/PI);
	}

	return sound;
}



WC_Sound* getNextSound( WC_AudioEngine* engine, unsigned long timeout ){
	WC_Sound* out = NULL;

	while( timeout > 0 && (out=WC_GetFinishedSound(engine)) == NULL ){
		Pa_Sleep(100);
		timeout -= 100;
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

	free( soundOne->audioData );
	free( soundOne );
	free( soundTwo->audioData );
	free( soundTwo );

	WC_Dispose(engine);
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

	ae();
}