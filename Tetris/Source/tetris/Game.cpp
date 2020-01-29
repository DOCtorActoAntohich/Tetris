#include "tetris/Game.h"

#include <iostream>
#include <tuple>


#include "resource.h"

#include "tetris/helper/Helper.h"

using namespace tetris;



Game::Game() {
	changeScene(scene_handling::Scene::SPLASH_SCREEN);

	this->splashScreen_textBlinkTimer.setTimingFrames(this->SPLASH_TEXT_BLINK_TIMING);

	this->menu_highlightersBlinkTimer.setTimingFrames(this->MENU_HIGHLIGHTERS_BLINK_TIMING);

	this->menu_startLevel = 0;
	this->menu_musicType = 0;

	this->game_softDropTimer.setTimingFrames(this->SOFT_DROP_DELAY);
	this->game_currentMoveDirection = Direction::NONE;

	this->shouldCallDrawer = true;
}



void Game::run() {
	this->loadContent();

	this->runMainLoop();

	this->unloadContent();
}


#pragma region Resource Management
	

void Game::initializeWindow() {
	this->window.create(
		sf::VideoMode(800, 600), "Tetris",
		sf::Style::Titlebar | sf::Style::Close //TODO: | sf::Style::Fullscreen
	);
	this->window.setFramerateLimit(this->FPS);
}



#pragma region Loading Resources


void Game::loadFont(int32_t id, std::vector<byte>& bytes, sf::Font& font) {
	bytes = helper::loadEmbeddedResource(id);
	font.loadFromMemory(&bytes[0], bytes.size());
}



void Game::openMusic(int32_t id, std::vector<byte>& musicBytes, sf::Music& music) {
	musicBytes = helper::loadEmbeddedResource(id);
	music.openFromMemory(&musicBytes[0], musicBytes.size());
}


#pragma /* Loading Resources */ endregion


#pragma region Other Data


void Game::pressEnter_text_initialize() {
	this->pressEnter_text.setFont(this->font);
	this->pressEnter_text.setString("PRESS ENTER");
	this->pressEnter_text.setCharacterSize(this->FONT_SIZE);
	this->pressEnter_text.setFillColor(sf::Color::White);
	this->pressEnter_text.setStyle(sf::Text::Regular);
	this->pressEnter_text.setPosition(256, 326);
	this->pressEnter_text_isVisible = true;
}



void Game::predefinedDigitPositions_initialize() {
	// Looks like crapcode :(.
	this->predefinedLevelDigitPositions = {
		sf::Vector2f(140, 152),
		sf::Vector2f(188, 152),
		sf::Vector2f(236, 152),
		sf::Vector2f(284, 152),
		sf::Vector2f(332, 152),
		sf::Vector2f(140, 201),
		sf::Vector2f(188, 201),
		sf::Vector2f(236, 201),
		sf::Vector2f(284, 201),
		sf::Vector2f(332, 201)
	};
}



void Game::menu_levelHighlighter_initialize() {
	predefinedDigitPositions_initialize();
	this->menu_levelHighlighter.setSize(sf::Vector2f(42, 45));
	this->menu_levelHighlighter.setFillColor(sf::Color::Yellow);
	this->menu_isLevelHighlighterVisible = true;
	this->menu_levelHighlighter_update();
}



void Game::menu_levelHighlighter_update() {
	this->menu_levelHighlighter.setPosition(
		predefinedLevelDigitPositions[this->menu_startLevel]
	);
}



void Game::menu_levelDigits_initialize() {
	predefinedDigitPositions_initialize();
	sf::Vector2f offset(10, 12); // To center positions.
	for (int32_t digit = 0; digit < this->DIGITS; ++digit) {
		this->menu_levelDigits[digit].setFont(this->font);
		this->menu_levelDigits[digit].setString(std::to_string(digit));
		this->menu_levelDigits[digit].setCharacterSize(this->FONT_SIZE);
		this->menu_levelDigits[digit].setFillColor(sf::Color(216, 40, 0));
		this->menu_levelDigits[digit].setStyle(sf::Text::Regular);
		this->menu_levelDigits[digit].setPosition(
			this->predefinedLevelDigitPositions[digit] + offset
		);
	}
}



