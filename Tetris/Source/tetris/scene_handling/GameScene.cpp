#include "tetris/scene_handling/GameScene.h"

#include <array>
#include <tuple>

#include "tetris/Record.h"
#include "tetris/scene_handling/ResourceManager.h"


using namespace tetris::scene_handling;

GameScene::GameScene() {
	this->loadResources();

	this->blocksDrawingOffset = sf::Vector2f(321, 70);

	this->reset();
}



void GameScene::loadResources() {
	this->background.setTexture(
		ResourceManager::getTexture(GAME_SCREEN_BMP)
	);

	this->droughtIndicator.setTexture(
		ResourceManager::getTexture(DROUGHT_INDICATOR_BMP)
	);
	this->blocks.setTexture(
		ResourceManager::getTexture(BLOCKS_BMP)
	);

	this->loadSounds();
}



void GameScene::loadSounds() {
	std::vector<std::pair<sf::Sound*, int32_t>> sounds = {
		{ &this->pauseSound, PAUSE_WAV },
		{ &this->gameOverSound, GAME_OVER_WAV },
		{ &this->newLevelSound, NEW_LEVEL_WAV },

		{ &this->tetriminoMoveSound, TETRIMINO_MOVE_WAV },
		{ &this->tetriminoRotateSound, TETRIMINO_ROTATE_WAV },
		{ &this->tetriminoLandSound, TETRIMINO_LAND_WAV },

		{ &this->lineClearSound, LINE_CLEARED_WAV },
		{ &this->tetrisClearSound, TETRIS_CLEARED_WAV }
	};

	for (auto pair : sounds) {
		auto [sound, id] = pair;
		sound->setBuffer(ResourceManager::getSoundBuffer(id));
	}
}



void GameScene::initializeCounters() {
	auto& font = ResourceManager::getFont(GAME_FONT);
	static const int32_t FONT_SIZE = 24;

	wrapper::CounterUI::setDefaultDisplayOptions(
		font, FONT_SIZE,
		sf::Color::Red, sf::Text::Style::Regular
	);
	this->initializePieceCounters();

	wrapper::CounterUI::setDefaultDisplayOptions(
		font, FONT_SIZE,
		sf::Color::White, sf::Text::Style::Regular
	);
	this->initializeStatisticsCounters();
}



void GameScene::initializePieceCounters() {
	std::vector<sf::Vector2f> positions = {
		{ 166,  73 },
		{ 166, 118 },
		{ 166, 164 },
		{ 166, 211 },
		{ 166, 258 },
		{ 166, 305 },
		{ 166, 349 }
	};
	this->pieceCounters = std::vector<wrapper::CounterUI>(GameField::DIFFERENT_PIECES);
	for (uint32_t i = 0; i < GameField::DIFFERENT_PIECES; ++i) {
		this->pieceCounters[i].applyDefaultDisplayOptions();
		this->pieceCounters[i].setMaximalValue(GameField::MAX_PIECE_AMOUNT);
		this->pieceCounters[i].setPosition(positions[i]);
	}
}



void GameScene::initializeStatisticsCounters() {
	std::vector<std::tuple<wrapper::CounterUI*, int32_t, sf::Vector2f>> data = {
		{ &this->tetrisesCounter, GameField::MAX_TETRISES, { 177, 419 } },
		{ &this->burnCounter, GameField::MAX_BURN, { 177, 464 } },
		{ &this->tetrisRateCounter, GameField::MAX_TETRIS_RATE, { 177, 510 } },
		{ &this->linesCounter, GameField::MAX_LINES, { 482, 43 } },
		{ &this->topScoreCounter, Record::MAX_SCORE, { 608, 86 } },
		{ &this->currentScoreCounter, Record::MAX_SCORE, { 608, 166 } },
		{ &this->levelCounter, GameField::MAX_LEVEL, { 642, 431 } },
		{ &this->droughtCounter, GameField::MAX_DROUGHT, { 642, 519 } },
	};
	for (uint32_t i = 0; i < data.size(); ++i) {
		wrapper::CounterUI* counter = std::get<0>(data[i]);
		counter->applyDefaultDisplayOptions();
		counter->setMaximalValue(std::get<1>(data[i]));
		counter->setPosition(std::get<2>(data[i]));
	}
}



