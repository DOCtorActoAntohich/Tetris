#pragma once

#include <SFML/Graphics.hpp>

namespace tetris {
	class SpriteWrapper {
	public:
		void loadFromResource(int32_t id);

		const sf::Vector2f& getPosition() const;
		void setPosition(sf::Vector2f position);

		void setColor(const sf::Color& color);

		const sf::Vector2f& getScale() const;
		void setScale(const sf::Vector2f& scale);
		void setScale(float x, float y);

		// Sets the part of texture to be drawn.
		void setTextureRect(const sf::IntRect& rectangle);

		// Tells to draw the full texture.
		void resetTextureRect();

		void drawOn(sf::RenderWindow& renderTarget) const;
	private:
		sf::Texture texture;
		sf::Sprite  sprite;
	};
}