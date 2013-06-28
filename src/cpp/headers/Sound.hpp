#ifndef _SOUND_HPP_
#define _SOUND_HPP_

#include <memory>

#include "dyn_array.hpp"

namespace Whisper {

class Sound {
public:

	virtual const dyn_array<float>* getData() = 0;

	virtual std::unique_ptr<dyn_array<float>> getData( size_t offset, size_t length ) = 0;

	virtual size_t length() = 0;
};

}

#endif
