#include "tetris/scene_handling/ResourceManager.h"

#include <vector>

#include "tetris/helper/Helper.h"

using namespace tetris::scene_handling;


ResourceManager::ResourceManager()
{ }

#pragma region Static Members Initialization

std::map<int32_t, sf::Texture>		ResourceManager::textures;
std::map<int32_t, sf::SoundBuffer>	ResourceManager::soundBuffers;
std::map<int32_t, std::pair<std::vector<byte>, sf::Font>> ResourceManager::fonts;

#pragma /* Static Members Initialization */ endregion


#pragma region Resource Loading


void ResourceManager::loadResources() {
	ResourceManager::loadTextures();
	ResourceManager::loadSounds();
	ResourceManager::loadFonts();
}



void ResourceManager::loadTextures() {
	static std::vector<int32_t> idTextures = {
		// Backgrounds.
		SPLASH_SCREEN_BMP,
		CONTROLS_SCREEN_BMP,
		MENU_SCREEN_BMP,
		GAME_SCREEN_BMP,
		PAUSE_SCREEN_BMP,

		// Sprites.
		DROUGHT_INDICATOR_BMP,
		BLOCKS_BMP
	};

	for (auto id : idTextures) {
		ResourceManager::textures[id] = sf::Texture();
		std::vector<byte> bytes = helper::loadEmbeddedResource(id);
		ResourceManager::textures[id].loadFromMemory(&bytes[0], bytes.size());
	}
}



void ResourceManager::loadSounds() {
	static std::vector<int32_t> idSounds = {
		MENU_CLICK_MAJOR_WAV,
		MENU_CLICK_MINOR_WAV,
		PAUSE_WAV,

		TETRIMINO_MOVE_WAV,
		TETRIMINO_ROTATE_WAV,
		TETRIMINO_LAND_WAV,

		LINE_CLEARED_WAV,
		TETRIS_CLEARED_WAV,
		NEW_LEVEL_WAV,

		GAME_OVER_WAV,
	};

	for (auto id : idSounds) {
		ResourceManager::soundBuffers[id] = sf::SoundBuffer();
		std::vector<byte> bytes = helper::loadEmbeddedResource(id);
		ResourceManager::soundBuffers[id].loadFromMemory(&bytes[0], bytes.size());
	}
}



void ResourceManager::loadFonts() {
	static std::vector<int32_t> idFonts = {
		GAME_FONT
	};

	for (auto id : idFonts) {

		ResourceManager::fonts[id] = { std::vector<byte>(), sf::Font() };
		ResourceManager::fonts[id].first = helper::loadEmbeddedResource(id);
		ResourceManager::fonts[id].second.loadFromMemory(
			&ResourceManager::fonts[id].first[0],
			ResourceManager::fonts[id].first.size()
		);
	}
}

#pragma /* Resource Loading */ endregion


#pragma region Resource Access


const sf::Texture& ResourceManager::getTexture(int32_t id) {
	if (ResourceManager::textures.size() == 0) {
		ResourceManager::loadResources();
	}
	return ResourceManager::textures[id];
}



const sf::SoundBuffer& ResourceManager::getSoundBuffer(int32_t id) {
	if (ResourceManager::soundBuffers.size() == 0) {
		ResourceManager::loadResources();
	}
	return ResourceManager::soundBuffers[id];
}



const sf::Font& ResourceManager::getFont(int32_t id) {
	if (ResourceManager::fonts.size() == 0) {
		ResourceManager::loadResources();
	}
	return ResourceManager::fonts[id].second;
}

#pragma /* Resource Access */ endregion