void GameScene::initializeStatisticsPieceData() {
	this->staticticsBlocksData = {
		{ this->field.getPieceMatrix(Piece::T), { 45,  25 } },
		{ this->field.getPieceMatrix(Piece::J), { 45,  72 } },
		{ this->field.getPieceMatrix(Piece::Z), { 45, 119 } },
		{ this->field.getPieceMatrix(Piece::O), { 54, 166 } },
		{ this->field.getPieceMatrix(Piece::S), { 45, 213 } },
		{ this->field.getPieceMatrix(Piece::L), { 45, 260 } },
		{ this->field.getPieceMatrix(Piece::I), { 53, 307 } },
	};
}



int32_t GameScene::getDropFrames(int32_t level) {
	static std::vector<int32_t> frames = {
		48, // Level 0.
		43,
		38,
		33,
		28,
		23, // Level 5.
		18,
		13,
		8,
		6,
		5,  // Level 10.
		5,
		5,
		4,  // Level 13.
		4,
		4,
		3,  // Level 16.
		3,
		3   // Level 18.
	};

	if (level >= 0 && level <= 18) {
		return frames[level];
	}
	else if (level >= 19 && level <= 28) {
		return 2;
	}
	else {
		return 1;
	}
}



void GameScene::update() {
	this->nextScene = Scene::NONE;

	if (Keyboard::isKeyPushed(GamePadKey::START)) {
		this->pauseSound.play();
		this->nextScene = Scene::PAUSE_SCREEN;
		return;
	}

	// Updates delay on first piece spawn.
	if (this->isFirstSpawn) {
		this->firstSpawnTimer.update();
		if (this->firstSpawnTimer.isTriggered() || Keyboard::isKeyHeld(GamePadKey::DOWN)) {
			this->isFirstSpawn = false;
		}
	}


	if (this->field.doesActivePieceExist()) {
		this->updatePieceControls();

		this->field.checkFullLines();
		const std::vector<int32_t>& linesToClear = this->field.getLinesToClear();
		if (linesToClear.size() != 0) {

			if (linesToClear.size() == 4) {
				this->tetrisClearSound.play();
			}
			else {
				this->lineClearSound.play();
			}
			this->field.clearLines();
		}
	}


	// Spawns new piece
	if (!this->field.doesActivePieceExist()) {
		this->respawnTimer.update();
		if (this->respawnTimer.isTriggered()) {
			bool spawned = this->field.spawnNewPiece();
			if (!spawned) {
				this->gameOverSound.play();
				this->nextScene = Scene::SPLASH_SCREEN; // TODO: change scene to correct one.
				return;
			}
		}
	}
}



void GameScene::updatePieceControls() {
	if (Keyboard::isKeyPushed(GamePadKey::B)) {
		if (this->field.rotatePiece(Rotation::COUNTERCLOCKWISE)) {
			this->tetriminoRotateSound.play();
		}
	}
	else if (Keyboard::isKeyPushed(GamePadKey::A)) {
		if (this->field.rotatePiece(Rotation::CLOCKWISE)) {
			this->tetriminoRotateSound.play();
		}
	}



	bool down = Keyboard::isKeyHeld(GamePadKey::DOWN) && this->allowSoftDrop;
	bool left = Keyboard::isKeyHeld(GamePadKey::LEFT);
	bool right = Keyboard::isKeyHeld(GamePadKey::RIGHT);
	if (down && (left || right)) {
		return;
	}
	else if (down) {
		this->softDropTimer.update();
		if (this->softDropTimer.isTriggered()) {
			this->dropPieceDown(true);
		}
	}
	else {
		this->updateDas();
	}

	if (!down && !this->isFirstSpawn) {
		this->dropTimer.update();
		if (this->dropTimer.isTriggered()) {
			this->dropPieceDown(false);
		}
	}

	this->updateCounters();
}



