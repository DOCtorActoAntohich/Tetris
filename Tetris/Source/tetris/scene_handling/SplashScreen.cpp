#include "tetris/scene_handling/SplashScreen.h"

#include "tetris/scene_handling/Scene.h"
#include "tetris/scene_handling/ResourceManager.h"
#include "tetris/wrapper/Keyboard.h"

using namespace tetris::scene_handling;

#pragma region Initialization


SplashScreen::SplashScreen() : SceneHandlerBase() {
	this->initializeResources();
	this->initializeBlinkingText();
}



void SplashScreen::initializeResources() {
	this->background.setTexture(
		ResourceManager::getTexture(SPLASH_SCREEN_BMP)
	);
	this->click_sound.setBuffer(
		ResourceManager::getSoundBuffer(MENU_CLICK_MAJOR_WAV)
	);
}



void SplashScreen::initializeBlinkingText() {
	static const int32_t FONT_SIZE = 24;
	static const int32_t TEXT_BLINK_TIMING = 30;

	sf::Text text;
	text.setFont(
		ResourceManager::getFont(GAME_FONT)
	);
	text.setString("PRESS ENTER");
	text.setCharacterSize(FONT_SIZE);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Regular);
	text.setPosition(256, 326);
	this->pressEnter_text.setDrawable(text);
	this->pressEnter_text.setBlinkingTiming(TEXT_BLINK_TIMING);
}

#pragma /* Initialization */ endregion



#pragma region Main Loop


void SplashScreen::update() {
	this->pressEnter_text.update();
	
	this->updateSceneControls();
}



void SplashScreen::updateSceneControls() {
	this->nextScene = Scene::NONE;

	if (Keyboard::isKeyPushed(GamePadKey::START)) {
		this->nextScene = Scene::CONTROLS_SCREEN;
		this->click_sound.play();
	}
	else if (Keyboard::isKeyPushed(GamePadKey::EXIT)) {
		this->nextScene = Scene::EXIT;
	}
}



void SplashScreen::draw(sf::RenderWindow& window) {
	window.clear(sf::Color::Black);

	window.draw(this->background);

	this->pressEnter_text.drawOn(window);

	window.display();
}



void SplashScreen::reset() {
	this->pressEnter_text.reset();
}

#pragma /* Main Loop */ endregion
