
#include <memory>

#include "Sound.hpp"
#include "dyn_array.hpp"
#include "ArraySound.hpp"

namespace Whisper {

const dyn_array<float>* ArraySound::getData() {
	return data;
}

std::unique_ptr<dyn_array<float>> ArraySound::getData( size_t offset, size_t length ) {
	return this->data->subarray( offset, length );
}

size_t ArraySound::length() {
	return this->data->size;
}

}