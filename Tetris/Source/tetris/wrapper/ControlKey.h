#pragma once

namespace tetris::wrapper {
	// Helps to avoid long names like sf::Keyboard::Key::Enter.
	enum class ControlKey {
		UP = 0,
		DOWN = 1, 
		LEFT = 2,
		RIGHT = 3,

		START = 4,

		B = 5,
		A = 6,

		EXIT = 7
	};
}