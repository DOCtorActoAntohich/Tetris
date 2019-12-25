#include "Game.h"

#include "resource.h"

namespace tetris {
	Game::Game() {
		changeScene(Scene::SPLASH_SCREEN);
		this->isInputAllowed = true;

		this->startLevel = 0;
		this->musicType = 0;
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
			sf::Style::Titlebar | sf::Style::Close //TODO: add "sf::Style::Fullscreen"
		);
		this->window.setFramerateLimit(60);
	}



#pragma region Loading Resources


	std::vector<byte> Game::loadEmbeddedResource(int32_t id) {
		// Took code from link bellow and rewrote a bit.
		// https://smacdo.com/programming/embedding-user-defined-resources-in-visual-c-binaries/

		std::vector<byte> output;

		HMODULE module = nullptr;
		LPCTSTR name = MAKEINTRESOURCE(id);
		LPCTSTR type = RT_RCDATA;

		// Raw data is used here because resource fails to be read otherwise.
		HRSRC resourceHandle = ::FindResource(module, name, type);
		if (resourceHandle == nullptr) {
			return output;
		}

		HGLOBAL dataHandle = ::LoadResource(module, resourceHandle);
		if (dataHandle == nullptr) {
			return output;
		}

		DWORD resourceSize = ::SizeofResource(module, resourceHandle);
		if (resourceSize == 0) {
			return output;
		}

		auto firstByte = reinterpret_cast<const char*>(::LockResource(dataHandle));
		if (firstByte == nullptr) {
			return output;
		}

		output.resize(resourceSize);
		std::copy(firstByte, firstByte + resourceSize, output.begin());

		return output;
	}



	sf::Texture* Game::loadTexture(int32_t id) {
		std::vector<byte> bytes = this->loadEmbeddedResource(id);
		sf::Texture* texture = new sf::Texture;
		(*texture).loadFromMemory(&bytes[0], bytes.size());
		return texture;
	}



	sf::SoundBuffer* Game::loadSound(int32_t id) {
		std::vector<byte> bytes = this->loadEmbeddedResource(id);
		sf::SoundBuffer* soundBuffer = new sf::SoundBuffer;
		(*soundBuffer).loadFromMemory(&bytes[0], bytes.size());
		return soundBuffer;
	}



	sf::Font* Game::loadFont(int32_t id) {
		std::vector<byte> bytes = this->loadEmbeddedResource(id);
		sf::Font* font = new sf::Font;
		(*font).loadFromMemory(&bytes[0], bytes.size());
		return font;
	}



	void Game::openMusic(int32_t id, std::vector<byte>& musicBytes,
						 sf::Music& music) {
		musicBytes = this->loadEmbeddedResource(id);
		music.openFromMemory(&musicBytes[0], musicBytes.size());
	}


#pragma /* Loading Resources */ endregion



	void Game::loadContent() {
		this->splashScreen_texture = this->loadTexture(SPLASH_SCREEN_BMP);
		this->splashScreen_sprite.setTexture(*this->splashScreen_texture);

		this->controlsScreen_texture = this->loadTexture(CONTROLS_SCREEN_BMP);
		this->controlsScreen_sprite.setTexture(*this->controlsScreen_texture);

		this->menuScreen_texture = this->loadTexture(LEVEL_SELECT_SCREEN_BMP);
		this->menuScreen_sprite.setTexture(*this->menuScreen_texture);



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
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					this->exit();
				}
			}

			this->keyboard.update();

			(this->*update)();
			(this->*draw)();
		}
	}


	void Game::changeScene(Scene scene) {
		this->menuClickMajor_sound.play();

		auto [update, draw] = this->chooseUpdaters(scene);
		this->update = update;
		this->draw = draw;

		this->scene = scene;
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
		if (keyboard.isKeyPushed(ControlKey::START)) {
			this->changeScene(Scene::CONTROLS_SCREEN);
		}
		else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
			this->window.close();
		}
	}


	void Game::draw_SplashScreen() {
		this->window.clear(sf::Color::Black);

		this->window.draw(this->splashScreen_sprite);

		this->window.display();
	}


#pragma /* Splash Screen */ endregion


#pragma region Controls Screen

	void Game::update_ControlsScreen() {
		if (keyboard.isKeyPushed(ControlKey::START)) {
			this->changeScene(Scene::MENU);
		}
		else if (keyboard.isKeyPushed(ControlKey::B)) {
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
		if (keyboard.isKeyPushed(ControlKey::START)) {
			//this->changeScene(Scene::GAME);
		}
		else if (keyboard.isKeyPushed(ControlKey::B)) {
			this->changeScene(Scene::CONTROLS_SCREEN);
		}
		else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
			this->window.close();
		}

		this->update_Menu_LevelSelection();
		this->update_Menu_MusicSelection();
	}



	void Game::update_Menu_LevelSelection() {
		if (keyboard.isKeyPushed(ControlKey::LEFT)) {
			this->menuClickMinor_sound.play();
			if (this->startLevel > MINIMAL_LEVEL) {
				--this->startLevel;
			}
		}
		else if (keyboard.isKeyPushed(ControlKey::RIGHT)) {
			this->menuClickMinor_sound.play();
			if (this->startLevel < MAXIMAL_LEVEL) {
				++this->startLevel;
			}
		}
	}



	void Game::update_Menu_MusicSelection() {
		if (keyboard.isKeyPushed(ControlKey::UP)) {
			this->menuClickMinor_sound.play();
			if (this->musicType > MINIMAL_MUSIC_TYPE) {
				--this->musicType;
			}
		}
		else if (keyboard.isKeyPushed(ControlKey::DOWN)) {
			this->menuClickMinor_sound.play();
			if (this->musicType < MAXIMAL_MUSIC_TYPE) {
				++this->musicType;
			}
		}
	}



	void Game::draw_Menu() {
		this->window.clear(sf::Color::Black);

		this->window.draw(this->menuScreen_sprite);

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