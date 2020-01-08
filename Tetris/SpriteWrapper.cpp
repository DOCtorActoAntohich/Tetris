#include "SpriteWrapper.h"

#include "Helper.h"

using namespace tetris;



void SpriteWrapper::loadFromResource(int32_t id) {
	std::vector<byte> bytes = Helper::loadEmbeddedResource(id);
	this->texture.loadFromMemory(&bytes[0], bytes.size());
	this->sprite.setTexture(this->texture);
}



sf::Vector2f SpriteWrapper::getPosition() {
	return this->sprite.getPosition();
}



void SpriteWrapper::setPosition(sf::Vector2f position) {
	this->sprite.setPosition(position);
}



void SpriteWrapper::draw(sf::RenderWindow& window) {
	window.draw(this->sprite);
}
