#include <iostream>
#include <cmath>

#include "portaudio.h"

#include "RingBuffer.hpp"
#include "Sound.hpp"

#include "AudioEngine.hpp"
#include "AudioEngineBuilder.hpp"

using namespace Whisper;

Sound* as(unsigned int length){

  const unsigned int size = 44100 * length;
  const double PI = 3.14159265358979323846;
  std::cout << "pi: " << PI << " size: " << size << std::endl;

  Sound* sound = new Sound();
  sound->length = size;
  sound->audioData = new float[size];

  for(unsigned int i=0; i < size; i++){
    double pos =  i* 1.0/75.0 * 2.0*PI;
    sound->audioData[i] = (float) sin(pos);
  }

  return sound;
}



Sound* getNextSound( AudioEngine* engine, unsigned long timeout ){
  Sound* out = NULL;

  while( timeout > 0 && (out=engine->getFinishedSound()) == NULL ){
    Pa_Sleep(10);
    timeout -= 10;
  }

  return out;
}

void ae(){

  unsigned int length = 4;

  Sound* soundOne = as(length);

  AudioEngine* engine = AudioEngineBuilder().build();
  
  engine->addSound(soundOne, true );
  
  Pa_Sleep(length * 1000);

  delete engine;
  
  delete [] soundOne->audioData ;
  delete soundOne ;
}

int main(){
  
  std::cout << "sizeof(float): " << sizeof(float) << std::endl;

  ae();
}