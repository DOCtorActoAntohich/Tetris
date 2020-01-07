#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


namespace tetris {
	class CounterUI {
	public:
		CounterUI();
		

		size_t getWidth();
		void setWidth(size_t width);

		int32_t getMaximalValue();
		void setMaximalValue(int32_t maxValue);


		void increment(int32_t value = 1);
		void decrement(int32_t value = 1);

		int32_t getNumericValue();
		void setNumericValue(int32_t value);

		// Sets DEFAULT options for dislaying text.
		static void setDefaultDisplayOptions(
			const sf::Font& font, size_t charSize, const sf::Color& color,
			sf::Text::Style style = sf::Text::Style::Regular
		);

		// Sets options for dislaying text.
		void setDisplayOptions(
			const sf::Font& font, size_t charSize, const sf::Color& color,
			sf::Text::Style style = sf::Text::Style::Regular
		);

		sf::Vector2f getPosition();
		void setPosition(const sf::Vector2f& position);
		void setPosition(float x, float y);

		std::string getString();

		void draw(sf::RenderWindow& window);
	private:
		int32_t game_levelCounter;
		int32_t maximalValue;

		static const size_t MAX_WIDTH = 10;
		size_t textWidth;
		sf::Text text;
		bool areOptionsAssigned;

		// Makes counter value valid (if it is not).
		void validateCounterValue();

		// Updates the text. Should be called once counter is updated.
		void updateText();


		static bool areDefaultsSet;
		static sf::Font		   default_font;
		static size_t		   default_characterSize;
		static sf::Color	   default_color;
		static sf::Text::Style default_textStyle;
		void applyDefaults();
	};
}