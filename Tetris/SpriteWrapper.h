#pragma once

#include <SFML/Graphics.hpp>

namespace tetris {
	class SpriteWrapper {
	public:
		void loadFromResource(int32_t id);

		sf::Vector2f getPosition();
		void setPosition(sf::Vector2f position);

		void draw(sf::RenderWindow& renderTarget);
	private:
		sf::Texture texture;
		sf::Sprite  sprite;
	};
}