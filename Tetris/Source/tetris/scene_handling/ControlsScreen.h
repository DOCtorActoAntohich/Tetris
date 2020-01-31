#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Sound.hpp>

#include "tetris/scene_handling/SceneHandlerBase.h"
#include "tetris/scene_handling/Scene.h"

namespace tetris::scene_handling {
	class ControlsScreen : public SceneHandlerBase {
	public:
		ControlsScreen();

		void update() override;
		void draw(sf::RenderWindow& window) override;

		void reset() override;
	private:
		sf::Sprite background;
		sf::Sound clickSound;
	};
}