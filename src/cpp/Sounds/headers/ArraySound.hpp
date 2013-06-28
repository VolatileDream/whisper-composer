#ifndef _ARRAY_SOUND_HPP_
#define _ARRAY_SOUND_HPP_

#include <memory>

#include "dyn_array.hpp"
#include "Sound.hpp"

namespace Whisper {

class ArraySound : public Sound {
	public:
		ArraySound( dyn_array<float>* data ) : data(data) {}
		~ArraySound(){
			delete data;
		}
		const dyn_array<float>* getData();
		std::unique_ptr<dyn_array<float>> getData( size_t offset, size_t length );
		size_t length();

	private:
		const dyn_array<float>* data;
};

}

#endif
