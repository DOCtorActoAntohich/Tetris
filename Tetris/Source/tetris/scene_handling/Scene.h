#pragma once

#include <cstdint>

namespace tetris::scene_handling {
	enum class Scene : int32_t {
		// Tells to close the app.
		EXIT = -1,

		// Tells to do nothing
		NONE = 0,



		// Tells to show splash screen with game logo.
		SPLASH_SCREEN = 1,

		// Tells to show controls screen.
		CONTROLS_SCREEN = 2,

		// Tells to show level/music select menu.
	    MENU = 3,

		// Tells to show the actual game.
		GAME = 4,

		// Tells to show pause screen.
		PAUSE_SCREEN = 5
	};
}