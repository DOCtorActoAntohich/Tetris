#pragma once

#include "tetris/scene_handling/Scenes.h"

namespace tetris {
	class App {
	public:
		App();

		void run();
	private:
		static const int32_t FPS = 60;

		sf::RenderWindow window;
		void initializeWindow();

		void chooseScene();

		void exit();

		scene_handling::SceneHandlerBase* currentScene;

		scene_handling::SplashScreen	splashScreen;
		scene_handling::ControlsScreen  controlsScreen;
		scene_handling::Menu			menu;
		scene_handling::GameScene		game;


		void (scene_handling::SceneHandlerBase::* update)();
		void (scene_handling::SceneHandlerBase::* draw)(sf::RenderWindow&);
	};
}