#include "SoundWrapper.h"

#include "Helper.h"

using namespace tetris;



void SoundWrapper::loadFromResource(int32_t id) {
	std::vector<byte> bytes = Helper::loadEmbeddedResource(id);
	this->soundBuffer.loadFromMemory(&bytes[0], bytes.size());
	this->sound.setBuffer(this->soundBuffer);
}



void SoundWrapper::play() {
	this->sound.play();
}