void Game::predefinedMusicHighlightersPositions_initialize() {
	this->predefinedMusicHighlightersPositionsX = {
		435, 641
	};
	this->predefinedMusicHighlightersPositionsY = {
		164, 212, 260, 308
	};
}



void Game::menu_musicHighlighters_initialize() {
	// Crapcode again :(.
	this->predefinedMusicHighlightersPositions_initialize();
	std::string strings[this->MUSIC_HIGHLIGHTERS] = { ">", "<" };
	for (int32_t i = 0; i < this->MUSIC_HIGHLIGHTERS; ++i) {
		this->menu_musicHighlighters[i].setFont(this->font);
		this->menu_musicHighlighters[i].setString(strings[i]);
		this->menu_musicHighlighters[i].setCharacterSize(this->FONT_SIZE);
		this->menu_musicHighlighters[i].setFillColor(sf::Color::Yellow);
		this->menu_musicHighlighters[i].setStyle(sf::Text::Regular);
	}
	this->menu_areMusicHighlightersVisible = true;
	this->menu_musicHighlighters_update();
}



void Game::menu_musicHighlighters_update() {
	for (int i = 0; i < this->MUSIC_HIGHLIGHTERS; ++i) {
		this->menu_musicHighlighters[i].setPosition(
			this->predefinedMusicHighlightersPositionsX[i],
			this->predefinedMusicHighlightersPositionsY[this->menu_musicType]
		);
	}
}



void Game::initializeCounters() {
	wrapper::CounterUI::setDefaultDisplayOptions(
		this->font, this->FONT_SIZE,
		sf::Color::Red, sf::Text::Style::Regular
	);
	this->initializePieceCounters();

	wrapper::CounterUI::setDefaultDisplayOptions(
		this->font, this->FONT_SIZE,
		sf::Color::White, sf::Text::Style::Regular
	);
	this->initializeStaticticsCounters();
}



void Game::initializePieceCounters() {
	std::vector<sf::Vector2f> positions = {
		{ 166,  73 },
		{ 166, 118 },
		{ 166, 164 },
		{ 166, 211 },
		{ 166, 258 },
		{ 166, 305 },
		{ 166, 349 }
	};
	this->game_pieceCounters = std::vector<wrapper::CounterUI>(GameField::DIFFERENT_PIECES);
	for (uint32_t i = 0; i < GameField::DIFFERENT_PIECES; ++i) {
		this->game_pieceCounters[i].applyDefaultDisplayOptions();
		this->game_pieceCounters[i].setMaximalValue(GameField::MAX_PIECE_AMOUNT);
		this->game_pieceCounters[i].setPosition(positions[i]);
	}
}



void Game::initializeStaticticsCounters() {
	std::vector<std::tuple<wrapper::CounterUI*, int32_t, sf::Vector2f>> data = {
		{ &this->game_tetrisesCounter, GameField::MAX_TETRISES, { 177, 419 } },
		{ &this->game_burnCounter, GameField::MAX_BURN, { 177, 464 } },
		{ &this->game_tetrisRateCounter, GameField::MAX_TETRIS_RATE, { 177, 510 } },
		{ &this->game_linesCounter, GameField::MAX_LINES, { 482, 43 } },
		{ &this->game_topScoreCounter, Record::MAX_SCORE, { 608, 86 } },
		{ &this->game_currentScoreCounter, Record::MAX_SCORE, { 608, 166 } },
		{ &this->game_levelCounter, GameField::MAX_LEVEL, { 642, 431 } },
		{ &this->game_droughtCounter, GameField::MAX_DROUGHT, { 642, 519 } },
	};
	for (uint32_t i = 0; i < data.size(); ++i) {
		wrapper::CounterUI* counter = std::get<0>(data[i]);
		counter->applyDefaultDisplayOptions();
		counter->setMaximalValue(std::get<1>(data[i]));
		counter->setPosition(std::get<2>(data[i]));
	}
}



