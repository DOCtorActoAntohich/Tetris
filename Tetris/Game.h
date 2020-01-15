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
#include "FrameTimer.h"


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



		CounterUI game_tetrisesCounter;
		CounterUI game_burnCounter;
		CounterUI game_tetrisRateCounter;
		CounterUI game_linesCounter;
		CounterUI game_topScoreCounter;
		CounterUI game_currentScoreCounter;
		CounterUI game_levelCounter;
		CounterUI game_droughtCounter;

		CounterUI game_pieceCounter_T;
		CounterUI game_pieceCounter_J;
		CounterUI game_pieceCounter_Z;
		CounterUI game_pieceCounter_O;
		CounterUI game_pieceCounter_S;
		CounterUI game_pieceCounter_L;
		CounterUI game_pieceCounter_I;

		void initializeCounters();
		void initializePieceCounters();
		void initializeStaticticsCounters();

		bool shouldCallDrawer;


		std::vector<CounterUI> game_pieceCounters;
		std::vector<std::tuple<const Tetrimino::Matrix::Array*, sf::Vector2f>> game_staticticsBlocksData;
		void initializeStatisticsPieceData();

#pragma /* Other Data */ endregion


#pragma region Sounds

		Sound menuClickMajor_sound;
		Sound menuClickMinor_sound;

		Sound pause_sound;

		Sound tetriminoMove_sound;
		Sound tetriminoRotate_sound;
		Sound tetriminoLand_sound;

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

		static const int32_t FPS = 60;

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

		// How often splash screen text will blink (in frames).
		static const int32_t SPLASH_TEXT_BLINK_TIMING = 30;

		// Frames counter for blinking text.
		FrameTimer splashScreen_textBlinkTimer;

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
		FrameTimer menu_highlightersBlinkTimer;

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
		void menu_prepareForGame();

		// Sets start level to a level counter.
		void menu_setStartLevel();

		void menu_draw();

#pragma /* Menu */ endregion

#pragma region Game

		// States for DAS (Delayed Auto Shift).
		enum class DasState {
			NONE = 0,
			LONG_DELAYED_MOVE = 1,
			SHORT_DELAYED_MOVE = 2
		};
		DasState game_dasState;
		Direction game_previousMoveDirection;
		Direction game_currentMoveDirection;

		// Controls DAS.
		FrameTimer game_dasTimer;

		// Delay for 1st tetrimino moves.
		static const int32_t DAS_DELAY_LONG = 16;

		// Delay for 2nd and other tetrimino moves.
		static const int32_t DAS_DELAY_SHORT = 6;


		// How fast figures fall on soft drop.
		static const int32_t SOFT_DROP_DELAY = 2;
		FrameTimer game_softDropTimer;


		GameField game_field;

		// Offset from (0, 0) for drawing game field.
		sf::Vector2f game_blocksDrawingOffset;
		
		// Offset from (0, 0) for drawing next piece.
		sf::Vector2f game_nextPieceDrawingOffset;
		sf::Vector2f game_nextPieceDrawingCenter;
		
		void game_update();
		void game_updateFigureControls();
		void game_updateFigureDrop();
		void game_updateDas();
		void game_moveFigure(Direction direction);
		void game_updateCounters();
		void game_updateStatisticsCounters();
		void game_updatePieceCounters();

		void game_drawCounters();
		void game_drawStaticticsCounters();
		void game_drawPieceCounters();
		void game_drawBlocks();
		void game_drawField();
		void game_drawCurrentPiece();
		void game_drawStaticticsBlocks();
		void game_drawNextPiece();
		void game_drawFigure(const Tetrimino::Matrix::Array& matrix, const sf::Vector2f& offset);
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