#ifndef _SOUND_HPP_
#define _SOUND_HPP_

class Sound {
public:

	virtual dyn_array<float>* getData() {
		return getData(0, length());
	};

	virtual dyn_array<float>* getData( size_t offset, size_t length ) = 0;

	virtual size_t length() = 0;
};

#endif
