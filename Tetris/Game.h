#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <vector>
#include <ResourceIndexer.h>

#include "Scene.h"
#include "Keyboard.h"


namespace tetris {
	class Game {
	public:
		Game();

		// Runs the game processes.
		void run();
	private:

#pragma region Resource Management


		sf::RenderWindow window;
		void initializeWindow();


#pragma region Loading Resources

		// Loads resources embedded into EXE.
		std::vector<byte> loadEmbeddedResource(
			int32_t id, LPCTSTR resourceType = RT_RCDATA
		);

		// Returns a texture loaded from embedded resources.
		sf::Texture* loadTexture(int32_t id);

		// Returns a sound loaded from embedded resourves.
		sf::SoundBuffer* loadSound(int32_t id);

		// Opens a music file from embedded resources.
		// Music data is saved in musicBytes which MUST NOT be changed.
		void openMusic(int32_t id, std::vector<byte>& musicBytes, sf::Music& music);

#pragma /* Loading Resources */ endregion


#pragma region Textures

		sf::Texture* splashScreen_texture;
		sf::Sprite   splashScreen_sprite;

		sf::Texture* controlsScreen_texture;
		sf::Sprite   controlsScreen_sprite;

		sf::Texture* menuScreen_texture;
		sf::Sprite   menuScreen_sprite;

#pragma /* Textures */ endregion


#pragma region Sounds

		sf::SoundBuffer* menuClickMajor_soundBuffer;
		sf::Sound		 menuClickMajor_sound;

#pragma /* Sounds */ endregion

		// Loads data to be used.
		void loadContent();

		// Frees memory.
		void unloadContent();

#pragma /* Resource Management */ endregion


#pragma region Main Loop

		// Closes the game window.
		void exit();

		Scene scene;
		bool isInputAllowed;
		Keyboard keyboard;

		// Runs the game loop.
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


		void update_SplashScreen();
		void draw_SplashScreen();

#pragma /* Splash Screen */ endregion

#pragma region Controls Screen

		void update_ControlsScreen();
		void draw_ControlsScreen();

#pragma /* Controls Screen */ endregion 

#pragma region Menu

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