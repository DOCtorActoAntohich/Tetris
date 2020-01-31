#include "tetris/scene_handling/ControlsScreen.h"

#include "tetris/scene_handling/ResourceManager.h"

using namespace tetris::scene_handling;



ControlsScreen::ControlsScreen() {
	this->background.setTexture(
		ResourceManager::getTexture(CONTROLS_SCREEN_BMP)
	);
	this->clickSound.setBuffer(
		ResourceManager::getSoundBuffer(MENU_CLICK_MAJOR_WAV)
	);
}



void ControlsScreen::update() {
	this->nextScene = Scene::NONE;

	if (Keyboard::isKeyPushed(GamePadKey::START)) {
		this->nextScene = Scene::MENU;
		this->clickSound.play();
	}
	else if (Keyboard::isKeyPushed(GamePadKey::B)) {
		this->nextScene = Scene::SPLASH_SCREEN;
		this->clickSound.play();
	}
	else if (Keyboard::isKeyPushed(GamePadKey::EXIT)) {
		this->nextScene = Scene::EXIT;
	}
}



void ControlsScreen::draw(sf::RenderWindow& window) {
	window.clear(sf::Color::Black);

	window.draw(this->background);

	window.display();
}



void ControlsScreen::reset()
{ }
