#pragma once

#include "tetris/scene_handling/SceneHandlerBase.h"

namespace tetris::scene_handling {
	class GameScene : public SceneHandlerBase {
	public:
		GameScene();

		void update();
		void draw(sf::RenderWindow& window);

		void reset();
	private:

	};
}