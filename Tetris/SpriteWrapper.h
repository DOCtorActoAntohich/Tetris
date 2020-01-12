#pragma once

#include <SFML/Graphics.hpp>

namespace tetris {
	class SpriteWrapper {
	public:
		void loadFromResource(int32_t id);

		sf::Vector2f getPosition();
		void setPosition(sf::Vector2f position);

		void setColor(const sf::Color& color);

		// Sets the part of texture to be drawn.
		void setTextureRect(const sf::IntRect& rectangle);

		// Tells to draw the full texture.
		void resetTextureRect();

		void draw(sf::RenderWindow& renderTarget);
	private:
		sf::Texture texture;
		sf::Sprite  sprite;
	};
}