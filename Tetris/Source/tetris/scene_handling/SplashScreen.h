#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "tetris/scene_handling/SceneHandlerBase.h"
#include "tetris/wrapper/BlinkingObject.h"
#include "tetris/wrapper/FrameTimer.h"

namespace tetris::scene_handling {
	class SplashScreen final : SceneHandlerBase {
	public:
		SplashScreen();

		void update() override;
		void draw(sf::RenderWindow& window) override;

		void reset() override;
	private:
		sf::Sprite background;
		sf::Sound click_sound;
		void initializeResources();

		wrapper::BlinkingObject<sf::Text> pressEnter_text;
		void initializeText();
		

		void update_sceneControls();
	};
}