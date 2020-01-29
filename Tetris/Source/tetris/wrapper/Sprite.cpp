#include "tetris/wrapper/Sprite.h"

#include "tetris/helper/Helper.h"

using namespace tetris::wrapper;



void Sprite::loadFromResource(int32_t id) {
	std::vector<byte> bytes = helper::loadEmbeddedResource(id);
	this->texture.loadFromMemory(&bytes[0], bytes.size());
	this->sprite.setTexture(this->texture);
}



const sf::Vector2f& Sprite::getPosition() const {
	return this->sprite.getPosition();
}



void Sprite::setPosition(sf::Vector2f position) {
	this->sprite.setPosition(position);
}



void Sprite::setColor(const sf::Color& color) {
	this->sprite.setColor(color);
}



const sf::Vector2f& Sprite::getScale() const {
	return this->sprite.getScale();
}



void Sprite::setScale(const sf::Vector2f& scale) {
	this->sprite.setScale(scale);
}



void Sprite::setScale(float x, float y) {
	this->sprite.setScale(x, y);
}



void Sprite::setTextureRect(const sf::IntRect& rectangle) {
	this->sprite.setTextureRect(rectangle);
}



void Sprite::resetTextureRect() {
	auto textureSize = this->texture.getSize();
	sf::IntRect rectangle(0, 0, textureSize.x, textureSize.y);
	this->sprite.setTextureRect(rectangle);
}



void Sprite::drawOn(sf::RenderWindow& window) const {
	window.draw(this->sprite);
}
