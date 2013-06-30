#include <iostream>
#include "portaudio.h"
#include <cmath>

#include "ConcurrentRingBuffer.hpp"
#include "AudioEngine.hpp"
#include "Sound.hpp"
#include "ArraySound.hpp"

using namespace Whisper;

Sound* as(){

	float PI = 4*std::acos(1.0);

	dyn_array<float>* contents = dyn_array<float>::create( 256 );

	// do not play with this pointer...
	float* data = contents->data;

	for(size_t i=0; i < contents->size; i++){
		data[i] = std::sin( i/PI ) * std::sin(4*i/PI);
	}
	data = NULL;


	ArraySound* sound = new ArraySound( contents );	
	return sound;
}

Sound* getNextSound( AudioEngine* engine, unsigned long timeout ){
	Sound* out = NULL;

	while( timeout > 0 && (out=engine->getFinishedSound()) == NULL ){
		Pa_Sleep(100);
		timeout -= 100;
	}

	return out;
}

void ae(){

	Sound* soundOne = as();
	Sound* soundTwo = as();

	AudioEngine* engine = new AudioEngine( (void*) NULL );

	engine->addSound( soundOne, true );
	engine->addSound( soundTwo, false );

	unsigned long timeout = 1000;
	Sound* sound = getNextSound(engine, timeout);

	if( sound == soundTwo ){
		std::cout << "Error, managed to retrieve SoundTwo" << std::endl;
	}

	sound = getNextSound(engine, timeout);

	if( sound == soundTwo ){
		std::cout << "Error, managed to retrieve SoundTwo" << std::endl;
	}

	delete soundOne;
	delete soundTwo;
	delete engine;
}

void crb(){
	const size_t size = 16;
	
	ConcurrentRingBuffer<unsigned long,size>* crb = new ConcurrentRingBuffer<unsigned long,size>();

	unsigned long index = 0, tmp = 0;
	while( crb->add(index) ){
		index ++;
	}

	std::cout << "Max writes for size " << size << " was: " << index << std::endl;

	index = 0;
	while( crb->remove(tmp) ){
		index++;
	}

	std::cout << "Max reads after full for size " << size << " was: " << index << std::endl;
	delete crb;
}

int main(){
	
	crb();

	ae();
}