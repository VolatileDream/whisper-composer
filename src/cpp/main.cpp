#include <iostream>
#include "portaudio.h"
#include <cmath>

#include "ConcurrentRingBuffer.hpp"
#include "AudioEngine.hpp"
#include "Sound.hpp"
#include "ArraySound.hpp"

using namespace Whisper;

Sound* as();
void crb();
void ae();

int main(){
	
	crb();

	ae();
}

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

void ae(){

	AudioEngine* engine = new AudioEngine( (void*) NULL );

	engine->addSound( as() );

	Sound* doneSound = NULL;
	while( doneSound == NULL ){
		Pa_Sleep(500);
		doneSound = engine->getFinishedSound();
	}
	delete doneSound;

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