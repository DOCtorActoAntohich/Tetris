#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <vector>
#include <filesystem>
#include <ResourceIndexer.h>

#include "tetris/scene_handling/Scene.h"
#include "tetris/wrapper/Keyboard.h"
#include "tetris/GameField.h"
#include "tetris/Score.h"
#include "tetris/wrapper/Sprite.h"
#include "tetris/wrapper/Sound.h"
#include "tetris/wrapper/CounterUI.h"
#include "tetris/wrapper/FrameTimer.h"


namespace tetris {
	class Game {
		using GamePadKey = wrapper::GamePadKey;
		using Keyboard = wrapper::Keyboard;

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

		wrapper::Sprite splash_background;
		wrapper::Sprite controls_background;
		wrapper::Sprite menu_background;
		wrapper::Sprite game_background;
		wrapper::Sprite pause_background;

		wrapper::Sprite game_droughtIndicator;

		wrapper::Sprite game_blocks;

		// Actual block size (in pixels).
		static const int32_t BLOCK_SIZE = 19;

		// Distance between blocks (in pixels).
		static const int32_t BLOCK_GAP = 2;

		// Block size needed for calculating screen positions.
		static const int32_t BLOCK_SIZE_WITH_GAP = Game::BLOCK_SIZE + Game::BLOCK_GAP;


#pragma /* Textures */ endregion


#pragma region Other Data

		std::vector<byte> fontBytes;
		sf::Font font;
		static const int32_t FONT_SIZE = 24;

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



		wrapper::CounterUI game_tetrisesCounter;
		wrapper::CounterUI game_burnCounter;
		wrapper::CounterUI game_tetrisRateCounter;
		wrapper::CounterUI game_linesCounter;
		wrapper::CounterUI game_topScoreCounter;
		wrapper::CounterUI game_currentScoreCounter;
		wrapper::CounterUI game_levelCounter;
		wrapper::CounterUI game_droughtCounter;

		wrapper::CounterUI game_pieceCounter_T;
		wrapper::CounterUI game_pieceCounter_J;
		wrapper::CounterUI game_pieceCounter_Z;
		wrapper::CounterUI game_pieceCounter_O;
		wrapper::CounterUI game_pieceCounter_S;
		wrapper::CounterUI game_pieceCounter_L;
		wrapper::CounterUI game_pieceCounter_I;

		void initializeCounters();
		void initializePieceCounters();
		void initializeStaticticsCounters();

		bool shouldCallDrawer;


		std::vector<wrapper::CounterUI> game_pieceCounters;
		std::vector<std::tuple<const Tetrimino::Matrix::Array*, sf::Vector2f>> game_staticticsBlocksData;
		void initializeStatisticsPieceData();

#pragma /* Other Data */ endregion


#pragma region Sounds

		wrapper::Sound menuClickMajor_sound;
		wrapper::Sound menuClickMinor_sound;

		wrapper::Sound pause_sound;

		wrapper::Sound tetriminoMove_sound;
		wrapper::Sound tetriminoRotate_sound;
		wrapper::Sound tetriminoLand_sound;

		wrapper::Sound lineCleared_sound;
		wrapper::Sound tetrisCleared_sound;
		wrapper::Sound newLevel_sound;

		wrapper::Sound gameOver_sound;

#pragma /* Sounds */ endregion

		// Loads data to be used.
		void loadContent();

		// Frees memory.
		void unloadContent();

#pragma /* Resource Management */ endregion


#pragma region Main Loop
		
		// Closes the game window.
		void exit();

		scene_handling::Scene scene;

		static const int32_t FPS = 60;

		// Runs the game loop.
		void runMainLoop();

		// Delegate calling scene updaters.
		void (Game::* update)();

		// Delegate calling scene drawers.
		void (Game::* draw)();

		// Changes scene handlers.
		void changeScene(scene_handling::Scene scene);

		// Returns pointers to handlers of a specified scene.
		// First handler is updater, second is drawer.
		std::pair<void(Game::*)(), void(Game::*)()> chooseUpdaters(scene_handling::Scene scene);

#pragma region Scene Handlers

#pragma region Splash Screen

		// How often splash screen text will blink (in frames).
		static const int32_t SPLASH_TEXT_BLINK_TIMING = 30;

