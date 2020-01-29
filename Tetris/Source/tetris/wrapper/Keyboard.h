#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <vector>

#include "tetris/wrapper/ControlKey.h"

namespace tetris::wrapper {
	// Small Wrapper over sf::Keyboard to make it a bit more comfortable.
	class Keyboard {
	public:
		Keyboard();

		// Saves current state to previous and updates key states.
		void update();

		// Checks if key has state DOWN (pressed for a while).
		bool isKeyHeld(ControlKey key);

		// Checks if key changed state from UP to DOWN.
		bool isKeyPushed(ControlKey key);

		// Checks if key changed state from DOWN to UP.
		bool isKeyReleased(ControlKey key);
	private:
		// Returns the list of sensitive keys (that should be checked).
		std::vector<sf::Keyboard::Key> getKeysList();


		std::map<ControlKey, sf::Keyboard::Key> keysMap_;

		// Defines a default map from "ControlKey" to "sf::Keyboard::Key".
		std::map<ControlKey, sf::Keyboard::Key> defineKeyMap();

		// Transforms input key using defined map.
		sf::Keyboard::Key transformKey(ControlKey key);


		// Saves current state of pressed keys to previous state.
		void saveState();


		std::map<sf::Keyboard::Key, bool> currentState_;
		std::map<sf::Keyboard::Key, bool> previousState_;
	};
}