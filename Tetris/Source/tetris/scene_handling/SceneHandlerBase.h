#pragma once

#include <SFML/Graphics.hpp>

#include "tetris/wrapper/ControlKey.h"
#include "tetris/wrapper/Keyboard.h"
#include "tetris/scene_handling/Scene.h"



namespace tetris::scene_handling {
	class SceneHandlerBase {
		using GamePadKey = wrapper::GamePadKey;
		using Keyboard = wrapper::Keyboard;

	public:
		SceneHandlerBase();
		
		virtual void update() = 0;
		virtual void draw(sf::RenderWindow& window) = 0;

		virtual void reset() = 0;

		Scene getNextScene();
	protected:
		Scene nextScene;
	};
}