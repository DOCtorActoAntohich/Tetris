#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "tetris/scene_handling/SceneHandlerBase.h"
#include "tetris/wrapper/FrameTimer.h"
#include "tetris/scene_handling/Scene.h"

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
		void initializeText();

		void update_sceneControls();

		static const int32_t FONT_SIZE = 18;

		// How often splash screen text will blink (in frames).
		static const int32_t TEXT_BLINK_TIMING = 30;
		wrapper::FrameTimer textBlinkTimer;
		bool isTextVisible;
		sf::Text pressEnter_text;
	};
}