void Game::initializeStatisticsPieceData() {
	this->game_staticticsBlocksData = {
		{ this->game_field.getPieceMatrix(Piece::T), { 45,  25 } },
		{ this->game_field.getPieceMatrix(Piece::J), { 45,  72 } },
		{ this->game_field.getPieceMatrix(Piece::Z), { 45, 119 } },
		{ this->game_field.getPieceMatrix(Piece::O), { 54, 166 } },
		{ this->game_field.getPieceMatrix(Piece::S), { 45, 213 } },
		{ this->game_field.getPieceMatrix(Piece::L), { 45, 260 } },
		{ this->game_field.getPieceMatrix(Piece::I), { 53, 307 } },
	};
}


#pragma /* Other Data */ endregion



void Game::loadContent() {
	this->framesPerGridcell = {
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
		3
	};
	for (int32_t i = 19; i <= 28; ++i) {
		this->framesPerGridcell.push_back(2);
	}
	this->framesPerGridcell.push_back(1);

	loadFont(GAME_FONT, this->fontBytes, this->font);


	this->splash_background.loadFromResource(SPLASH_SCREEN_BMP);
	this->pressEnter_text_initialize();

	this->controls_background.loadFromResource(CONTROLS_SCREEN_BMP);

	this->menu_background.loadFromResource(MENU_SCREEN_BMP);
	this->menu_levelHighlighter_initialize();
	this->menu_levelDigits_initialize();
	this->menu_musicHighlighters_initialize();

	this->game_background.loadFromResource(GAME_SCREEN_BMP);
	this->game_blocksDrawingOffset = sf::Vector2f(321, 70);

	this->pause_background.loadFromResource(PAUSE_SCREEN_BMP);


	this->game_droughtIndicator.loadFromResource(DROUGHT_INDICATOR_BMP);
	this->game_droughtIndicator.setPosition({ 624, 491 });

	this->game_blocks.loadFromResource(BLOCKS_BMP);


	this->menuClickMajor_sound.loadFromResource(MENU_CLICK_MAJOR_WAV);
	this->menuClickMinor_sound.loadFromResource(MENU_CLICK_MINOR_WAV);
	this->pause_sound.loadFromResource(PAUSE_WAV);

	this->tetriminoMove_sound.loadFromResource(TETRIMINO_MOVE_WAV);
	this->tetriminoRotate_sound.loadFromResource(TETRIMINO_ROTATE_WAV);
	this->tetriminoLand_sound.loadFromResource(TETRIMINO_LAND_WAV);
	
	this->lineCleared_sound.loadFromResource(LINE_CLEARED_WAV);
	this->tetrisCleared_sound.loadFromResource(TETRIS_CLEARED_WAV);
	this->newLevel_sound.loadFromResource(NEW_LEVEL_WAV);

	this->gameOver_sound.loadFromResource(GAME_OVER_WAV);

	this->initializeCounters();
	this->initializeStatisticsPieceData();

	this->initializeWindow();
}



void Game::unloadContent() {

}

#pragma /* Resource Management */ endregion


#pragma region Main Loop


void Game::exit() {
	if (this->window.isOpen()) {
		this->window.close();
	}
}



void Game::runMainLoop() {
	while (this->window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->exit();
			}
		}

		if (this->window.hasFocus()) {
			this->keyboard.update();

			(this->*update)();
			if (this->shouldCallDrawer) {
				(this->*draw)();
			}
			this->shouldCallDrawer = true;
		}
	}
}


void Game::changeScene(scene_handling::Scene scene) {
	auto [update, draw] = this->chooseUpdaters(scene);
	this->update = update;
	this->draw = draw;

	this->scene = scene;

	this->shouldCallDrawer = false;
}


