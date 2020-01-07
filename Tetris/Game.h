#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <vector>
#include <filesystem>
#include <ResourceIndexer.h>

#include "Scene.h"
#include "Keyboard.h"
#include "GameField.h"
#include "Score.h"
#include "SpriteWrapper.h"
#include "SoundWrapper.h"
#include "CounterUI.h"


namespace tetris {
	using Sound = SoundWrapper;
	using Sprite = SpriteWrapper;


	class Game {
	public:
		Game();

		// Runs the game processes.
		void run();
	private:

#pragma region Resource Management


		sf::RenderWindow window;
		void initializeWindow();


#pragma region Loading Resources

		// Returns a font loaded from embedded resources.
		// Font data is saved in fontBytes which MUST NOT be changed. 
		void loadFont(int32_t id, std::vector<byte>& fontbytes, sf::Font& font);// TODO: Use size 18.

		// Opens a music file from embedded resources.
		// Music data is saved in musicBytes which MUST NOT be changed.
		void openMusic(int32_t id, std::vector<byte>& musicBytes, sf::Music& music);

#pragma /* Loading Resources */ endregion


#pragma region Textures

		Sprite splash_background;
		Sprite controls_background;
		Sprite menu_background;
		Sprite game_background;
		Sprite pause_background;

		Sprite droughtIndicator;

#pragma /* Textures */ endregion


#pragma region Other Data

		std::vector<byte> fontBytes;
		sf::Font font;
		const int32_t FONT_SIZE = 24;

		sf::Text pressEnter_text;
		bool pressEnter_text_isVisible;
		void pressEnter_text_initialize();

		

		static const int32_t DIGITS = 10;
		std::vector<sf::Vector2f> predefinedLevelDigitPositions;
		void predefinedDigitPositions_initialize();

		sf::RectangleShape menu_levelHighlighter;
		bool menu_isLevelHighlighterVisible;
		void menu_levelHighlighter_initialize();
		void menu_levelHighlighter_update();

		sf::Text menu_levelDigits[DIGITS];
		void menu_levelDigits_initialize();



		static const int32_t MUSIC_HIGHLIGHTERS = 2;
		std::vector<float> predefinedMusicHighlightersPositionsX;
		std::vector<float> predefinedMusicHighlightersPositionsY;
		void predefinedMusicHighlightersPositions_initialize();

		sf::Text menu_musicHighlighters[MUSIC_HIGHLIGHTERS];
		bool menu_areMusicHighlightersVisible;
		void menu_musicHighlighters_initialize();
		void menu_musicHighlighters_update();



		CounterUI game_tetrisesCounter;
		CounterUI game_burnCounter;
		CounterUI game_tetrisRateCounter;
		CounterUI game_linesCounter;
		CounterUI game_topScoreCounter;
		CounterUI game_currentScoreCounter;
		CounterUI game_levelCounter;
		CounterUI game_droughtCounter;

#pragma /* Other Data */ endregion


#pragma region Sounds

		Sound menuClickMajor_sound;
		Sound menuClickMinor_sound;

		Sound pause_sound;

#pragma /* Sounds */ endregion

		// Loads data to be used.
		void loadContent();

		// Frees memory.
		void unloadContent();

#pragma /* Resource Management */ endregion


#pragma region Main Loop

		// Closes the game window.
		void exit();

		Scene scene;
		bool isInputAllowed;
		Keyboard keyboard;

		// How many frames have passed.
		// Used to check timed events.
		// Set to 0 upon scene changes.
		int32_t frames;
		const int32_t FPS = 60;

		// Runs the game loop.
		void runMainLoop();

		// Delegate calling scene updaters.
		void (Game::* update)();

		// Delegate calling scene drawers.
		void (Game::* draw)();

		// Changes scene handlers.
		void changeScene(Scene scene);

		// Returns pointers to handlers of a specified scene.
		// First handler is updater, second is drawer.
		std::pair<void(Game::*)(), void(Game::*)()> chooseUpdaters(Scene scene);

#pragma region Scene Handlers

#pragma region Splash Screen


		void update_SplashScreen();
		void draw_SplashScreen();

#pragma /* Splash Screen */ endregion

#pragma region Controls Screen

		void update_ControlsScreen();
		void draw_ControlsScreen();

#pragma /* Controls Screen */ endregion 

#pragma region Menu

		// Boundaries only for level and music select in menu.

		const uint8_t MINIMAL_LEVEL = 0;
		const uint8_t MAXIMAL_LEVEL = 9;
		uint8_t startLevel;

		const int32_t MINIMAL_MUSIC_TYPE = 0;
		const int32_t MAXIMAL_MUSIC_TYPE = 3;
		int32_t musicType;


		void update_Menu();
		void update_Menu_LevelSelection();
		void update_Menu_MusicSelection();

		void draw_Menu();

#pragma /* Menu */ endregion

#pragma region Game

		GameField field;

		void update_Game();
		void draw_Game();

#pragma /* Game */ endregion

#pragma region Pause Screen

		void update_PauseScreen();
		void draw_PauseScreen();

#pragma /* Pause Screen */ endregion

#pragma /* Scene Handlers */ endregion


#pragma /* Main Loop */ endregion

	};
}