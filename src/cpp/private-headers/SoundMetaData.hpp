#ifndef _WHISPER_SOUND_META_DATA_H_
#define _WHISPER_SOUND_META_DATA_H_

#include "Sound.hpp"

namespace Whisper {

struct SoundMetaData {
	Sound* sound;
	bool needsNotify;
	unsigned int offset;
};

}

#endif
