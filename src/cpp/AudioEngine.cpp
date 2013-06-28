#include <algorithm>

#include "AudioEngine.hpp"

// some small helper functions
template<typename T>
static T min( T a, T b ){
	if( a < b ){
		return a;
	}
	return b;
}


// Class stuff

AudioEngine::AudioEngine( void* audioSettings ){
	// uhhh...TODO, dunno how to handle audio settings yet.
}

bool AudioEngine::addSound( Sound* sound ){

	if( sound == NULL || sound->length() == 0 ){
		// we're too nice to say anything, we pretend it worked.
		return true;
	}
	
	return newSoundBuffer.add( sound );
}

AudioEngine::~AudioEngine(){
	// TODO clean up port audio stuff
}

// Private Stuff beyond here.
// Warning: Here be dragons.

int AudioEngine::givePortAudioData(
		void *outputBuffer
        ,unsigned long framesPerBuffer
//        ,const PaStreamCallbackTimeInfo* timeInfo
//        ,PaStreamCallbackFlags statusFlags
    ){

	float* out = (float*)outputBuffer;

	// do all the exsting sounds first, then new sounds.

	writeOutPreExisting(out, framesPerBuffer);

	// new sounds

	size_t oldSoundLength = playingSoundsLength;

	copyNewSoundsToExisting();

	if( oldSoundLength == playingSoundsLength ){
		// no new sounds
		return 0;
	}

	// adjust the stream amplitude because we've added new sounds

	float amplitudeModifier = (playingSoundsLength - oldSoundLength)*1.0/playingSoundsLength;
	for(unsigned long i=0; i < framesPerBuffer; i++){
		out[i] = out[i] * amplitudeModifier;
	}

	writeOutPreExisting(out, framesPerBuffer, oldSoundLength);

	return 0;
}

void AudioEngine::writeOutPreExisting(float* out, unsigned long framesPerBuffer, size_t startIndex){
	
	float soundAmplitude = (playingSoundsLength - startIndex)*1.0/playingSoundsLength;

	Sound* currentSound;
	unsigned long soundOffset, soundRemaining;

	for(size_t i= startIndex; i < playingSoundsLength; i++){

		std::tie( currentSound, soundOffset ) = playingSounds.at(i);

		soundRemaining = currentSound->length() - soundOffset;

		unsigned long outputCount = min( framesPerBuffer, soundRemaining );

		// TODO Write the song out
		dyn_array<float> soundData = *( currentSound->getData(soundOffset, soundRemaining) );

		for( unsigned long index = 0; index < soundRemaining; index++){
			out[index] = out[index] + soundAmplitude * soundData.data[index];
		}

		// song is done playing
		if( outputCount == soundRemaining ){
			playingSoundsLength--;
			playingSounds.at(i) = playingSounds.at(playingSoundsLength);
			i--;

			if( !playedSoundBuffer.add(currentSound) ){
				// TODO what do we do?
			}
		}

	}
}

void AudioEngine::copyNewSoundsToExisting(){
	Sound* sound = NULL;
	while(
		newSoundBuffer.remove(sound)
		&& playingSoundsLength < playingSounds.max_size()
	){
		playingSounds.at( playingSoundsLength ) = std::tuple<Sound*,unsigned long>(sound, 0);
		playingSoundsLength++;
	}

	if( sound != NULL && playingSoundsLength == playingSounds.max_size() ){
		//TODO Adding too many sounds
	}

}
