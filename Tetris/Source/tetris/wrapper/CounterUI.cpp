#include "tetris/wrapper/CounterUI.h"

#include <SFML/System.hpp>

#include <string>

#include "tetris/helper/Helper.h"

using namespace tetris::wrapper;


#pragma region Static Members Initialization

bool CounterUI::areDefaultsSet = false;

sf::Font		CounterUI::default_font			 = sf::Font();
uint32_t			CounterUI::default_characterSize = 24;
sf::Color		CounterUI::default_color		 = sf::Color::White;
sf::Text::Style CounterUI::default_textStyle     = sf::Text::Style::Regular;


#pragma /* Static Members Initialization */ endregion


CounterUI::CounterUI() {
	this->counter = 0;
	this->maximalValue = INT32_MAX;
	this->textWidth = 3;

	this->areDisplayOptionsAssigned = false;

	this->applyDefaultDisplayOptions();
}


#pragma region Data Members Access


int32_t CounterUI::getMaximalValue() {
	return this->maximalValue;
}

void CounterUI::setMaximalValue(int32_t maxValue) {
	this->maximalValue = maxValue;
	this->validateCounterValue();

	uint32_t digits = helper::countDigits(maxValue);
	this->setWidth(digits); // Text is updated here.
}



void CounterUI::increment(int32_t value) {
	this->counter += std::abs(value);
	this->validateCounterValue();
	this->updateText();
}

void CounterUI::decrement(int32_t value) {
	this->counter -= std::abs(value);
	this->validateCounterValue();
	this->updateText();
}



int32_t CounterUI::getNumericValue() {
	return this->counter;
}

void CounterUI::setNumericValue(int32_t value) {
	this->counter = value;
	this->validateCounterValue();
	this->updateText();
}



std::string CounterUI::getString() {
	return this->text.getString();
}


#pragma /* Data Members Access */ endregion


#pragma region Text and Screen


uint32_t CounterUI::getWidth() {
	return this->textWidth;
}

void CounterUI::setWidth(uint32_t width) {
	if (width > this->MAX_WIDTH) {
		width = this->MAX_WIDTH;
	}
	this->textWidth = width;
	this->updateText();
}



void CounterUI::setDefaultDisplayOptions(const sf::Font& font,
										 uint32_t charSize,
										 const sf::Color& color,
										 sf::Text::Style style) {
	CounterUI::default_font = font;
	CounterUI::default_characterSize = charSize;
	CounterUI::default_color = color;
	CounterUI::default_textStyle = style;
	CounterUI::areDefaultsSet = true;
}

void CounterUI::setDisplayOptions(const sf::Font& font,
								  uint32_t charSize,
								  const sf::Color& color,
								  sf::Text::Style style) {
	this->text.setFont(font);
	this->text.setCharacterSize(charSize);
	this->text.setFillColor(color);
	this->text.setStyle(style);
	this->areDisplayOptionsAssigned = true;
}



void CounterUI::applyDefaultDisplayOptions() {
	if (this->areDefaultsSet && !this->areDisplayOptionsAssigned) {
		this->setDisplayOptions(
			this->default_font,
			this->default_characterSize,
			this->default_color,
			this->default_textStyle
		);
	}
}



sf::Vector2f CounterUI::getPosition() {
	return this->text.getPosition();
}

void CounterUI::setPosition(const sf::Vector2f& position) {
	this->text.setPosition(position);
}

void CounterUI::setPosition(float x, float y) {
	this->text.setPosition({ x, y });
}



void CounterUI::drawOn(sf::RenderWindow& window) {
	window.draw(this->text);
}


#pragma /* Text and Screen */ endregion



void CounterUI::validateCounterValue() {
	if (this->counter < 0) {
		this->counter = 0;
	}
	else if (this->counter > this->maximalValue) {
		this->counter = this->maximalValue;
	}
}



void CounterUI::updateText() {
	std::string string = std::to_string(this->counter);
	if (string.size() < this->textWidth) {
		std::string zeros(this->textWidth - string.size(), '0');
		string = zeros + string;
	}
	else if (string.size() > this->textWidth) {
		string = string.substr(string.size() - this->textWidth, this->textWidth);
	}
	this->text.setString(string);
}