std::pair<void(Game::*)(), void(Game::*)()> Game::chooseUpdaters(scene_handling::Scene scene) {
	using voidfunc = void(Game::*)();
	using Scene = scene_handling::Scene;
	static std::map<Scene, std::pair<voidfunc, voidfunc>> handlers = {
		{ Scene::SPLASH_SCREEN,
		{ &Game::splashScreen_update, &Game::splashScreen_draw }},

		{ Scene::CONTROLS_SCREEN,
		{ &Game::controlsScreen_update, &Game::controlsScreen_draw }},

		{ Scene::MENU,
		{ &Game::menu_update, &Game::menu_draw }},

		{ Scene::GAME,
		{ &Game::game_update, &Game::game_draw }},

		{ Scene::PAUSE_SCREEN,
		{ &Game::pauseScreen_update, &Game::pauseScreen_draw }},
	};

	return handlers[scene];
}

#pragma region Scene Handlers


#pragma region Splash Screen


void Game::splashScreen_update() {
	this->splashScreen_textBlinkTimer.update();

	if (this->splashScreen_textBlinkTimer.isTriggered()) {
		this->pressEnter_text_isVisible = !this->pressEnter_text_isVisible;
	}

	if (keyboard.isKeyPushed(GamePadKey::START)) {
		this->menuClickMajor_sound.play();
		this->changeScene(scene_handling::Scene::CONTROLS_SCREEN);
	}
	else if (keyboard.isKeyPushed(GamePadKey::EXIT)) {
		this->window.close();
	}
}


void Game::splashScreen_draw() {
	this->window.clear(sf::Color::Black);

	this->splash_background.drawOn(this->window);
	if (this->pressEnter_text_isVisible) {
		this->window.draw(this->pressEnter_text);
	}

	this->window.display();
}


#pragma /* Splash Screen */ endregion


#pragma region Controls Screen

void Game::controlsScreen_update() {
	if (keyboard.isKeyPushed(GamePadKey::START)) {
		this->menu_highlightersBlinkTimer.reset();
		this->menuClickMajor_sound.play();
		this->changeScene(scene_handling::Scene::MENU);
	}
	else if (keyboard.isKeyPushed(GamePadKey::B)) {
		this->splashScreen_textBlinkTimer.reset();
		this->menuClickMajor_sound.play();
		this->changeScene(scene_handling::Scene::SPLASH_SCREEN);
	}
	else if (keyboard.isKeyPushed(GamePadKey::EXIT)) {
		this->window.close();
	}
}


void Game::controlsScreen_draw() {
	this->window.clear(sf::Color::Black);

	this->controls_background.drawOn(this->window);

	this->window.display();
}

#pragma /* Controls Screen */ endregion


#pragma region Menu


void Game::menu_update() {
	this->menu_highlightersBlinkTimer.update();

	if (keyboard.isKeyPushed(GamePadKey::START)) {
		this->menu_prepareForGame();
		this->menuClickMajor_sound.play();
		this->changeScene(scene_handling::Scene::GAME);
	}
	else if (keyboard.isKeyPushed(GamePadKey::B)) {
		this->menuClickMajor_sound.play();
		this->changeScene(scene_handling::Scene::CONTROLS_SCREEN);
	}
	else if (keyboard.isKeyPushed(GamePadKey::EXIT)) {
		this->window.close();
	}

	this->menu_updateLevelSelection();
	this->menu_updateMusicSelection();
}



void Game::menu_updateLevelSelection() {
	if (this->menu_highlightersBlinkTimer.isTriggered()) {
		this->menu_isLevelHighlighterVisible = 
			!this->menu_isLevelHighlighterVisible;
	}

	if (keyboard.isKeyPushed(GamePadKey::LEFT)) {
		this->menuClickMinor_sound.play();
		if (this->menu_startLevel > MINIMAL_LEVEL) {
			--this->menu_startLevel;
			this->menu_levelHighlighter_update();
		}
	}
	else if (keyboard.isKeyPushed(GamePadKey::RIGHT)) {
		this->menuClickMinor_sound.play();
		if (this->menu_startLevel < MAXIMAL_LEVEL) {
			++this->menu_startLevel;
			this->menu_levelHighlighter_update();
		}
	}
}



