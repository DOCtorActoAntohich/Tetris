#include "Game.h"

#include "resource.h"

#include "Helper.h"
#include <iostream>

namespace tetris {
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


	sf::Texture* Game::loadTexture(int32_t id) {
		std::vector<byte> bytes = Helper::loadEmbeddedResource(id);
		sf::Texture* texture = new sf::Texture;
		(*texture).loadFromMemory(&bytes[0], bytes.size());
		return texture;
	}



	sf::SoundBuffer* Game::loadSound(int32_t id) {
		std::vector<byte> bytes = Helper::loadEmbeddedResource(id);
		sf::SoundBuffer* soundBuffer = new sf::SoundBuffer;
		(*soundBuffer).loadFromMemory(&bytes[0], bytes.size());
		return soundBuffer;
	}



	void Game::loadFont(int32_t id, std::vector<byte>& bytes, sf::Font& font) {
		bytes = Helper::loadEmbeddedResource(id);
		font.loadFromMemory(&bytes[0], bytes.size());
	}



	void Game::openMusic(int32_t id, std::vector<byte>& musicBytes,
						 sf::Music& music) {
		musicBytes = Helper::loadEmbeddedResource(id);
		music.openFromMemory(&musicBytes[0], musicBytes.size());
	}



	std::filesystem::path Game::getGameFolderPath() {
		auto path = Helper::getProgramDataPath();
		path /= this->GAME_DATA_FOLDER;
		return path;
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


#pragma /* Other Data */ endregion



	void Game::loadContent() {
		loadFont(GAME_FONT, this->fontBytes, this->font);


		this->splashScreen_texture = this->loadTexture(SPLASH_SCREEN_BMP);
		this->splashScreen_sprite.setTexture(*this->splashScreen_texture);
		this->pressEnter_text_initialize();


		this->controlsScreen_texture = this->loadTexture(CONTROLS_SCREEN_BMP);
		this->controlsScreen_sprite.setTexture(*this->controlsScreen_texture);

		
		this->menuScreen_texture = this->loadTexture(LEVEL_SELECT_SCREEN_BMP);
		this->menuScreen_sprite.setTexture(*this->menuScreen_texture);
		this->menu_levelHighlighter_initialize();
		this->menu_levelDigits_initialize();
		this->menu_musicHighlighters_initialize();


		this->menuClickMajor_soundBuffer = this->loadSound(MENU_CLICK_MAJOR_WAV);
		this->menuClickMajor_sound.setBuffer(*this->menuClickMajor_soundBuffer);

		this->menuClickMinor_soundBuffer = this->loadSound(MENU_CLICK_MINOR_WAV);
		this->menuClickMinor_sound.setBuffer(*this->menuClickMinor_soundBuffer);


		this->initializeWindow();
	}



	void Game::unloadContent() {
		delete this->splashScreen_texture;
		delete this->controlsScreen_texture;
		delete this->menuScreen_texture;

		delete this->menuClickMajor_soundBuffer;
		delete this->menuClickMinor_soundBuffer;
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
			if (this->window.hasFocus()) {
				sf::Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						this->exit();
					}
				}

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

		std::pair<voidfunc, voidfunc> result;
		switch (scene)
		{
		case Scene::SPLASH_SCREEN :
			result = std::make_pair<voidfunc, voidfunc>(
				&Game::update_SplashScreen, &Game::draw_SplashScreen
			);
			break;

		case Scene::CONTROLS_SCREEN :
			result = std::make_pair<voidfunc, voidfunc>(
				&Game::update_ControlsScreen, &Game::draw_ControlsScreen
			);
			break;

		case Scene::MENU :
			result = std::make_pair<voidfunc, voidfunc>(
				&Game::update_Menu, &Game::draw_Menu
			);
			break;

		case Scene::GAME :
		default : // No other options left, "default" is here for safety.
			result = std::make_pair<voidfunc, voidfunc>(
				&Game::update_Game, &Game::draw_Game
			);
			break;
		}
		return result;
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

		this->window.draw(this->splashScreen_sprite);
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

		this->window.draw(this->controlsScreen_sprite);

		this->window.display();
	}

#pragma /* Controls Screen */ endregion


#pragma region Menu


	void Game::update_Menu() {
		this->frames %= (this->FPS / 3);

		if (keyboard.isKeyPushed(ControlKey::START)) {
			this->menuClickMajor_sound.play();
			//this->changeScene(Scene::GAME);
		}
		else if (keyboard.isKeyPushed(ControlKey::B)) {
			this->menuClickMajor_sound.play();
			this->changeScene(Scene::CONTROLS_SCREEN);
		}
		else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
			this->window.close();
		}

		this->update_Menu_LevelSelection();
		this->update_Menu_MusicSelection();
	}



	void Game::update_Menu_LevelSelection() {
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



	void Game::update_Menu_MusicSelection() {
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

		this->window.draw(this->menuScreen_sprite);

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

	}


	void Game::draw_Game() {
		this->window.clear(sf::Color::Black);

		this->window.display();
	}


#pragma /* Game */ endregion


#pragma /* Scene Handlers */ endregion


#pragma /* Main Loop */ endregion
}