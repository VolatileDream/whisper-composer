#ifndef _WHISPER_SOUND_H_
#define _WHISPER_SOUND_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	float* audioData;
	unsigned long length;
} Sound;

#ifdef __cplusplus
}
#endif

#endif