void Game::menu_updateMusicSelection() {
	if (this->menu_highlightersBlinkTimer.isTriggered()) {
		this->menu_areMusicHighlightersVisible =
			!this->menu_areMusicHighlightersVisible;
	}

	if (keyboard.isKeyPushed(GamePadKey::UP)) {
		this->menuClickMinor_sound.play();
		if (this->menu_musicType > MINIMAL_MUSIC_TYPE) {
			--this->menu_musicType;
			this->menu_musicHighlighters_update();
		}
	}
	else if (keyboard.isKeyPushed(GamePadKey::DOWN)) {
		this->menuClickMinor_sound.play();
		if (this->menu_musicType < MAXIMAL_MUSIC_TYPE) {
			++this->menu_musicType;
			this->menu_musicHighlighters_update();
		}
	}
}



void Game::menu_prepareForGame() {
	this->game_field.clear();
	this->menu_setStartLevel();
	this->game_field.spawnNewPiece();
	this->game_dasState = DasState::NONE;
	this->game_allowSoftDrop = true;

	this->game_hasPieceLanded = false;
	this->game_respawnTimer.setTimingFrames(this->RESPAWN_DELAY);
	this->game_respawnTimer.reset();

	this->game_isFirstSpawn = true;
	this->game_firstSpawnTimer.setTimingFrames(this->FIRST_SPAWN_DROP_DELAY);
	this->game_firstSpawnTimer.reset();

	this->game_isPerformingAnimation = false;
	this->game_animationTimer.setTimingFrames(this->ANIMATION_TIMING);
	this->game_animationTimer.reset();
}



void Game::menu_setStartLevel() {
	int32_t newLevel = this->menu_startLevel;
	if (keyboard.isKeyHeld(GamePadKey::A)) {
		newLevel += this->LEVEL_INCREMENT_HARD_MODE;
	}
	this->game_field.setLevel(newLevel);
	this->game_levelCounter.setNumericValue(newLevel);
	this->game_dropTimer.setTimingFrames(this->framesPerGridcell[newLevel]);
}



void Game::menu_draw() {
	this->window.clear(sf::Color::Black);

	this->menu_background.drawOn(this->window);

	if (this->menu_isLevelHighlighterVisible) {
		this->window.draw(this->menu_levelHighlighter);
	}

	for (int32_t digit = 0; digit < this->DIGITS; ++digit) {
		this->window.draw(this->menu_levelDigits[digit]);
	}

	if (this->menu_areMusicHighlightersVisible) {
		for (int32_t i = 0; i < this->MUSIC_HIGHLIGHTERS; ++i) {
			this->window.draw(this->menu_musicHighlighters[i]);
		}
	}

	this->window.display();
}


#pragma /* Menu */ endregion


#pragma region Game


void Game::game_update() {
	if (keyboard.isKeyPushed(GamePadKey::START)) {
		this->pause_sound.play();
		this->changeScene(scene_handling::Scene::PAUSE_SCREEN);
		return;
	}


	if (this->game_isPerformingAnimation) {
		return;
	}
	this->game_animationTimer.reset();

	// Updates delay on first piece spawn.
	if (this->game_isFirstSpawn) {
		this->game_firstSpawnTimer.update();
		if (this->game_firstSpawnTimer.isTriggered() ||
			keyboard.isKeyHeld(GamePadKey::DOWN)) {
			this->game_isFirstSpawn = false;
		}
	}

	
	if (this->game_field.doesActivePieceExist()) {
		this->game_updatePieceControls();

		this->game_field.checkFullLines();
		const std::vector<int32_t>& linesToClear = this->game_field.getLinesToClear();
		if (linesToClear.size() != 0) {
			
			if (linesToClear.size() == 4) {
				this->tetrisCleared_sound.play();
			}
			else {
				this->lineCleared_sound.play();
			}
			this->game_field.clearLines();
		}
	}


	// Spawns new piece
	if (!this->game_field.doesActivePieceExist()) {
		this->game_respawnTimer.update();
		if (this->game_respawnTimer.isTriggered()) {
			bool spawned = this->game_field.spawnNewPiece();
			if (!spawned) {
				this->gameOver_sound.play();
				this->changeScene(scene_handling::Scene::SPLASH_SCREEN);
				return;
			}
		}
	}
}



