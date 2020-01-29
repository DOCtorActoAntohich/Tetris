#pragma once

namespace tetris::scene_handling {
	class SceneHandler {
	public:
		virtual void update() = 0;
		virtual void draw() = 0;
	};
}