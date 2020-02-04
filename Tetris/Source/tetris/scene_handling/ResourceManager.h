#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <map>

#include "tetris/helper/Helper.h"
#include "resource.h"



namespace tetris::scene_handling {
	// A container that allows to easily store resources and get them.
	// This class is STATIC.
	class ResourceManager {
	public:
		// Loads all resourses embedded into file.
		static void loadResources();

		static const sf::Texture&     getTexture(int32_t id);
		static const sf::SoundBuffer& getSoundBuffer(int32_t id);
		static const sf::Font&		  getFont(int32_t id);
	private:
		ResourceManager();

		static void loadTextures();
		static void loadSounds();
		static void loadFonts();


		static std::map<int32_t, sf::Texture>	  textures;
		static std::map<int32_t, sf::SoundBuffer> soundBuffers;
		static std::map<int32_t, std::pair<std::vector<byte>, sf::Font>> fonts;
	};
}