		// Frames counter for blinking text.
		wrapper::FrameTimer splashScreen_textBlinkTimer;

		void splashScreen_update();
		void splashScreen_draw();

#pragma /* Splash Screen */ endregion

#pragma region Controls Screen

		void controlsScreen_update();
		void controlsScreen_draw();

#pragma /* Controls Screen */ endregion 

#pragma region Menu

		// How often menu highlighters will blink (in frames).
		static const int32_t MENU_HIGHLIGHTERS_BLINK_TIMING = 15;

		// Frames counter for blinking menu highlighters.
		wrapper::FrameTimer menu_highlightersBlinkTimer;

		// Boundaries only for level and music select in menu.

		static const int32_t MINIMAL_LEVEL = 0;
		static const int32_t MAXIMAL_LEVEL = 9;

		// Defines how many levels to add if player wants "hard mode".
		static const int32_t LEVEL_INCREMENT_HARD_MODE = 10;

		// Controls from which level the game will start.
		int32_t menu_startLevel;

		static const int32_t MINIMAL_MUSIC_TYPE = 0;
		static const int32_t MAXIMAL_MUSIC_TYPE = 3;

		// Controls the music type to be played.
		int32_t menu_musicType;


		void menu_update();

		void menu_updateLevelSelection();
		void menu_updateMusicSelection();
		void menu_prepareForGame();

		// Sets start level to a level counter.
		void menu_setStartLevel();

		void menu_draw();

#pragma /* Menu */ endregion

#pragma region Game
		
		// For timing animations of clearing lines and curtain.
		static const int32_t ANIMATION_TIMING = 4;
		wrapper::FrameTimer game_animationTimer;
		bool game_isPerformingAnimation;

		int32_t game_blocksToCover;
		static const int32_t MAX_BLOCKS_TO_COVER = 5;

		static const int32_t FIRST_SPAWN_DROP_DELAY = 120;
		wrapper::FrameTimer game_firstSpawnTimer;
		bool game_isFirstSpawn;

		// How fast figures fall on soft drop.
		static const int32_t SOFT_DROP_DELAY = 2;
		wrapper::FrameTimer game_softDropTimer;
		bool game_allowSoftDrop;

		static const int32_t LEVEL_WITH_MAX_SPEED = 29;
		std::vector<int32_t> framesPerGridcell;
		wrapper::FrameTimer game_dropTimer;

		// Handles most of game scene processes.
		GameField game_field;

		// Offset from (0, 0) for drawing game field.
		sf::Vector2f game_blocksDrawingOffset;

		static const int32_t RESPAWN_DELAY = 10;
		wrapper::FrameTimer game_respawnTimer;
		bool game_hasPieceLanded;
		

#pragma region DAS

		// States for DAS (Delayed Auto Shift).
		enum class DasState {
			// Not applied.
			NONE = 0,

			// Move piece for the first time with long delay.
			LONG_DELAYED_MOVE = 1,

			// Move piece for the second (and other) time with short delay.
			SHORT_DELAYED_MOVE = 2
		};
		DasState game_dasState;
		Direction game_previousMoveDirection;
		Direction game_currentMoveDirection;

		// Controls DAS.
		wrapper::FrameTimer game_dasTimer;

		// Delay for 1st tetrimino moves.
		static const int32_t DAS_DELAY_LONG = 16;

		// Delay for 2nd and other tetrimino moves.
		static const int32_t DAS_DELAY_SHORT = 6;

		void game_updateDas();
		void game_updateDas_controls();
		void game_updateDas_movePiece();

#pragma /* DAS */ endregion

		void game_update();
		void game_updatePieceControls();
		void game_dropPieceDown(bool isSoftDrop);
		void game_movePiece(Direction direction);
		void game_updateCounters();
		void game_updateStatisticsCounters();
		void game_updatePieceCounters();
		

		void game_drawCounters();
		void game_drawStaticticsCounters();
		void game_drawPieceCounters();
		void game_drawBlocks();
		void game_drawField();
		void game_drawFieldCovered();
		void game_drawCurrentPiece();
		void game_drawStaticticsBlocks();
		void game_drawNextPiece();
		void game_drawPiece(const Tetrimino::Matrix::Array& matrix, const sf::Vector2f& offset);
		void game_drawSingleBlock(const sf::Vector2f& position, Tetrimino::Type type);
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