#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "tetris/wrapper/FrameTimer.h"

namespace tetris::wrapper {
	// Describes a drawable object that blinks on a screen.
	// Should be updates once per frame.
	template<class Drawable>
	class BlinkingObject {
	public:
		BlinkingObject() {
			this->isVisible = true;
		}

		BlinkingObject(const Drawable& drawable, int32_t frames) {
			this->isVisible = true;
			this->setDrawable(drawable);
			this->setBlinkingTiming(frames);
		}



		// Updates the built-in timer.
		void update() {
			this->timer.update();
			if (this->timer.isTriggered()) {
				this->isVisible = !this->isVisible;
			}
		}

		// Draws the object on a window.
		// If it should not be visible, there will be no effects.
		void drawOn(sf::RenderWindow& window) {
			if (this->isVisible) {
				window.draw(this->drawable);
			}
		}



		// Sets position of an object on a screen.
		void setPosition(const sf::Vector2f& position) {
			this->drawable.setPosition(position);
		}

		// Returns the position of an object on a screen.
		const sf::Vector2f& returnPosition() const {
			return this->drawable.getPosition();
		}



		// Sets a specific drawable.
		// Screen position also replaces with one the given drawable has.
		void setDrawable(const Drawable& drawable) {
			this->drawable = drawable;
		}

		// Returns the current drawable.
		const Drawable& getDrawable() const {
			return this->drawable;
		}



		// Sets how often (in frames) current object will appear and disappear.
		void setBlinkingTiming(int32_t frames) {
			this->timer.setTimingFrames(frames);
		}

		// Returns how often (in frames) current object will appear and disappear.
		int32_t getBlinkingTiming() {
			return this->timer.getTimingFrames();
		}



		// Resets the timer and visibility.
		void reset() {
			this->isVisible = true;
			this->timer.reset();
		}
	private:
		Drawable drawable;
		FrameTimer timer;
		bool isVisible;
	};



	template class BlinkingObject<sf::Sprite>;
	template class BlinkingObject<sf::Text>;
	template class BlinkingObject<sf::RectangleShape>;
}