#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SfML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Sound.hpp>

#include "tetris/scene_handling/SceneHandlerBase.h"
#include "tetris/wrapper/BlinkingObject.h"



namespace tetris::scene_handling {
	class Menu : public SceneHandlerBase {
	public:
		Menu();

		void update();
		void draw(sf::RenderWindow& window);

		void reset();

		std::pair<int32_t, int32_t> getGameParameters();
	private:
		void updateSceneControls();
		void updateSelectorsControls();
		void updateLevelSelect();
		void updateMusicSelect();

		sf::Sprite background;

		sf::Sound minorClickSound;
		sf::Sound majorClickSound;

		void initializeResources();
		void initializeSelectors();

		static const int32_t SELECTORS_BLINK_TIMING = 15;



		static const int32_t DIGITS = 10;
		sf::Text levelDigits[DIGITS];
		const std::vector<sf::Vector2f>& getDigitBoxesPositions() const;
		void initializeLevelDigits();

		static const int32_t MINIMAL_LEVEL = 0;
		static const int32_t MAXIMAL_LEVEL = 9;
		int32_t startLevel;

		wrapper::BlinkingObject<sf::RectangleShape> levelSelectorBox;
		void initializeLevelSelectorBox();
		void updateLevelSelectorBoxPosition();

		// Defines how many levels to add if player wants "hard mode".
		static const int32_t LEVEL_INCREMENT_HARD_MODE = 10;



		static const int32_t MINIMAL_MUSIC_TYPE = 0;
		static const int32_t MAXIMAL_MUSIC_TYPE = 3;
		int32_t musicType;

		static const int32_t MUSIC_SELECTORS = 2;
		wrapper::BlinkingObject<sf::Text> musicSelectors[MUSIC_SELECTORS];
		void initializeMusicSelectors();
		void updateMusicSelectorsPositions();
	};
}