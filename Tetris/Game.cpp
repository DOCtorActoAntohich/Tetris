#include "Game.h"

#include "resource.h"

#include "Helper.h"
#include <iostream>

using namespace tetris;



Game::Game() {
	changeScene(Scene::SPLASH_SCREEN);

	this->menu_startLevel = 0;
	this->menu_musicType = 0;

	this->frames = 0;

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
	bytes = Helper::loadEmbeddedResource(id);
	font.loadFromMemory(&bytes[0], bytes.size());
}



void Game::openMusic(int32_t id, std::vector<byte>& musicBytes,
						sf::Music& music) {
	musicBytes = Helper::loadEmbeddedResource(id);
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
	CounterUI::setDefaultDisplayOptions(
		this->font, this->FONT_SIZE,
		sf::Color::White, sf::Text::Style::Regular
	);

	this->game_tetrisesCounter.applyDefaultDisplayOptions();
	this->game_tetrisesCounter.setMaximalValue(GameField::MAX_TETRISES);
	this->game_tetrisesCounter.setPosition(177, 419);

	this->game_burnCounter.applyDefaultDisplayOptions();
	this->game_burnCounter.setMaximalValue(GameField::MAX_BURN);
	this->game_burnCounter.setPosition(177, 464);

	this->game_tetrisRateCounter.applyDefaultDisplayOptions();
	this->game_tetrisRateCounter.setMaximalValue(GameField::MAX_TETRIS_RATE);
	this->game_tetrisRateCounter.setWidth(2);
	this->game_tetrisRateCounter.setPosition(177, 510);

	this->game_linesCounter.applyDefaultDisplayOptions();
	this->game_linesCounter.setMaximalValue(GameField::MAX_LINES);
	this->game_linesCounter.setPosition(482, 43);

	this->game_topScoreCounter.applyDefaultDisplayOptions();
	this->game_topScoreCounter.setMaximalValue(Record::MAX_SCORE);
	this->game_topScoreCounter.setPosition(608, 86);

	this->game_currentScoreCounter.applyDefaultDisplayOptions();
	this->game_currentScoreCounter.setMaximalValue(Record::MAX_SCORE);
	this->game_currentScoreCounter.setPosition(608, 166);

	this->game_levelCounter.applyDefaultDisplayOptions();
	this->game_levelCounter.setMaximalValue(Record::MAX_LEVEL);
	this->game_levelCounter.setPosition(642, 431);

	this->game_droughtCounter.applyDefaultDisplayOptions();
	this->game_droughtCounter.setMaximalValue(99);
	this->game_droughtCounter.setPosition(642, 519);
}


#pragma /* Other Data */ endregion



void Game::loadContent() {
	loadFont(GAME_FONT, this->fontBytes, this->font);


	this->splash_background.loadFromResource(SPLASH_SCREEN_BMP);
	this->pressEnter_text_initialize();

	this->controls_background.loadFromResource(CONTROLS_SCREEN_BMP);

	this->menu_background.loadFromResource(MENU_SCREEN_BMP);
	this->menu_levelHighlighter_initialize();
	this->menu_levelDigits_initialize();
	this->menu_musicHighlighters_initialize();

	this->game_background.loadFromResource(GAME_SCREEN_BMP);
	this->game_BlocksDrawingOffset = sf::Vector2f(321, 70);

	this->pause_background.loadFromResource(PAUSE_SCREEN_BMP);


	this->game_droughtIndicator.loadFromResource(DROUGHT_INDICATOR_BMP);
	this->game_droughtIndicator.setPosition({ 640, 490 });

	this->game_blocks.loadFromResource(BLOCKS_BMP);


	this->menuClickMajor_sound.loadFromResource(MENU_CLICK_MAJOR_WAV);
	this->menuClickMinor_sound.loadFromResource(MENU_CLICK_MINOR_WAV);
	this->pause_sound.loadFromResource(PAUSE_WAV);

	this->tetrominoMove_sound.loadFromResource(TETROMINO_MOVE_WAV);
	this->tetrominoRotate_sound.loadFromResource(TETROMINO_ROTATE_WAV);
	this->tetrominoLand_sound.loadFromResource(TETROMINO_LAND_WAV);

	this->initializeCounters();

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
			++frames;

			(this->*update)();
			if (this->shouldCallDrawer) {
				(this->*draw)();
			}
			if (!this->shouldCallDrawer) {
				this->shouldCallDrawer = true;
			}
		}
	}
}


void Game::changeScene(Scene scene) {
	auto [update, draw] = this->chooseUpdaters(scene);
	this->update = update;
	this->draw = draw;

	this->scene = scene;
	this->frames = 0;

	this->shouldCallDrawer = false;
}