void GameScene::dropPieceDown(bool isSoftDrop) {
	this->hasPieceLanded = this->field.dropPieceDown(isSoftDrop);
	if (this->hasPieceLanded) {
		this->tetriminoLandSound.play();
	}
}



#pragma region DAS


void GameScene::updateDas() {
	this->updateDas_controls();
	this->updateDas_movePiece();
}



void GameScene::updateDas_controls() {
	this->previousMoveDirection = this->currentMoveDirection;
	this->currentMoveDirection = Direction::NONE;
	if (Keyboard::isKeyHeld(GamePadKey::LEFT)) {
		this->currentMoveDirection = Direction::LEFT;
	}
	else if (Keyboard::isKeyHeld(GamePadKey::RIGHT)) {
		this->currentMoveDirection = Direction::RIGHT;
	}
}



void GameScene::updateDas_movePiece() {
	bool currentlyPressed = this->currentMoveDirection != Direction::NONE;
	bool previouslyPressed = this->previousMoveDirection != Direction::NONE;
	bool directionsDiffer = this->currentMoveDirection != this->previousMoveDirection;
	if (!currentlyPressed) {
		this->dasState = DasState::NONE;
		return;
	}
	else if (previouslyPressed && directionsDiffer) {
		this->dasState = DasState::NONE;
	}


	if (this->dasState == DasState::NONE) {
		this->movePiece(this->currentMoveDirection);
		this->dasState = DasState::LONG_DELAYED_MOVE;
		this->dasTimer.setTimingFrames(this->DAS_DELAY_LONG);
		this->dasTimer.reset();
	}

	else if (this->dasState == DasState::LONG_DELAYED_MOVE) {
		this->dasTimer.update();
		if (this->dasTimer.isTriggered()) {
			this->movePiece(this->currentMoveDirection);
			this->dasState = DasState::SHORT_DELAYED_MOVE;
			this->dasTimer.setTimingFrames(this->DAS_DELAY_SHORT);
			this->dasTimer.reset();
		}
	}

	else if (this->dasState == DasState::SHORT_DELAYED_MOVE) {
		this->dasTimer.update();
		if (this->dasTimer.isTriggered()) {
			this->movePiece(this->currentMoveDirection);
		}
	}
}


#pragma /* DAS */ endregion



void GameScene::movePiece(Direction direction) {
	if (this->field.movePiece(direction)) {
		this->tetriminoMoveSound.play();
	}
}




void GameScene::updateCounters() {
	this->updateStatisticsCounters();
	this->updatePieceCounters();
}



void GameScene::updateStatisticsCounters() {
	this->tetrisesCounter.setNumericValue(this->field.getTetrises());
	this->burnCounter.setNumericValue(this->field.getBurn());
	this->tetrisRateCounter.setNumericValue(this->field.getTetrisRate());
	this->linesCounter.setNumericValue(this->field.getLines());
	this->droughtCounter.setNumericValue(this->field.getDrought());
	this->currentScoreCounter.setNumericValue(this->field.getScore());
	this->topScoreCounter.setNumericValue(this->field.getTopScore());

	int32_t level = this->field.getLevel();
	if (level != this->levelCounter.getNumericValue()) {
		this->dropTimer.setTimingFrames(this->getDropFrames(level));
		this->newLevelSound.play();
	}
	this->levelCounter.setNumericValue(level);
}



void GameScene::updatePieceCounters() {
	for (uint32_t piece = 0; piece < GameField::DIFFERENT_PIECES; ++piece) {
		this->pieceCounters[piece].setNumericValue(
			this->field.getPieceAmount(Piece(piece))
		);
	}
}



void GameScene::drawCounters(sf::RenderWindow& window) {
	this->drawStaticticsCounters(window);
	this->drawPieceCounters(window);
}



void GameScene::drawStaticticsCounters(sf::RenderWindow& window) {
	this->tetrisesCounter.drawOn(window);
	this->burnCounter.drawOn(window);
	this->tetrisRateCounter.drawOn(window);

	this->linesCounter.drawOn(window);

	this->topScoreCounter.drawOn(window);
	this->currentScoreCounter.drawOn(window);

	this->levelCounter.drawOn(window);
	this->droughtCounter.drawOn(window);
}



