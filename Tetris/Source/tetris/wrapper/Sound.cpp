#include "tetris/wrapper/Sound.h"

#include "tetris/helper/Helper.h"

using namespace tetris::wrapper;



void Sound::loadFromResource(int32_t id) {
	std::vector<byte> bytes = helper::loadEmbeddedResource(id);
	this->soundBuffer.loadFromMemory(&bytes[0], bytes.size());
	this->sound.setBuffer(this->soundBuffer);
}



void Sound::play() {
	this->sound.play();
}
