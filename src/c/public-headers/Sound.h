#ifndef _WHISPER_SOUND_H_
#define _WHISPER_SOUND_H_

struct WC_Sound {
	float* audioData;
	unsigned int length;
};

typedef struct WC_Sound WC_Sound;

#endif