#include <iostream>
#include <cmath>

#include "portaudio.h"

#include "RingBuffer.hpp"
#include "Sound.hpp"

#include "AudioEngine.hpp"
#include "AudioEngineBuilder.hpp"

using namespace Whisper;

Sound* as(){

  const unsigned int size = 44100;
  const float PI = 3.14159265358979323846;
  std::cout << "pi: " << PI << " size: " << size << std::endl;

  Sound* sound = new Sound();
  sound->length = size;
  sound->audioData = new float[size];

  for(unsigned int i=0; i < size; i++){
    float pos =  i* 1.0/400.0 * 2.0*PI;
    std::cout << i << " " << pos << " ";
    pos = sin(pos);
    std::cout << pos << std::endl;
    sound->audioData[i] = pos;
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

  Sound* soundOne = as();
  Sound* soundTwo = as();

  AudioEngine* engine = AudioEngineBuilder().build();

  engine->addSound( soundOne, true );
  engine->addSound( soundTwo, false );

  unsigned long timeout = 1000;
  Sound* sound = getNextSound(engine, timeout);

  if( sound == soundTwo ){
    std::cout << "Error, managed to retrieve SoundTwo\n";
  }

  sound = getNextSound(engine, timeout);

  if( sound == soundTwo ){
    std::cout << "Error, managed to retrieve SoundTwo\n";
  }

  // tell the engine to stop BEFORE we free the sounds
  delete engine;

  delete [] soundOne->audioData;
  delete soundOne;
  delete [] soundTwo->audioData;
  delete soundTwo;
}

void ae2(){

  Sound* soundOne = as();

  AudioEngine* engine = AudioEngineBuilder().build();
  

  const unsigned int stepSize = 1;
  unsigned long timeout = 5000;
  unsigned int playCount = 0;

  Sound* tmp = NULL;

  engine->addSound(soundOne, true );
  
  while( playCount < 20 && timeout > 0 ){

    while( timeout > 0 && (tmp=engine->getFinishedSound()) == NULL ){
      Pa_Sleep(stepSize);
      timeout -= stepSize;
    }
    std::cout << "done: " << playCount << "\n";

    engine->addSound(soundOne, true);
    playCount++;
  }

  std::cout << "Waiting...\n";
  Pa_Sleep(1000);

  delete engine;
  
  delete [] soundOne->audioData ;
  delete soundOne ;
}

int main(){
  
  //ae();
  ae2();
}