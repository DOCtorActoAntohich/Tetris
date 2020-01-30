#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <vector>

#include "tetris/wrapper/ControlKey.h"

namespace tetris::wrapper {
	// Small Wrapper over sf::Keyboard to make it a bit more comfortable.
	// Must be initialized first. Will not do anything otherwise.
	// This class is STATIC.
	class Keyboard {
	public:
		// Initializes keyboard to make it
		static void initialize();

		// Saves current state to previous and updates key states.
		static void update();

		// Checks if key has state DOWN (pressed for a while).
		static bool isKeyHeld(ControlKey key);

		// Checks if key changed state from UP to DOWN.
		static bool isKeyPushed(ControlKey key);

		// Checks if key changed state from DOWN to UP.
		static bool isKeyReleased(ControlKey key);
	private:
		Keyboard();

		// Saves current state of keyboard to previous.
		static void saveState();

		// Transforms input key using defined map.
		static sf::Keyboard::Key transformKey(ControlKey key);

		static const std::map<ControlKey, sf::Keyboard::Key>& getKeysMap();

		static bool isInitialized;

		static std::map<sf::Keyboard::Key, bool> currentState;
		static std::map<sf::Keyboard::Key, bool> previousState;
	};
}