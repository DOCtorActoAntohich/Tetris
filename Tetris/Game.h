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

		Sprite game_droughtIndicator;

		Sprite game_blocks;

		// Actual block size (in pixels).
		static const uint32_t BLOCK_SIZE = 19;

		// Distance between blocks (in pixels).
		static const uint32_t BLOCK_GAP = 2;

		// Block size needed for calculating screen positions.
		static const uint32_t BLOCK_SIZE_WITH_GAP = Game::BLOCK_SIZE + Game::BLOCK_GAP;


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

		void initializeCounters();

		bool shouldCallDrawer;

#pragma /* Other Data */ endregion


#pragma region Sounds

		Sound menuClickMajor_sound;
		Sound menuClickMinor_sound;

		Sound pause_sound;

		Sound tetrominoMove_sound;
		Sound tetrominoRotate_sound;
		Sound tetrominoLand_sound;

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


		void splashScreen_update();
		void splashScreen_draw();

#pragma /* Splash Screen */ endregion

#pragma region Controls Screen

		void controlsScreen_update();
		void controlsScreen_draw();

#pragma /* Controls Screen */ endregion 

#pragma region Menu

		// Boundaries only for level and music select in menu.

		static const uint32_t MINIMAL_LEVEL = 0;
		static const uint32_t MAXIMAL_LEVEL = 9;

		// Defines how many levels to add if player wants "hard mode".
		static const uint32_t LEVEL_INCREMENT_HARD_MODE = 10;

		// Controls from which level the game will start.
		uint32_t menu_startLevel;

		static const int32_t MINIMAL_MUSIC_TYPE = 0;
		static const int32_t MAXIMAL_MUSIC_TYPE = 3;

		// Controls the music type to be played.
		uint32_t menu_musicType;


		void menu_update();

		void menu_updateLevelSelection();
		void menu_updateMusicSelection();

		// Sets start level to a level counter.
		void menu_setStartLevel();

		void menu_draw();

#pragma /* Menu */ endregion

#pragma region Game

		GameField game_field;

		sf::Vector2f game_BlocksDrawingOffset;

		void game_update();
		void game_updateFigureControls();

		void game_drawCounters();
		void game_drawBlocks();
		void game_drawField();
		void game_drawCurrentPiece();
		void game_drawSingleBlock(const sf::Vector2f& position, Tetromino::Type type);
		void game_draw();

#pragma /* Game */ endregion

#pragma region Pause Screen

		void pauseScreen_update();
		void pauseScreen_draw();

#pragma /* Pause Screen */ endregion

#pragma /* Scene Handlers */ endregion


#pragma /* Main Loop */ endregion

	};
}