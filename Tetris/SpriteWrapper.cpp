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



void SpriteWrapper::setColor(const sf::Color& color) {
	this->sprite.setColor(color);
}



void SpriteWrapper::setTextureRect(const sf::IntRect& rectangle) {
	this->sprite.setTextureRect(rectangle);
}



void SpriteWrapper::resetTextureRect() {
	auto textureSize = this->texture.getSize();
	sf::IntRect rectangle(0, 0, textureSize.x, textureSize.y);
	this->sprite.setTextureRect(rectangle);
}



void SpriteWrapper::draw(sf::RenderWindow& window) {
	window.draw(this->sprite);
}
