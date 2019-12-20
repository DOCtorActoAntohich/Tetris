#include "Game.h"

#include "resource.h"

namespace tetris {
	Game::Game() {
		changeScene(Scene::SPLASH_SCREEN);
		this->isInputAllowed = true;
		
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


	std::vector<byte> Game::LoadEmbeddedResource(int32_t id) {
		// Took code from link bellow and rewrote a bit.
		// https://smacdo.com/programming/embedding-user-defined-resources-in-visual-c-binaries/

		std::vector<byte> output;

		HMODULE module = nullptr;

		LPCTSTR name = MAKEINTRESOURCE(id);
		
		// Raw data is used here because BMP fails to be read otherwise.
		HRSRC resourceHandle = ::FindResource(module, name, RT_RCDATA);
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



	sf::Texture* Game::LoadTexture(int32_t id) {
		std::vector<byte> bytes = this->LoadEmbeddedResource(id);
		sf::Texture *texture = new sf::Texture;
		(*texture).loadFromMemory(&bytes[0], bytes.size());
		return texture;
	}



	void Game::loadContent() {
		this->splashScreenTexture = this->LoadTexture(SPLASH_SCREEN_BMP);
		this->splashScreenSprite.setTexture(*this->splashScreenTexture);

		this->controlsScreenTexture = this->LoadTexture(CONTROLS_SCREEN_BMP);
		this->controlsScreenSprite.setTexture(*this->controlsScreenTexture);
		
		this->menuScreenTexture = this->LoadTexture(MENU_SCREEN_BMP);
		this->menuScreenSprite.setTexture(*this->menuScreenTexture);

		this->initializeWindow();
	}



	void Game::unloadContent() {
		delete this->splashScreenTexture;
		delete this->controlsScreenTexture;
		delete this->menuScreenTexture;
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

			case Scene::CONTROLS_SCREEN:
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
			default : // No other options left, "default" is here for compiler's sake.
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

		this->window.draw(this->splashScreenSprite);

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

		this->window.draw(this->controlsScreenSprite);

		this->window.display();
	}

#pragma /* Controls Screen */ endregion


#pragma region Menu


	void Game::update_Menu() {
		if (keyboard.isKeyPushed(ControlKey::B)) {
			this->changeScene(Scene::CONTROLS_SCREEN);
		}
		else if (keyboard.isKeyPushed(ControlKey::EXIT)) {
			this->window.close();
		}
	}


	void Game::draw_Menu() {
		this->window.clear(sf::Color::Black);

		this->window.draw(this->menuScreenSprite);

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