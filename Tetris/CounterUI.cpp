#include "CounterUI.h"

#include <SFML/System.hpp>

#include <string>

#include "Helper.h"

namespace tetris {
	bool CounterUI::areDefaultsSet = false;
	sf::Font		CounterUI::default_font			 = sf::Font();
	size_t			CounterUI::default_characterSize = 0;
	sf::Color		CounterUI::default_color		 = sf::Color::White;

#pragma warning(push)
	// I can do nothing with this warning since it's caused by SFML.
#pragma warning(disable : 26812)
	sf::Text::Style CounterUI::default_textStyle     = sf::Text::Style::Regular;
#pragma warning(pop)


	CounterUI::CounterUI() {
		this->game_levelCounter = 0;
		this->maximalValue = SIZE_MAX;
		this->textWidth = 3;

		this->areOptionsAssigned = false;

		this->applyDefaults();
	}



	size_t CounterUI::getWidth() {
		return this->textWidth;
	}



	void CounterUI::setWidth(size_t width) {
		this->textWidth = width;
		this->applyDefaults();
		this->updateText();
	}



	int32_t CounterUI::getMaximalValue() {
		return this->maximalValue;
	}



	void CounterUI::setMaximalValue(int32_t maxValue) {
		this->maximalValue = maxValue;
		size_t digits = Helper::countDigits(maxValue);
		this->validateCounterValue();
		this->setWidth(digits);
	}



	void CounterUI::increment(int32_t value) {
		this->game_levelCounter += std::abs(value);
		this->validateCounterValue();
		this->updateText();
	}



	void CounterUI::decrement(int32_t value) {
		this->game_levelCounter -= std::abs(value);
		this->validateCounterValue();
		this->updateText();
	}



	int32_t CounterUI::getNumericValue() {
		return this->game_levelCounter;
	}



	void CounterUI::setNumericValue(int32_t value) {
		this->game_levelCounter = value;
		this->validateCounterValue();
		this->updateText();
	}



#pragma warning(push)
	// I can do nothing with this warning since it's caused by SFML.
#pragma warning(disable : 26812)
	void CounterUI::setDefaultDisplayOptions(
			const sf::Font& font, size_t charSize,
			const sf::Color& color, sf::Text::Style style) {
		CounterUI::default_font = font;
		CounterUI::default_characterSize = charSize;
		CounterUI::default_color = color;
		CounterUI::default_textStyle = style;
		CounterUI::areDefaultsSet = true;
	}



	void CounterUI::setDisplayOptions(
		    const sf::Font& font, size_t charSize,
		    const sf::Color& color, sf::Text::Style style) {
		this->text.setFont(font);
		this->text.setCharacterSize(charSize);
		this->text.setFillColor(color);
		this->text.setStyle(style);
		this->areOptionsAssigned = true;
		this->updateText();
	}
#pragma warning(pop)


	sf::Vector2f CounterUI::getPosition() {
		return this->text.getPosition();
	}


	void CounterUI::setPosition(const sf::Vector2f& position) {
		this->text.setPosition(position);
	}


	void CounterUI::setPosition(float x, float y) {
		this->text.setPosition({x, y});
	}



	std::string CounterUI::getString() {
		return this->text.getString();
	}



	void CounterUI::draw(sf::RenderWindow& window) {
		window.draw(this->text);
	}



	void CounterUI::validateCounterValue() {
		if (this->game_levelCounter < 0) {
			this->game_levelCounter = 0;
		}
		else if (this->game_levelCounter > this->maximalValue) {
			this->game_levelCounter = this->maximalValue;
		}
	}



	void CounterUI::updateText() {
		std::string string = std::to_string(this->game_levelCounter);
		if (string.size() < this->textWidth) {
			std::string zeros(this->textWidth - string.size(), '0');
			string = zeros + string;
		}
		else if (string.size() > this->textWidth) {
			string = string.substr(string.size() - this->textWidth, this->textWidth);
		}
		this->text.setString(string);
	}



	void CounterUI::applyDefaults() {
		if (this->areDefaultsSet && !this->areOptionsAssigned) {
			this->setDisplayOptions(
				this->default_font,
				this->default_characterSize,
				this->default_color,
				this->default_textStyle
			);
		}
	}
}
