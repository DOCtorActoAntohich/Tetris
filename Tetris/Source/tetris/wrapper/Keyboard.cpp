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


#pragma warning(push)
// I can do nothing with this warning since it's caused by SFML.
#pragma warning(disable : 26812)

void Keyboard::initialize() {
	auto& keysMap = Keyboard::getKeysMap();
	for (auto [_, sfKey] : keysMap) {
		Keyboard::currentState[sfKey] = false;
	}
	Keyboard::saveState();
	Keyboard::isInitialized = true;
}

#pragma warning(pop)



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


bool Keyboard::isKeyHeld(ControlKey key) {
	if (!Keyboard::isInitialized) {
		return false;
	}
	sf::Keyboard::Key sfKey = Keyboard::transformKey(key);
	return Keyboard::currentState[sfKey];
}



bool Keyboard::isKeyPushed(ControlKey key) {
	if (!Keyboard::isInitialized) {
		return false;
	}
	sf::Keyboard::Key sfKey = Keyboard::transformKey(key);
	return Keyboard::currentState[sfKey] && !Keyboard::previousState[sfKey];
}



bool Keyboard::isKeyReleased(ControlKey key) {
	if (!Keyboard::isInitialized) {
		return false;
	}
	sf::Keyboard::Key sfKey = Keyboard::transformKey(key);
	return !Keyboard::currentState[sfKey] && Keyboard::previousState[sfKey];
}



sf::Keyboard::Key Keyboard::transformKey(ControlKey key) {
	auto& keysMap = Keyboard::getKeysMap();
	return keysMap.at(key);
}



const std::map<ControlKey, sf::Keyboard::Key>& Keyboard::getKeysMap() {
	static std::map<ControlKey, sf::Keyboard::Key> keysMap = {
		{ ControlKey::UP,	 sf::Keyboard::Key::W },
		{ ControlKey::DOWN,  sf::Keyboard::Key::S },
		{ ControlKey::LEFT,  sf::Keyboard::Key::A },
		{ ControlKey::RIGHT, sf::Keyboard::Key::D },
		{ ControlKey::START, sf::Keyboard::Key::Enter },
		{ ControlKey::B,	 sf::Keyboard::Key::Left },
		{ ControlKey::A,	 sf::Keyboard::Key::Right },
		{ ControlKey::EXIT,	 sf::Keyboard::Key::Escape }
	};
	return keysMap;
}

#pragma /* State Checking */ endregion