void Game::game_updatePieceControls() {
	if (keyboard.isKeyPushed(GamePadKey::B)) {
		if (this->game_field.rotatePiece(Rotation::COUNTERCLOCKWISE)) {
			this->tetriminoRotate_sound.play();
		}
	}
	else if (keyboard.isKeyPushed(GamePadKey::A)) {
		if (this->game_field.rotatePiece(Rotation::CLOCKWISE)) {
			this->tetriminoRotate_sound.play();
		}
	}

	

	bool down = keyboard.isKeyHeld(GamePadKey::DOWN) && this->game_allowSoftDrop;
	bool left = keyboard.isKeyHeld(GamePadKey::LEFT);
	bool right = keyboard.isKeyHeld(GamePadKey::RIGHT);
	if (down && (left || right)) {
		return;
	}
	else if (down) {
		this->game_softDropTimer.update();
		if (this->game_softDropTimer.isTriggered()) {
			this->game_dropPieceDown(true);
		}
	}
	else {
		this->game_updateDas();
	}

	if (!down && !this->game_isFirstSpawn) {
		this->game_dropTimer.update();
		if (this->game_dropTimer.isTriggered()) {
			this->game_dropPieceDown(false);
		}
	}

	this->game_updateCounters();
}



void Game::game_dropPieceDown(bool isSoftDrop) {
	this->game_hasPieceLanded = this->game_field.dropPieceDown(isSoftDrop);
	if (this->game_hasPieceLanded) {
		this->tetriminoLand_sound.play();
		this->game_isPerformingAnimation = true;
		this->game_blocksToCover = 0;
	}
}



#pragma region DAS


void Game::game_updateDas() {
	this->game_updateDas_controls();
	this->game_updateDas_movePiece();
}



void Game::game_updateDas_controls() {
	this->game_previousMoveDirection = this->game_currentMoveDirection;
	this->game_currentMoveDirection = Direction::NONE;
	if (keyboard.isKeyHeld(GamePadKey::LEFT)) {
		this->game_currentMoveDirection = Direction::LEFT;
	}
	else if (keyboard.isKeyHeld(GamePadKey::RIGHT)) {
		this->game_currentMoveDirection = Direction::RIGHT;
	}
}



void Game::game_updateDas_movePiece() {
	bool currentlyPressed = this->game_currentMoveDirection != Direction::NONE;
	bool previouslyPressed = this->game_previousMoveDirection != Direction::NONE;
	bool directionsDiffer = this->game_currentMoveDirection != this->game_previousMoveDirection;
	if (!currentlyPressed) {
		this->game_dasState = DasState::NONE;
		return;
	}
	else if (previouslyPressed && directionsDiffer) {
		this->game_dasState = DasState::NONE;
	}


	if (this->game_dasState == DasState::NONE) {
		this->game_movePiece(this->game_currentMoveDirection);
		this->game_dasState = DasState::LONG_DELAYED_MOVE;
		this->game_dasTimer.setTimingFrames(this->DAS_DELAY_LONG);
		this->game_dasTimer.reset();
	}

	else if (this->game_dasState == DasState::LONG_DELAYED_MOVE) {
		this->game_dasTimer.update();
		if (this->game_dasTimer.isTriggered()) {
			this->game_movePiece(this->game_currentMoveDirection);
			this->game_dasState = DasState::SHORT_DELAYED_MOVE;
			this->game_dasTimer.setTimingFrames(this->DAS_DELAY_SHORT);
			this->game_dasTimer.reset();
		}
	}

	else if (this->game_dasState == DasState::SHORT_DELAYED_MOVE) {
		this->game_dasTimer.update();
		if (this->game_dasTimer.isTriggered()) {
			this->game_movePiece(this->game_currentMoveDirection);
		}
	}
}


