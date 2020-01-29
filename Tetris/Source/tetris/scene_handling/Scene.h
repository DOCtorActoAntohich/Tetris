#pragma once

#include <cstdint>

namespace tetris::scene_handling {
	enum class Scene : int32_t {
		SPLASH_SCREEN = 0,
		CONTROLS_SCREEN = 1,
	    MENU = 2,
		GAME = 3,
		PAUSE_SCREEN = 4
	};
}