#include "tetris/App.h"

#include "tetris/wrapper/Keyboard.h"

using namespace tetris;

App::App() {
	this->currentScene = &this->splashScreen;
	this->initializeWindow();
}



void App::initializeWindow() {
	this->window.create(
		sf::VideoMode(800, 600), "Tetris",
		sf::Style::Titlebar | sf::Style::Close //TODO: | sf::Style::Fullscreen
	);
	this->window.setFramerateLimit(this->FPS);
}



void App::run() {
	using namespace scene_handling;

	wrapper::Keyboard::initialize();

	while (this->window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->exit();
			}
		}

		if (this->window.hasFocus()) {
			wrapper::Keyboard::update();

			this->currentScene->update();
			this->currentScene->draw(this->window);

			this->chooseScene();
		}
	}
}



void App::chooseScene() {
	using namespace scene_handling;
	auto nextScene = this->currentScene->getNextScene();
	switch (nextScene)
	{
	case Scene::NONE :
		break;
	case Scene::EXIT :
		this->exit();
		break;
	case Scene::SPLASH_SCREEN :
		this->currentScene = &this->splashScreen;
		this->currentScene->reset();
		break;
	case Scene::CONTROLS_SCREEN :
		this->currentScene = &this->controlsScreen;
		this->currentScene->reset();
		break;
	case Scene::MENU :
		this->currentScene = &this->menu;
		break;
	case Scene::GAME :
		{
			auto [level, music] = this->menu.getGameParameters();
			this->currentScene = &this->game;
			this->currentScene->reset();
		}
		break;
	default :
		break;
	}
}



void App::exit() {
	this->window.close();
}