std::pair<void(Game::*)(), void(Game::*)()> Game::chooseUpdaters(Scene scene) {
	using voidfunc = void(Game::*)();
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
	this->frames %= (this->FPS / 2);
	if (this->frames == 0) {
		this->pressEnter_text_isVisible = !this->pressEnter_text_isVisible;
	}

	if (keyboard.isKeyPushed(ControlKey::START)) {
		this->menuClickMajor_sound.play();
		this->changeScene(Scene::CONTROLS_SCREEN);
	}
	else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
		this->window.close();
	}
}


void Game::splashScreen_draw() {
	this->window.clear(sf::Color::Black);

	this->splash_background.draw(this->window);
	if (this->pressEnter_text_isVisible) {
		this->window.draw(this->pressEnter_text);
	}

	this->window.display();
}


#pragma /* Splash Screen */ endregion


#pragma region Controls Screen

void Game::controlsScreen_update() {
	this->frames %= this->FPS;

	if (keyboard.isKeyPushed(ControlKey::START)) {
		this->menuClickMajor_sound.play();
		this->changeScene(Scene::MENU);
	}
	else if (keyboard.isKeyPushed(ControlKey::B)) {
		this->menuClickMajor_sound.play();
		this->changeScene(Scene::SPLASH_SCREEN);
	}
	else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
		this->window.close();
	}
}


void Game::controlsScreen_draw() {
	this->window.clear(sf::Color::Black);

	this->controls_background.draw(this->window);

	this->window.display();
}

#pragma /* Controls Screen */ endregion


#pragma region Menu


void Game::menu_update() {
	this->frames %= (this->FPS / 3);

	if (keyboard.isKeyPushed(ControlKey::START)) {
		this->menu_setStartLevel();
		this->game_field.clear();
		this->game_field.spawnNewFigure();
		this->menuClickMajor_sound.play();
		this->changeScene(Scene::GAME);
	}
	else if (keyboard.isKeyPushed(ControlKey::B)) {
		this->menuClickMajor_sound.play();
		this->changeScene(Scene::CONTROLS_SCREEN);
	}
	else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
		this->window.close();
	}

	this->menu_updateLevelSelection();
	this->menu_updateMusicSelection();
}



void Game::menu_updateLevelSelection() {
	if (this->frames == 0) {
		this->menu_isLevelHighlighterVisible = 
			!this->menu_isLevelHighlighterVisible;
	}

	if (keyboard.isKeyPushed(ControlKey::LEFT)) {
		this->menuClickMinor_sound.play();
		if (this->menu_startLevel > MINIMAL_LEVEL) {
			--this->menu_startLevel;
			this->menu_levelHighlighter_update();
		}
	}
	else if (keyboard.isKeyPushed(ControlKey::RIGHT)) {
		this->menuClickMinor_sound.play();
		if (this->menu_startLevel < MAXIMAL_LEVEL) {
			++this->menu_startLevel;
			this->menu_levelHighlighter_update();
		}
	}
}



void Game::menu_updateMusicSelection() {
	if (this->frames == 0) {
		this->menu_areMusicHighlightersVisible =
			!this->menu_areMusicHighlightersVisible;
	}

	if (keyboard.isKeyPushed(ControlKey::UP)) {
		this->menuClickMinor_sound.play();
		if (this->menu_musicType > MINIMAL_MUSIC_TYPE) {
			--this->menu_musicType;
			this->menu_musicHighlighters_update();
		}
	}
	else if (keyboard.isKeyPushed(ControlKey::DOWN)) {
		this->menuClickMinor_sound.play();
		if (this->menu_musicType < MAXIMAL_MUSIC_TYPE) {
			++this->menu_musicType;
			this->menu_musicHighlighters_update();
		}
	}
}



void Game::menu_setStartLevel() {
	int32_t newLevel = int32_t(this->menu_startLevel);
	if (keyboard.isKeyHeld(ControlKey::A)) {
		newLevel += this->LEVEL_INCREMENT_HARD_MODE;
	}
	this->game_levelCounter.setNumericValue(newLevel);
}



void Game::menu_draw() {
	this->window.clear(sf::Color::Black);

	this->menu_background.draw(this->window);

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
	if (keyboard.isKeyPushed(ControlKey::START)) {
		this->pause_sound.play();
		this->changeScene(Scene::PAUSE_SCREEN);
	}

	if (!this->game_field.doesActivePieceExist()) {
		this->tetrominoLand_sound.play();
		bool spawned = this->game_field.spawnNewFigure();
		if (!spawned) {
			this->changeScene(Scene::SPLASH_SCREEN);
			return;
		}
	}

	this->game_updateFigureControls();

	this->game_field.checkFullLines();
	std::vector<int32_t> linesToClear = this->game_field.getLinesToClear();
	if (linesToClear.size() != 0) {
		this->game_field.clearLines();
	}
}



