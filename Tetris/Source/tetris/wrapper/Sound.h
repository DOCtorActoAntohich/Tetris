#pragma once

#include <SFML/Audio.hpp>

namespace tetris::wrapper {
	// A wrapper for sf::Sound.
	class Sound {
	public:
		// Loads sound from embedded resource.
		// Stores data in soundBuffer and links it to sound.
		void loadFromResource(int32_t id);

		// Plays the sound.
		void play();
	private:
		sf::SoundBuffer soundBuffer;
		sf::Sound       sound;
	};
}
