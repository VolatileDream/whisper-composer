#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// for sleep 
#include <portaudio.h>

#include "Sound.hpp"

#include "WC_AudioEngine.h"


Sound* as(){

	const unsigned int size = 44100;
	const double PI = 3.14159265358979323846;
	printf("pi: %f\n", PI);

	Sound* sound = (Sound*) malloc( sizeof(Sound) );
	sound->length = size;
	sound->audioData = (float*) malloc(size * sizeof(float));

	for(unsigned int i=0; i < size; i++){
		double pos =  i* 1.0/75.0 * 2.0*PI;
	    sound->audioData[i] = (float) sin(pos);
	}

	return sound;
}

void ae(){

	Sound* soundOne = as();

	void* engine = WC_Create(32,32,32);

	WC_AddSound( engine, soundOne, true );
	
	Pa_Sleep(1000);

	WC_Dispose(engine);
	
	free( soundOne->audioData );
	free( soundOne );
}

int main(){
	
	ae();
}