#pragma /* DAS */ endregion



void Game::game_movePiece(Direction direction) {
	if (this->game_field.movePiece(direction)) {
		this->tetriminoMove_sound.play();
	}
}



void Game::game_updateCounters() {
	this->game_updateStatisticsCounters();
	this->game_updatePieceCounters();
}



void Game::game_updateStatisticsCounters() {
	this->game_tetrisesCounter.setNumericValue(this->game_field.getTetrises());
	this->game_burnCounter.setNumericValue(this->game_field.getBurn());
	this->game_tetrisRateCounter.setNumericValue(this->game_field.getTetrisRate());
	this->game_linesCounter.setNumericValue(this->game_field.getLines());
	this->game_droughtCounter.setNumericValue(this->game_field.getDrought());
	this->game_currentScoreCounter.setNumericValue(this->game_field.getScore());
	this->game_topScoreCounter.setNumericValue(this->game_field.getTopScore());

	int32_t level = this->game_field.getLevel();
	if (level != this->game_levelCounter.getNumericValue()) {
		this->game_dropTimer.setTimingFrames(this->framesPerGridcell[level]);
		this->newLevel_sound.play();
	}
	this->game_levelCounter.setNumericValue(level);
}



void Game::game_updatePieceCounters() {
	for (uint32_t piece = 0; piece < GameField::DIFFERENT_PIECES; ++piece) {
		this->game_pieceCounters[piece].setNumericValue(
			this->game_field.getPieceAmount(Piece(piece))
		);
	}
}



void Game::game_drawCounters() {
	this->game_drawStaticticsCounters();
	this->game_drawPieceCounters();
}



void Game::game_drawStaticticsCounters() {
	this->game_tetrisesCounter.drawOn(this->window);
	this->game_burnCounter.drawOn(this->window);
	this->game_tetrisRateCounter.drawOn(this->window);

	this->game_linesCounter.drawOn(this->window);

	this->game_topScoreCounter.drawOn(this->window);
	this->game_currentScoreCounter.drawOn(this->window);

	this->game_levelCounter.drawOn(this->window);
	this->game_droughtCounter.drawOn(this->window);
}



void Game::game_drawPieceCounters() {
	for (auto& counter : this->game_pieceCounters) {
		counter.drawOn(this->window);
	}
}



void Game::game_drawBlocks() {
	this->game_drawField();
	if (this->game_field.doesActivePieceExist()) {
		this->game_drawCurrentPiece();
	}
	this->game_drawNextPiece();
	this->game_drawStaticticsBlocks();
}



void Game::game_drawField() {
	const auto& blocks = this->game_field.getBlocks();
	for (uint32_t y = 2; y < blocks.size(); ++y) {
		for (uint32_t x = 0; x < blocks[y].size(); ++x) {
			if (blocks[y][x] != Tetrimino::Type::E) {
				auto drawPos = sf::Vector2f(
					this->game_blocksDrawingOffset.x + float(x * this->BLOCK_SIZE_WITH_GAP),
					this->game_blocksDrawingOffset.y + float(y * this->BLOCK_SIZE_WITH_GAP)
				);

				this->game_drawSingleBlock(drawPos, blocks[y][x]);
			}
		}
	}
	if (this->game_isPerformingAnimation) {
		this->game_drawFieldCovered();
	}
}



void Game::game_drawFieldCovered() {
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



void Game::game_drawCurrentPiece() {
	auto& pieceMatrix = this->game_field.getCurrentPieceMatrix();
	auto drawStartPos = this->game_field.getCurrentPieceCenter() - Tetrimino::Matrix::CENTER;
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
						this->game_blocksDrawingOffset.x + float(currentPos.x * this->BLOCK_SIZE_WITH_GAP),
						this->game_blocksDrawingOffset.y + float(currentPos.y * this->BLOCK_SIZE_WITH_GAP)
					);

					this->game_drawSingleBlock(drawPos, pieceMatrix[y][x]);
				}
			}
		}
	}
}



