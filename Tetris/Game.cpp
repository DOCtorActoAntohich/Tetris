#include "Game.h"

#include "resource.h"

#include "Helper.h"
#include <iostream>

using namespace tetris;



Game::Game() {
	changeScene(Scene::SPLASH_SCREEN);
	this->isInputAllowed = true;

	this->startLevel = 0;
	this->musicType = 0;

	this->frames = 0;
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
		predefinedLevelDigitPositions[this->startLevel]
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
			this->predefinedMusicHighlightersPositionsY[this->musicType]
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

	this->counter.applyDefaultDisplayOptions();
	this->counter.setMaximalValue(Record::MAX_LEVEL);
	this->counter.setPosition(642, 431);

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
	this->pause_background.loadFromResource(PAUSE_SCREEN_BMP);


	this->droughtIndicator.loadFromResource(DROUGHT_INDICATOR_BMP);
	this->droughtIndicator.setPosition({ 640, 490 });


	this->menuClickMajor_sound.loadFromResource(MENU_CLICK_MAJOR_WAV);
	this->menuClickMinor_sound.loadFromResource(MENU_CLICK_MINOR_WAV);
	this->pause_sound.loadFromResource(PAUSE_WAV);

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
			(this->*draw)();
		}
	}
}


void Game::changeScene(Scene scene) {
	auto [update, draw] = this->chooseUpdaters(scene);
	this->update = update;
	this->draw = draw;

	this->scene = scene;
	this->frames = 0;
}


std::pair<void(Game::*)(), void(Game::*)()> Game::chooseUpdaters(Scene scene) {
	using voidfunc = void(Game::*)();
	static std::map<Scene, std::pair<voidfunc, voidfunc>> handlers = {
		{ Scene::SPLASH_SCREEN,
		{ &Game::update_SplashScreen, &Game::draw_SplashScreen }},

		{ Scene::CONTROLS_SCREEN,
		{ &Game::update_ControlsScreen, &Game::draw_ControlsScreen }},

		{ Scene::MENU,
		{ &Game::update_Menu, &Game::draw_Menu }},

		{ Scene::GAME,
		{ &Game::update_Game, &Game::draw_Game }},

		{ Scene::PAUSE_SCREEN,
		{ &Game::update_PauseScreen, &Game::draw_PauseScreen }},
	};

	return handlers[scene];
}

#pragma region Scene Handlers


#pragma region Splash Screen


void Game::update_SplashScreen() {
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


void Game::draw_SplashScreen() {
	this->window.clear(sf::Color::Black);

	this->splash_background.draw(this->window);
	if (this->pressEnter_text_isVisible) {
		this->window.draw(this->pressEnter_text);
	}

	this->window.display();
}


#pragma /* Splash Screen */ endregion


#pragma region Controls Screen

void Game::update_ControlsScreen() {
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


void Game::draw_ControlsScreen() {
	this->window.clear(sf::Color::Black);

	this->controls_background.draw(this->window);

	this->window.display();
}

#pragma /* Controls Screen */ endregion


#pragma region Menu


void Game::update_Menu() {
	this->frames %= (this->FPS / 3);

	if (keyboard.isKeyPushed(ControlKey::START)) {
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

	this->updateLevelSelection_Menu();
	this->updateMusicSelection_Menu();
}



void Game::updateLevelSelection_Menu() {
	if (this->frames == 0) {
		this->menu_isLevelHighlighterVisible = 
			!this->menu_isLevelHighlighterVisible;
	}

	if (keyboard.isKeyPushed(ControlKey::LEFT)) {
		this->menuClickMinor_sound.play();
		if (this->startLevel > MINIMAL_LEVEL) {
			--this->startLevel;
			this->menu_levelHighlighter_update();
		}
	}
	else if (keyboard.isKeyPushed(ControlKey::RIGHT)) {
		this->menuClickMinor_sound.play();
		if (this->startLevel < MAXIMAL_LEVEL) {
			++this->startLevel;
			this->menu_levelHighlighter_update();
		}
	}
}



void Game::updateMusicSelection_Menu() {
	if (this->frames == 0) {
		this->menu_areMusicHighlightersVisible =
			!this->menu_areMusicHighlightersVisible;
	}

	if (keyboard.isKeyPushed(ControlKey::UP)) {
		this->menuClickMinor_sound.play();
		if (this->musicType > MINIMAL_MUSIC_TYPE) {
			--this->musicType;
			this->menu_musicHighlighters_update();
		}
	}
	else if (keyboard.isKeyPushed(ControlKey::DOWN)) {
		this->menuClickMinor_sound.play();
		if (this->musicType < MAXIMAL_MUSIC_TYPE) {
			++this->musicType;
			this->menu_musicHighlighters_update();
		}
	}
}



void Game::draw_Menu() {
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


void Game::update_Game() {
	if (keyboard.isKeyPushed(ControlKey::START)) {
		this->pause_sound.play();
		this->changeScene(Scene::PAUSE_SCREEN);
	}

	if (keyboard.isKeyPushed(ControlKey::B)) {
		this->field.rotateFigure(Rotation::COUNTERCLOCKWISE);
	}
	else if (keyboard.isKeyPushed(ControlKey::A)) {
		this->field.rotateFigure(Rotation::CLOCKWISE);
	}

	if (keyboard.isKeyPushed(ControlKey::DOWN)) {
		this->field.dropFigureDown();
	}
	else if (keyboard.isKeyPushed(ControlKey::LEFT)) {
		this->field.moveFigure(Direction::LEFT);
	}
	else if (keyboard.isKeyPushed(ControlKey::RIGHT)) {
		this->field.moveFigure(Direction::RIGHT);
	}
}



void Game::drawCounters_Game() {
	this->game_tetrisesCounter.draw(this->window);
	this->game_burnCounter.draw(this->window);
	this->game_tetrisRateCounter.draw(this->window);

	this->game_linesCounter.draw(this->window);
	auto a = this->game_linesCounter.getString();

	this->game_topScoreCounter.draw(this->window);
	this->game_currentScoreCounter.draw(this->window);

	this->counter.draw(this->window);
	this->game_droughtCounter.draw(this->window);
}



void Game::draw_Game() {
	this->window.clear(sf::Color::Black);

	this->game_background.draw(this->window);
	this->droughtIndicator.draw(this->window);

	this->drawCounters_Game();

	auto blocks = this->field.getBlocks();
	for (uint32_t y = 0; y < blocks.size(); ++y) {
		for (uint32_t x = 0; x < blocks[y].size(); ++x) {

		}
	}

	this->window.display();
}


#pragma /* Game */ endregion



#pragma region Pause Screen

void Game::update_PauseScreen() {
	if (keyboard.isKeyPushed(ControlKey::START)) {
		this->changeScene(Scene::GAME);
	}
	else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
		this->exit();
	}
}



void Game::draw_PauseScreen() {
	this->window.clear(sf::Color::Black);

	this->pause_background.draw(this->window);

	this->window.display();
}

#pragma /* Pause Screen */ endregion


#pragma /* Scene Handlers */ endregion


#pragma /* Main Loop */ endregion
