#include "Keyboard.h"


namespace tetris {
	#pragma warning(push)
	// I can do nothing with this warning since it's caused by SFML.
	// It distracts me.
	#pragma warning(disable : 26812)

	Keyboard::Keyboard() {
		this->keysMap_ = this->defineKeyMap();

		for (auto key : this->getKeysList()) {
			this->currentState_[key] = false;
		}
		this->saveState();
	}

	#pragma warning(pop)



	std::map<ControlKey, sf::Keyboard::Key> Keyboard::defineKeyMap() {
		return std::map<ControlKey, sf::Keyboard::Key>{
			{ ControlKey::UP,	 sf::Keyboard::Key::W		},
			{ ControlKey::DOWN,  sf::Keyboard::Key::S		},
			{ ControlKey::LEFT,  sf::Keyboard::Key::A		},
			{ ControlKey::RIGHT, sf::Keyboard::Key::D		},
			{ ControlKey::START, sf::Keyboard::Key::Enter	},
			{ ControlKey::B,	 sf::Keyboard::Key::Left	},
			{ ControlKey::A,	 sf::Keyboard::Key::Right	},
			{ ControlKey::EXIT,	 sf::Keyboard::Key::Escape	}
		};
	}



	void Keyboard::update() {
		this->saveState();

		for (auto key : this->getKeysList()) {
			this->currentState_[key] = sf::Keyboard::isKeyPressed(key);
		}
	}

	void Keyboard::saveState() {
		this->previousState_ = std::map<sf::Keyboard::Key, bool>(this->currentState_);
	}



	std::vector<sf::Keyboard::Key> Keyboard::getKeysList() {
		static std::vector<sf::Keyboard::Key> keys;
		static bool isInitialized = false;
		if (!isInitialized) {
			for (auto [_, value] : this->keysMap_) {
				keys.push_back(value);
			}
			isInitialized = true;
		}
		return keys;
	}



	sf::Keyboard::Key Keyboard::transformKey(ControlKey key) {
		return this->keysMap_[key];
	}



	bool Keyboard::isKeyHeld(ControlKey key) {
		sf::Keyboard::Key sfKey = this->transformKey(key);
		return this->currentState_[sfKey];
	}



	bool Keyboard::isKeyPushed(ControlKey key) {
		sf::Keyboard::Key sfKey = this->transformKey(key);
		return this->currentState_[sfKey] && !this->previousState_[sfKey];
	}



	bool Keyboard::isKeyReleased(ControlKey key) {
		sf::Keyboard::Key sfKey = this->transformKey(key);
		return !this->currentState_[sfKey] && this->previousState_[sfKey];
	}
}