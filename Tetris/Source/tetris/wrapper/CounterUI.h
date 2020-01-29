#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


namespace tetris::wrapper {
	class CounterUI {
	public:
		static const size_t MAX_WIDTH = 10;

		CounterUI();

#pragma region Data Members Access

		int32_t getMaximalValue();
		void setMaximalValue(int32_t maxValue);

		// Increases the value of counter.
		// It will be adjusted to be between zero and maximum value.
		void increment(int32_t value = 1);

		// Decreases the value of counter.
		// It will be adjusted to be between zero and maximum value.
		void decrement(int32_t value = 1);

		// Returns a value of counter.
		int32_t getNumericValue();

		// Sets special value of counter.
		// It will be adjusted to be between zero and maximum value.
		void setNumericValue(int32_t value);

		// Returns text string to be drawn.
		std::string getString();

#pragma /* Data Members Access */ endregion


#pragma region Text and Screen

		// Returns text width in symbols.
		size_t getWidth();

		// Set the text width in symbols.
		// Cannot be more than MAX_WIDTH.
		void setWidth(size_t width);

		// Sets DEFAULT options for dislaying text.
		static void setDefaultDisplayOptions(
			const sf::Font& font,
			size_t charSize,
			const sf::Color& color,
			sf::Text::Style style
		);

		// Sets options for dislaying text.
		void setDisplayOptions(
			const sf::Font& font,
			size_t charSize,
			const sf::Color& color,
			sf::Text::Style style = sf::Text::Style::Regular
		);

		// Applies default display parameters to the text.
		// If they are not set manually, it has no effect.
		void applyDefaultDisplayOptions();

		
		sf::Vector2f getPosition();
		void setPosition(const sf::Vector2f& position);
		void setPosition(float x, float y);
		void drawOn(sf::RenderWindow& window);

#pragma /* Text and Screen */ endregion

	private:
		int32_t counter;
		int32_t maximalValue;

		size_t textWidth;
		sf::Text text;
		bool areDisplayOptionsAssigned;

		// Makes counter value valid (if it is not).
		void validateCounterValue();

		// Updates the text. Should be called once counter is updated.
		// Resizes text string to make it have appropriate width.
		void updateText();


		static bool areDefaultsSet;
		static sf::Font		   default_font;
		static size_t		   default_characterSize;
		static sf::Color	   default_color;
		static sf::Text::Style default_textStyle;
	};
}