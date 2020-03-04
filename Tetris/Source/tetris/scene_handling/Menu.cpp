#include "tetris/scene_handling/Menu.h"

#include <string>

#include "tetris/scene_handling/ResourceManager.h"


using namespace tetris::scene_handling;


#pragma region Initialization

Menu::Menu() {
	this->initializeResources();
	this->initializeSelectors();
	this->reset();
}



void Menu::initializeResources() {
	this->background.setTexture(
		ResourceManager::getTexture(MENU_SCREEN_BMP)
	);


	this->minorClickSound.setBuffer(
		ResourceManager::getSoundBuffer(MENU_CLICK_MINOR_WAV)
	);
	this->majorClickSound.setBuffer(
		ResourceManager::getSoundBuffer(MENU_CLICK_MAJOR_WAV)
	);
}



void Menu::initializeSelectors() {
	this->initializeLevelDigits();
	this->initializeLevelSelectorBox();
	this->initializeMusicSelectors();
}



const std::vector<sf::Vector2f>& Menu::getDigitBoxesPositions() const {
	static std::vector<sf::Vector2f> positions = {
		sf::Vector2f(140, 152),
		sf::Vector2f(188, 152),
		sf::Vector2f(236, 152),
		sf::Vector2f(284, 152),
		sf::Vector2f(332, 152),
		sf::Vector2f(140, 201),
		sf::Vector2f(188, 201),
		sf::Vector2f(236, 201),
		sf::Vector2f(284, 201),
		sf::Vector2f(332, 201)
	};
	return positions;
}



void Menu::initializeLevelDigits() {
	static const int32_t FONT_SIZE = 24;
	auto& font = ResourceManager::getFont(GAME_FONT);

	auto& positions = this->getDigitBoxesPositions();
	sf::Vector2f offset(10, 12); // To center digits in boxes.

	for (int32_t digit = 0; digit < this->DIGITS; ++digit) {
		this->levelDigits[digit].setFont(font);
		this->levelDigits[digit].setString(std::to_string(digit));
		this->levelDigits[digit].setCharacterSize(FONT_SIZE);
		this->levelDigits[digit].setFillColor(sf::Color(216, 40, 0));
		this->levelDigits[digit].setStyle(sf::Text::Regular);
		this->levelDigits[digit].setPosition(positions[digit] + offset);
	}
}



void Menu::initializeLevelSelectorBox() {
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(42, 45));
	rectangle.setFillColor(sf::Color::Yellow);
	this->levelSelectorBox.setDrawable(rectangle);
	this->levelSelectorBox.setBlinkingTiming(this->SELECTORS_BLINK_TIMING);
	this->updateLevelSelectorBoxPosition();
}



void Menu::initializeMusicSelectors() {
	static const int32_t FONT_SIZE = 24;
	auto& font = ResourceManager::getFont(GAME_FONT);
	std::string strings[this->MUSIC_SELECTORS] = { ">", "<" };

	for (int32_t i = 0; i < this->MUSIC_SELECTORS; ++i) {
		sf::Text text;
		text.setFont(font);
		text.setString(strings[i]);
		text.setCharacterSize(FONT_SIZE);
		text.setFillColor(sf::Color::Yellow);
		text.setStyle(sf::Text::Regular);
		this->musicSelectors[i].setDrawable(text);
		this->musicSelectors[i].setBlinkingTiming(this->SELECTORS_BLINK_TIMING);
	}
	this->updateMusicSelectorsPositions();
}

#pragma /* Initialization */ endregion



#pragma region Update


void Menu::update() {
	this->updateSceneControls();
	this->updateSelectorsControls();
}



void Menu::updateSceneControls() {
	this->nextScene = Scene::NONE;

	if (Keyboard::isKeyPushed(GamePadKey::START)) {
		this->majorClickSound.play();
		this->nextScene = Scene::GAME;
	}
	else if (Keyboard::isKeyPushed(GamePadKey::B)) {
		this->majorClickSound.play();
		this->nextScene = Scene::CONTROLS_SCREEN;
	}
	else if (Keyboard::isKeyPushed(GamePadKey::EXIT)) {
		this->nextScene = Scene::EXIT;
	}
}



void Menu::updateSelectorsControls() {
	this->updateLevelSelect();
	this->updateMusicSelect();
}



void Menu::updateLevelSelect() {
	this->levelSelectorBox.update();

	if (Keyboard::isKeyPushed(GamePadKey::LEFT)) {
		this->minorClickSound.play();
		if (this->startLevel > this->MINIMAL_LEVEL) {
			--this->startLevel;
			this->updateLevelSelectorBoxPosition();
		}
	}
	else if (Keyboard::isKeyPushed(GamePadKey::RIGHT)) {
		this->minorClickSound.play();
		if (this->startLevel < this->MAXIMAL_LEVEL) {
			++this->startLevel;
			this->updateLevelSelectorBoxPosition();
		}
	}
}



void Menu::updateMusicSelect() {
	for (size_t i = 0; i < this->MUSIC_SELECTORS; ++i) {
		this->musicSelectors[i].update();
	}

	if (Keyboard::isKeyPushed(GamePadKey::UP)) {
		this->minorClickSound.play();
		if (this->musicType > this->MINIMAL_MUSIC_TYPE) {
			--this->musicType;
			this->updateMusicSelectorsPositions();
		}
	}
	else if (Keyboard::isKeyPushed(GamePadKey::DOWN)) {
		this->minorClickSound.play();
		if (this->musicType < this->MAXIMAL_MUSIC_TYPE) {
			++this->musicType;
			this->updateMusicSelectorsPositions();
		}
	}
}



void Menu::updateLevelSelectorBoxPosition() {
	auto& positions = this->getDigitBoxesPositions();
	this->levelSelectorBox.setPosition(positions[startLevel]);
}



void Menu::updateMusicSelectorsPositions() {
	static std::vector<float> xPositions = {
		435, 641
	};
	static std::vector<float> yPositions = {
		164, 212, 260, 308
	};
	for (size_t i = 0; i < this->MUSIC_SELECTORS; ++i) {
		this->musicSelectors[i].setPosition({
			xPositions[i],
			yPositions[this->musicType]
		});
	}
}



void Menu::draw(sf::RenderWindow& window) {
	window.clear(sf::Color::Black);

	window.draw(this->background);

	this->levelSelectorBox.drawOn(window);
	for (size_t i = 0; i < DIGITS; ++i) {
		window.draw(this->levelDigits[i]);
	}

	for (size_t i = 0; i < MUSIC_SELECTORS; ++i) {
		this->musicSelectors[i].drawOn(window);
	}

	window.display();
}

#pragma /* Update */ endregion


void Menu::reset() {
	this->startLevel = 0;
	this->updateLevelSelectorBoxPosition();

	this->musicType = 0;
	this->updateMusicSelectorsPositions();
}



std::pair<int32_t, int32_t> Menu::getGameParameters() {
	auto level = this->startLevel;
	if (Keyboard::isKeyHeld(GamePadKey::A)) {
		level += this->LEVEL_INCREMENT_HARD_MODE;
	}
	return { level, this->musicType };
}