void GameScene::drawPieceCounters(sf::RenderWindow& window) {
	for (auto& counter : this->pieceCounters) {
		counter.drawOn(window);
	}
}



void GameScene::drawBlocks(sf::RenderWindow& window) {
	this->drawField(window);
	if (this->field.doesActivePieceExist()) {
		this->drawCurrentPiece(window);
	}
	this->drawNextPiece(window);
	this->drawStaticticsBlocks(window);
}



void GameScene::drawField(sf::RenderWindow& window) {
	const auto& blocks = this->field.getBlocks();
	for (uint32_t y = 2; y < blocks.size(); ++y) {
		for (uint32_t x = 0; x < blocks[y].size(); ++x) {
			if (blocks[y][x] != Tetrimino::Type::E) {
				auto drawPos = sf::Vector2f(
					this->blocksDrawingOffset.x + float(x * this->BLOCK_SIZE_WITH_GAP),
					this->blocksDrawingOffset.y + float(y * this->BLOCK_SIZE_WITH_GAP)
				);

				this->drawSingleBlock(window, drawPos, blocks[y][x]);
			}
		}
	}
}


/*
void GameScene::drawFieldCovered() {
	this->game_animationTimer.update();
	if (this->game_animationTimer.isTriggered()) {
		++this->game_blocksToCover;
		if (this->game_blocksToCover == MAX_BLOCKS_TO_COVER) {
			this->game_isPerformingAnimation = false;
		}
	}

	this->game_blocks.setColor(sf::Color::Black);

	const std::vector<int32_t>& linesToClear = this->game_field.getLinesToClear();
	for (auto y : linesToClear) {
		int32_t coverLeft = 0;
		for (int32_t x = 4; x >= 0; --x) {
			if (coverLeft == this->game_blocksToCover) {
				break;
			}
			++coverLeft;
			auto drawPos = sf::Vector2f(
				this->game_blocksDrawingOffset.x + float(x * this->BLOCK_SIZE_WITH_GAP),
				this->game_blocksDrawingOffset.y + float(y * this->BLOCK_SIZE_WITH_GAP)
			);
			this->game_drawSingleBlock(drawPos, Tetrimino::Type::W);
		}
		int32_t coverRight = 0;
		for (int32_t x = 5; x < this->game_field.FIELD_X_SIZE; ++x) {
			if (coverRight == this->game_blocksToCover) {
				break;
			}
			++coverRight;
			auto drawPos = sf::Vector2f(
				this->game_blocksDrawingOffset.x + float(x * this->BLOCK_SIZE_WITH_GAP),
				this->game_blocksDrawingOffset.y + float(y * this->BLOCK_SIZE_WITH_GAP)
			);
			this->game_drawSingleBlock(drawPos, Tetrimino::Type::W);
		}
	}
	this->game_blocks.setColor(sf::Color::White);
}
*/


void GameScene::drawCurrentPiece(sf::RenderWindow& window) {
	auto& pieceMatrix = this->field.getCurrentPieceMatrix();
	auto drawStartPos = this->field.getCurrentPieceCenter() - Tetrimino::Matrix::CENTER;
	for (int32_t y = 0; y < Tetrimino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetrimino::Matrix::SIZE; ++x) {
			sf::Vector2i currentPos = sf::Vector2i(
				drawStartPos.x + x,
				drawStartPos.y + y
			);

			bool xOut = currentPos.x < 0 || currentPos.x > GameField::FIELD_X_SIZE;
			bool yOut = currentPos.y < 0 || currentPos.y > GameField::FIELD_Y_SIZE;
			if (!(xOut || yOut)) {
				bool isDrawPositionSuitable = currentPos.y >= GameField::UPPER_LINES_TO_CLEAR;
				bool isBlockVisible = pieceMatrix[y][x] != Tetrimino::Type::E;
				if (isDrawPositionSuitable && isBlockVisible) {
					auto drawPos = sf::Vector2f(
						this->blocksDrawingOffset.x + float(currentPos.x * this->BLOCK_SIZE_WITH_GAP),
						this->blocksDrawingOffset.y + float(currentPos.y * this->BLOCK_SIZE_WITH_GAP)
					);

					this->drawSingleBlock(window, drawPos, pieceMatrix[y][x]);
				}
			}
		}
	}
}



