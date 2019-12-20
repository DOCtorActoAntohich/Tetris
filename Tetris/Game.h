#pragma once

#include <SFML/Graphics.hpp>

#include <ResourceIndexer.h>
#include <vector>

#include "Scene.h"
#include "Keyboard.h"


namespace tetris {
	class Game {
	public:
		Game();

		void run();
	private:

#pragma region Resource Management

		sf::RenderWindow window;
		void initializeWindow();

		// Loads resources embedded into EXE.
		std::vector<byte> LoadEmbeddedResource(int32_t id);

		// Returns texture loaded from embedded resources.
		sf::Texture* LoadTexture(int32_t id);

		void loadContent();
		void unloadContent();

#pragma /* Resource Management */ endregion
		

#pragma region Main Loop

		void exit();

		Scene scene;
		bool isInputAllowed;
		Keyboard keyboard;

		void runMainLoop();

		// Delegate calling scene updaters.
		void (Game::* update)();

		// Delegate calling scene drawers.
		void (Game::* draw)();

		// Changes scene handlers.
		void changeScene(Scene scene);

		// Returns pointers to handlers of a specified scene.
		// First handler is updater, second is drawer.
		std::pair<void(Game::*)(), void(Game::*)()> chooseUpdaters(Scene scene);

#pragma region Scene Handlers

#pragma region Splash Screen

		sf::Texture* splashScreenTexture;
		sf::Sprite  splashScreenSprite;

		void update_SplashScreen();
		void draw_SplashScreen();

#pragma /* Splash Screen */ endregion

#pragma region Controls Screen

		sf::Texture* controlsScreenTexture;
		sf::Sprite   controlsScreenSprite;

		void update_ControlsScreen();
		void draw_ControlsScreen();

#pragma /* Controls Screen */ endregion 

#pragma region Menu

		sf::Texture* menuScreenTexture;
		sf::Sprite   menuScreenSprite;

		void update_Menu();
		void draw_Menu();

#pragma /* Menu */ endregion

#pragma region Game

		void update_Game();
		void draw_Game();

#pragma /* Game */ endregion

#pragma /* Scene Handlers */ endregion


#pragma /* Main Loop */ endregion
		
	};
}