void Game::game_drawStaticticsBlocks() {
	for (uint32_t i = 0; i < this->game_staticticsBlocksData.size(); ++i) {
		const Tetrimino::Matrix::Array* matrix = std::get<0>(this->game_staticticsBlocksData[i]);
		for (uint32_t y = 0; y < matrix->size(); ++y) {
			for (uint32_t x = 0; x < matrix->size(); ++x) {
				if ((*matrix)[y][x] != Tetrimino::Type::E) {
					auto& pos = std::get<1>(this->game_staticticsBlocksData[i]);
					auto drawPos = sf::Vector2f(
						pos.x + x * this->BLOCK_SIZE_WITH_GAP,
						pos.y + y * this->BLOCK_SIZE_WITH_GAP
					);
					this->game_drawSingleBlock(drawPos, (*matrix)[y][x]);
				}
			}
		}
	}
}



void Game::game_drawNextPiece() {
	static std::vector<sf::Vector2f> offsets = {
		{ 600, 261 }, // T.
		{ 600, 261 }, // J.
		{ 600, 261 }, // Z.
		{ 610, 261 }, // O.
		{ 600, 261 }, // S.
		{ 600, 261 }, // L.
		{ 610, 272 }  // I.
	};
	auto& pieceMatrix = this->game_field.getNextPieceMatrix();
	auto type = int32_t(this->game_field.getNextPieceType());
	for (int32_t y = 0; y < Tetrimino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetrimino::Matrix::SIZE; ++x) {
			if (pieceMatrix[y][x] != Tetrimino::Type::E) {
				auto drawPos = sf::Vector2f(
					offsets[type].x + x * this->BLOCK_SIZE_WITH_GAP,
					offsets[type].y + y * this->BLOCK_SIZE_WITH_GAP
				);
				this->game_drawSingleBlock(drawPos, pieceMatrix[y][x]);
			}
		}
	}
}



void Game::game_drawPiece(const Tetrimino::Matrix::Array& matrix, const sf::Vector2f& offset) {
	for (int32_t y = 0; y < Tetrimino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetrimino::Matrix::SIZE; ++x) {
			if (matrix[y][x] != Tetrimino::Type::E) {
				auto drawPos = sf::Vector2f(
					offset.x + x * this->BLOCK_SIZE_WITH_GAP,
					offset.y + y * this->BLOCK_SIZE_WITH_GAP
				);
				this->game_drawSingleBlock(drawPos, matrix[y][x]);
			}
		}
	}
}



void Game::game_drawSingleBlock(const sf::Vector2f& position, Tetrimino::Type type) {
	this->game_blocks.setPosition(position);

	this->game_blocks.setTextureRect(sf::IntRect(
		this->BLOCK_SIZE * (this->game_levelCounter.getNumericValue() % 10),
		this->BLOCK_SIZE * int32_t(type),
		this->BLOCK_SIZE,
		this->BLOCK_SIZE
	));

	this->game_blocks.drawOn(this->window);
}



void Game::game_draw() {
	this->window.clear(sf::Color::Black);

	this->game_background.drawOn(this->window);

	this->game_droughtIndicator.drawOn(this->window);

	this->game_drawCounters();

	this->game_drawBlocks();

	this->window.display();
}


#pragma /* Game */ endregion



#pragma region Pause Screen

void Game::pauseScreen_update() {
	if (keyboard.isKeyPushed(GamePadKey::START)) {
		this->changeScene(scene_handling::Scene::GAME);
	}
	else if (keyboard.isKeyPushed(GamePadKey::EXIT)) {
		this->exit();
	}
}



void Game::pauseScreen_draw() {
	this->window.clear(sf::Color::Black);

	this->pause_background.drawOn(this->window);

	this->window.display();
}

#pragma /* Pause Screen */ endregion


#pragma /* Scene Handlers */ endregion


#pragma /* Main Loop */ endregion