void GameScene::drawStaticticsBlocks(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < this->staticticsBlocksData.size(); ++i) {
		const Tetrimino::Matrix::Array* matrix = std::get<0>(this->staticticsBlocksData[i]);
		for (uint32_t y = 0; y < matrix->size(); ++y) {
			for (uint32_t x = 0; x < matrix->size(); ++x) {
				if ((*matrix)[y][x] != Tetrimino::Type::E) {
					auto& pos = std::get<1>(this->staticticsBlocksData[i]);
					auto drawPos = sf::Vector2f(
						pos.x + x * this->BLOCK_SIZE_WITH_GAP,
						pos.y + y * this->BLOCK_SIZE_WITH_GAP
					);
					this->drawSingleBlock(window, drawPos, (*matrix)[y][x]);
				}
			}
		}
	}
}



void GameScene::drawNextPiece(sf::RenderWindow& window) {
	static std::vector<sf::Vector2f> offsets = {
		{ 600, 261 }, // T.
		{ 600, 261 }, // J.
		{ 600, 261 }, // Z.
		{ 610, 261 }, // O.
		{ 600, 261 }, // S.
		{ 600, 261 }, // L.
		{ 610, 272 }  // I.
	};
	auto& pieceMatrix = this->field.getNextPieceMatrix();
	auto type = int32_t(this->field.getNextPieceType());
	for (int32_t y = 0; y < Tetrimino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetrimino::Matrix::SIZE; ++x) {
			if (pieceMatrix[y][x] != Tetrimino::Type::E) {
				auto drawPos = sf::Vector2f(
					offsets[type].x + x * this->BLOCK_SIZE_WITH_GAP,
					offsets[type].y + y * this->BLOCK_SIZE_WITH_GAP
				);
				this->drawSingleBlock(window, drawPos, pieceMatrix[y][x]);
			}
		}
	}
}



void GameScene::drawPiece(sf::RenderWindow& window, const Tetrimino::Matrix::Array& matrix, const sf::Vector2f& offset) {
	for (int32_t y = 0; y < Tetrimino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetrimino::Matrix::SIZE; ++x) {
			if (matrix[y][x] != Tetrimino::Type::E) {
				auto drawPos = sf::Vector2f(
					offset.x + x * this->BLOCK_SIZE_WITH_GAP,
					offset.y + y * this->BLOCK_SIZE_WITH_GAP
				);
				this->drawSingleBlock(window, drawPos, matrix[y][x]);
			}
		}
	}
}



void GameScene::drawSingleBlock(sf::RenderWindow&	window,
								const sf::Vector2f& position,
								Tetrimino::Type		type) {
	this->blocks.setPosition(position);

	this->blocks.setTextureRect(sf::IntRect(
		this->BLOCK_SIZE * (this->field.getLevel() % 10),
		this->BLOCK_SIZE * int32_t(type),
		this->BLOCK_SIZE,
		this->BLOCK_SIZE
	));

	window.draw(this->blocks);
}



void GameScene::draw(sf::RenderWindow& window) {
	window.clear(sf::Color::Black);

	window.draw(this->background);
	window.draw(this->droughtIndicator);

	this->drawCounters(window);
	this->drawBlocks(window);

	window.display();
}



void GameScene::reset() {
	this->isFirstSpawn = true;
	this->firstSpawnTimer.setTimingFrames(this->FIRST_SPAWN_DROP_DELAY);
	this->firstSpawnTimer.reset();

	this->allowSoftDrop = true;
	this->softDropTimer.setTimingFrames(this->SOFT_DROP_DELAY);
	this->softDropTimer.reset();
	
	this->hasPieceLanded = false;
	this->respawnTimer.setTimingFrames(RESPAWN_DELAY);
	this->respawnTimer.reset();
}



void GameScene::applySettings(int32_t level, int32_t music) {
	
}
