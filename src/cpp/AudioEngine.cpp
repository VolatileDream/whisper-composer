#include "Sound.hpp"
#include "AudioEngine.hpp"
#include "AudioEngineBuilder.hpp"

#include "RingBuffer.hpp"
#include "SoundMetaData.hpp"

namespace Whisper {

void print_pa_error(PaError err, const char* message ){
	if( err != paNoError ){
		printf("%s%s\n", message, Pa_GetErrorText(err) );
	}
}

/// Tries to add a sound to the engines play queue.
///
/// @param flagFinish: if false, the sound will not show up
///		in calls to getFinishedSound()
/// @return true on success
bool AudioEngine::addSound( Sound* sound, bool flagFinish ){

	if( sound == nullptr || sound->length == 0 ) return false;

	SoundMetaData data;

	data.sound = sound;
	data.needsNotify = flagFinish;
	data.offset = 0;

	return this->newSoundBuffer->add(data);
}

/// Tries to retrieve a pointer to the next sound that's done playing.
///
/// @return nullptr if there is no such sound
Sound* AudioEngine::getFinishedSound(){
	Sound* data;
	if( this->playedSoundBuffer->remove(data) ){
		return data;
	}
	return nullptr;
}

/// Shutsdown and disposes of the AudioEngine
AudioEngine::~AudioEngine(){
	
	if( this->stream != nullptr ){

		// we opened a stream, close it.
		PaError err = Pa_StopStream(this->stream);

		print_pa_error(err, "Error stopping stream: ");

		// we started port audio, stop it.
		// note that if we don't we could
		// cause all sorts of errors with
		// the audio 
		err = Pa_Terminate();

		print_pa_error(err, "Error terminating port audio: ");

	}

	delete [] this->playingSounds;

	delete this->newSoundBuffer;
	delete this->playedSoundBuffer;
}

// ==== Protected / Private Functions ====

void AudioEngine::copyNewSoundsToExisting(){
	SoundMetaData sound;
	while(
		this->newSoundBuffer->hasMore()
		&& this->playingSoundsLength < this->playingSoundsSize
	){
		// we can safely ignore the return value, we checked hasMore/1
		this->newSoundBuffer->remove(sound);

		this->playingSounds[ this->playingSoundsLength ] = sound;
		this->playingSoundsLength++;
	}

	// hasSound = true; iff we couldn't add all the sounds
	if( this->newSoundBuffer->hasMore() ){
		//TODO Adding too many sounds
	}
}

void AudioEngine::writeOutPreExisting(float* out, unsigned int framesPerBuffer){
	
	if( this->playingSoundsLength == 0 ) return;

	// adjust the amplitude
	float soundAmplitude = 1.0 / this->playingSoundsLength;

	for(unsigned int i= 0; i < this->playingSoundsLength; i++){

		SoundMetaData data = this->playingSounds[i];

		unsigned long soundRemaining = data.sound->length - data.offset;

		unsigned long outputCount =
			framesPerBuffer < soundRemaining ?
				framesPerBuffer : soundRemaining;

		// Write the song out
		float* rawSound = data.sound->audioData + data.offset;

		data.offset = data.offset + outputCount;

		for( unsigned long index = 0; index < outputCount; index++){
			out[index] = out[index] + soundAmplitude * rawSound[index];
		}

		// song is done playing
		if( outputCount == soundRemaining ){
			this->playingSoundsLength--;
			this->playingSounds[i] = this->playingSounds[this->playingSoundsLength];
			i--;

			if( data.needsNotify && ! this->playedSoundBuffer->add(data.sound) ){
				// TODO what do we do?
			}
		}else{
			this->playingSounds[i] = data;
		}

	}// end engine update sound
}


int AudioEngine::fetchSoundData(
 	void *outputBuffer
    , unsigned long framesPerBuffer
){
	// TODO we currently only support one format
	float* out = (float*)outputBuffer;

	// clear the buffer, because there could be junk in it.
	// and because writeOutPreExisting/2 doesn't overwrite
	// the buffer, but adds to it's contents
	for(unsigned long i = 0; i < framesPerBuffer; i++){
			out[i] = 0.0;
	}

	// do all the exsting sounds first, then deal with new sounds
	this->writeOutPreExisting(out, framesPerBuffer);

	// the new sounds we pick up will get played next time
	this->copyNewSoundsToExisting();
	
	return 0;
}

}
