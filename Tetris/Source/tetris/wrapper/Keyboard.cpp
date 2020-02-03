#include "tetris/wrapper/Keyboard.h"

using namespace tetris::wrapper;


#pragma region Static Members Initialization

bool Keyboard::isInitialized = false;

std::map<sf::Keyboard::Key, bool> Keyboard::currentState;
std::map<sf::Keyboard::Key, bool> Keyboard::previousState;

#pragma /* Static Members Initialization */ endregion



Keyboard::Keyboard()
{ }



#pragma region State Management

void Keyboard::initialize() {
	auto& keysMap = Keyboard::getKeysMap();
	for (auto [_, sfKey] : keysMap) {
		Keyboard::currentState[sfKey] = false;
	}
	Keyboard::saveState();
	Keyboard::isInitialized = true;
}



void Keyboard::update() {
	if (!Keyboard::isInitialized) {
		return;
	}

	Keyboard::saveState();
	
	auto& keysMap = Keyboard::getKeysMap();
	for (auto [_, sfKey] : keysMap) {
		Keyboard::currentState[sfKey] = sf::Keyboard::isKeyPressed(sfKey);
	}
}



void Keyboard::saveState() {
	Keyboard::previousState = Keyboard::currentState;
}

#pragma /* State Management */ endregion



#pragma region State Checking


bool Keyboard::isKeyHeld(GamePadKey key) {
	if (!Keyboard::isInitialized) {
		return false;
	}
	sf::Keyboard::Key sfKey = Keyboard::transformKey(key);
	return Keyboard::currentState[sfKey];
}



bool Keyboard::isKeyPushed(GamePadKey key) {
	if (!Keyboard::isInitialized) {
		return false;
	}
	sf::Keyboard::Key sfKey = Keyboard::transformKey(key);
	return Keyboard::currentState[sfKey] && !Keyboard::previousState[sfKey];
}



bool Keyboard::isKeyReleased(GamePadKey key) {
	if (!Keyboard::isInitialized) {
		return false;
	}
	sf::Keyboard::Key sfKey = Keyboard::transformKey(key);
	return !Keyboard::currentState[sfKey] && Keyboard::previousState[sfKey];
}



sf::Keyboard::Key Keyboard::transformKey(GamePadKey key) {
	auto& keysMap = Keyboard::getKeysMap();
	return keysMap.at(key);
}



const std::map<GamePadKey, sf::Keyboard::Key>& Keyboard::getKeysMap() {
	static std::map<GamePadKey, sf::Keyboard::Key> keysMap = {
		{ GamePadKey::UP,	 sf::Keyboard::Key::W },
		{ GamePadKey::DOWN,  sf::Keyboard::Key::S },
		{ GamePadKey::LEFT,  sf::Keyboard::Key::A },
		{ GamePadKey::RIGHT, sf::Keyboard::Key::D },
		{ GamePadKey::START, sf::Keyboard::Key::Enter },
		{ GamePadKey::B,	 sf::Keyboard::Key::Left },
		{ GamePadKey::A,	 sf::Keyboard::Key::Right },
		{ GamePadKey::EXIT,	 sf::Keyboard::Key::Escape }
	};
	return keysMap;
}

#pragma /* State Checking */ endregion
