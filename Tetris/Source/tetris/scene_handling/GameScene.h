#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include "tetris/scene_handling/SceneHandlerBase.h"
#include "tetris/scene_handling/Tetrimino.h"
#include "tetris/wrapper/FrameTimer.h"
#include "tetris/wrapper/CounterUI.h"
#include "tetris/scene_handling/GameField.h"




namespace tetris::scene_handling {
	class GameScene : public SceneHandlerBase {
	public:
		GameScene();

		void update();
		void draw(sf::RenderWindow& window);

		void reset();

		void applySettings(int32_t level, int32_t music);
	private:
		sf::Sprite background;

		sf::Sprite droughtIndicator;
		sf::Sprite blocks;


		sf::Sound pauseSound;
		sf::Sound gameOverSound;
		sf::Sound newLevelSound;

		sf::Sound lineClearSound;
		sf::Sound tetrisClearSound;

		sf::Sound tetriminoMoveSound;
		sf::Sound tetriminoRotateSound;
		sf::Sound tetriminoLandSound;

		void loadResources();
		void loadSounds();

		void initializeGraphics();
		void initializeCounters();
		void initializePieceCounters();
		void initializeStatisticsCounters();
		void initializeStatisticsPieceData();

		GameField field;

		void updatePieceControls();
		void dropPieceDown(bool isSoftDrop);
		void movePiece(Direction direction);


		// Delay for 1st tetrimino moves.
		static const int32_t DAS_DELAY_LONG = 16;

		// Delay for 2nd and other tetrimino moves.
		static const int32_t DAS_DELAY_SHORT = 6;

		enum class DasState {
			// Not applied DAS.
			NONE = 0,

			// Move piece for the first time with long delay.
			LONG_DELAYED_MOVE = 1,

			// Move piece for the second (and other) time with short delay.
			SHORT_DELAYED_MOVE = 2
		};
		DasState dasState;
		Direction previousMoveDirection;
		Direction currentMoveDirection;

		wrapper::FrameTimer dasTimer;

		void updateDas();
		void updateDas_controls();
		void updateDas_movePiece();




		
		bool isFirstSpawn;
		static const int32_t FIRST_SPAWN_DROP_DELAY = 120;
		wrapper::FrameTimer firstSpawnTimer;

		bool allowSoftDrop;
		static const int32_t SOFT_DROP_DELAY = 2;
		wrapper::FrameTimer softDropTimer;
		
		bool hasPieceLanded;
		static const int32_t RESPAWN_DELAY = 10;
		wrapper::FrameTimer respawnTimer;

		int32_t getDropFrames(int32_t level);
		wrapper::FrameTimer dropTimer;


		wrapper::CounterUI tetrisesCounter;
		wrapper::CounterUI burnCounter;
		wrapper::CounterUI tetrisRateCounter;
		wrapper::CounterUI linesCounter;
		wrapper::CounterUI topScoreCounter;
		wrapper::CounterUI currentScoreCounter;
		wrapper::CounterUI levelCounter;
		wrapper::CounterUI droughtCounter;


		std::vector<wrapper::CounterUI> pieceCounters;
		std::vector<std::tuple<const Tetrimino::Matrix::Array*, sf::Vector2f>> staticticsBlocksData;
		
		sf::Vector2f blocksDrawingOffset;

		void updateCounters();
		void updateStatisticsCounters();
		void updatePieceCounters();





		void drawCounters(sf::RenderWindow& window);
		void drawStaticticsCounters(sf::RenderWindow& window);
		void drawPieceCounters(sf::RenderWindow& window);

		void drawBlocks(sf::RenderWindow& window);
		void drawField(sf::RenderWindow& window);
		void drawCurrentPiece(sf::RenderWindow& window);
		void drawStaticticsBlocks(sf::RenderWindow& window);
		void drawNextPiece(sf::RenderWindow& window);

		void drawPiece(sf::RenderWindow&				window,
					   const Tetrimino::Matrix::Array&	matrix,
					   const sf::Vector2f&				offset);

		void drawSingleBlock(sf::RenderWindow&		window,
							 const sf::Vector2f&	position,
							 Tetrimino::Type		type);
		

		
		

		// Actual block size (in pixels).
		static const int32_t BLOCK_SIZE = 19;

		// Distance between blocks (in pixels).
		static const int32_t BLOCK_GAP = 2;

		// Block size needed for calculating screen positions.
		static const int32_t BLOCK_SIZE_WITH_GAP = GameScene::BLOCK_SIZE + GameScene::BLOCK_GAP;
	};
}