void Game::game_updateFigureControls() {
	if (keyboard.isKeyPushed(ControlKey::B)) {
		this->tetrominoRotate_sound.play();
		this->game_field.rotateFigure(Rotation::COUNTERCLOCKWISE);
	}
	else if (keyboard.isKeyPushed(ControlKey::A)) {
		this->tetrominoRotate_sound.play();
		this->game_field.rotateFigure(Rotation::CLOCKWISE);
	}

	if (keyboard.isKeyPushed(ControlKey::DOWN)) {
		this->game_field.dropFigureDown();
	}
	else if (keyboard.isKeyPushed(ControlKey::LEFT)) {
		this->tetrominoMove_sound.play();
		this->game_field.moveFigure(Direction::LEFT);
	}
	else if (keyboard.isKeyPushed(ControlKey::RIGHT)) {
		this->tetrominoMove_sound.play();
		this->game_field.moveFigure(Direction::RIGHT);
	}
}



void Game::game_drawCounters() {
	this->game_tetrisesCounter.draw(this->window);
	this->game_burnCounter.draw(this->window);
	this->game_tetrisRateCounter.draw(this->window);

	this->game_linesCounter.draw(this->window);
	auto a = this->game_linesCounter.getString();

	this->game_topScoreCounter.draw(this->window);
	this->game_currentScoreCounter.draw(this->window);

	this->game_levelCounter.draw(this->window);
	this->game_droughtCounter.draw(this->window);
}



void Game::game_drawBlocks() {
	this->game_drawField();
	if (this->game_field.doesActivePieceExist()) {
		this->game_drawCurrentPiece();
	}
}



void Game::game_drawField() {
	const auto& blocks = this->game_field.getBlocks();
	for (uint32_t y = 2; y < blocks.size(); ++y) {
		for (uint32_t x = 0; x < blocks[y].size(); ++x) {
			auto drawPos = sf::Vector2f(
				this->game_BlocksDrawingOffset.x + float(x * this->BLOCK_SIZE_WITH_GAP),
				this->game_BlocksDrawingOffset.y + float(y * this->BLOCK_SIZE_WITH_GAP)
			);

			this->game_drawSingleBlock(drawPos, blocks[y][x]);
		}
	}
}



void Game::game_drawCurrentPiece() {
	auto& pieceMatrix = this->game_field.getCurrentPieceMatrix();
	auto drawStartPos = this->game_field.getCurrentPieceCenter() - Tetromino::Matrix::CENTER;
	for (int32_t y = 0; y < Tetromino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetromino::Matrix::SIZE; ++x) {
			sf::Vector2i currentPos = sf::Vector2i(
				drawStartPos.x + x,
				drawStartPos.y + y
			);
			
			bool xOut = currentPos.x < 0 || currentPos.x > GameField::FIELD_X_SIZE;
			bool yOut = currentPos.y < 0 || currentPos.y > GameField::FIELD_Y_SIZE;
			

			if (!(xOut || yOut)) {
				bool isDrawPositionSuitable = currentPos.y >= GameField::UPPER_LINES_TO_CLEAR;
				bool isBlockVisible = pieceMatrix[y][x] != Tetromino::Type::E;
				if (isDrawPositionSuitable && isBlockVisible) {
					auto drawPos = sf::Vector2f(
						this->game_BlocksDrawingOffset.x + float(currentPos.x * this->BLOCK_SIZE_WITH_GAP),
						this->game_BlocksDrawingOffset.y + float(currentPos.y * this->BLOCK_SIZE_WITH_GAP)
					);

					this->game_drawSingleBlock(drawPos, pieceMatrix[y][x]);
				}
			}
		}
	}
}



void Game::game_drawSingleBlock(const sf::Vector2f& position, Tetromino::Type type) {
	this->game_blocks.setPosition(position);

	if (type == Tetromino::Type::E) {
		this->game_blocks.setColor(sf::Color::Black);
	}
	else {
		this->game_blocks.setColor(sf::Color::White);
	}

	this->game_blocks.setTextureRect(sf::IntRect(
		this->BLOCK_SIZE * (this->game_levelCounter.getNumericValue() % 10),
		this->BLOCK_SIZE * int32_t(type),
		this->BLOCK_SIZE,
		this->BLOCK_SIZE
	));

	this->game_blocks.draw(this->window);
}



void Game::game_draw() {
	this->window.clear(sf::Color::Black);

	this->game_background.draw(this->window);

	this->game_droughtIndicator.draw(this->window);

	this->game_drawCounters();

	this->game_drawBlocks();

	this->window.display();
}


#pragma /* Game */ endregion



#pragma region Pause Screen

void Game::pauseScreen_update() {
	if (keyboard.isKeyPushed(ControlKey::START)) {
		this->changeScene(Scene::GAME);
	}
	else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
		this->exit();
	}
}



void Game::pauseScreen_draw() {
	this->window.clear(sf::Color::Black);

	this->pause_background.draw(this->window);

	this->window.display();
}

#pragma /* Pause Screen */ endregion


#pragma /* Scene Handlers */ endregion


#pragma /* Main Loop */ endregion
