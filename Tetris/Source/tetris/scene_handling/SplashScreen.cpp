#include "tetris/scene_handling/SplashScreen.h"

#include "tetris/wrapper/Keyboard.h"
#include "tetris/scene_handling/ResourceManager.h"
#include "resource.h"

using namespace tetris::scene_handling;

#pragma region Initialization


SplashScreen::SplashScreen() : SceneHandlerBase() {
	this->initializeResources();
	this->initializeText();
}



void SplashScreen::initializeResources() {
	this->background.setTexture(
		ResourceManager::getTexture(SPLASH_SCREEN_BMP)
	);
	this->click_sound.setBuffer(
		ResourceManager::getSoundBuffer(MENU_CLICK_MINOR_WAV)
	);
}



void SplashScreen::initializeText() {
	this->textBlinkTimer.setTimingFrames(this->TEXT_BLINK_TIMING);
	this->isTextVisible = true;

	this->pressEnter_text.setFont(
		ResourceManager::getFont(GAME_FONT)
	);
	this->pressEnter_text.setString("PRESS ENTER");
	this->pressEnter_text.setCharacterSize(this->FONT_SIZE);
	this->pressEnter_text.setFillColor(sf::Color::White);
	this->pressEnter_text.setStyle(sf::Text::Regular);
	this->pressEnter_text.setPosition(256, 326);
}

#pragma /* Initialization */ endregion


#pragma region Main Loop


void SplashScreen::update() {
	this->textBlinkTimer.update();

	if (this->textBlinkTimer.isTriggered()) {
		this->isTextVisible = !this->isTextVisible;
	}

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

	if (this->isTextVisible) {
		window.draw(this->pressEnter_text);
	}

	window.display();
}

#pragma /* Main Loop */ endregion
