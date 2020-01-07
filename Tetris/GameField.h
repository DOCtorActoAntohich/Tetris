#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>

#include "FigureColor.h"

namespace tetris {
	enum class Rotation : int32_t {
		CLOCKWISE = 1,
		COUNTERCLOCKWISE = -1
	};



	enum class Direction : int32_t {
		LEFT = -1,
		RIGHT = 1
	};



	class GameField {
	public:
		static const int32_t MAX_LINES = 999;
		static const int32_t MAX_TETRISES = 999;
		static const int32_t MAX_BURN = 999;
		static const int32_t MAX_TETRIS_RATE = 100;

		GameField();

		// Moves figure 1 block left of right.
		void moveFigure(Direction direction);

		// Rotates figure clockwise or counterclockwise.
		void rotateFigure(Rotation rotation);
		
		// Moves figure 1 block down.
		void dropFigureDown();
		const std::vector<std::vector<FigureType>>& getBlocks();
	private:
		static const int32_t FIELD_X_SIZE = 10;
		static const int32_t FIELD_Y_SIZE = 22;
		std::vector<std::vector<FigureType>> field;

		// Coordinates of every new figure matrix center.
		sf::Vector2<int32_t> FIGURE_SPAWN;
	};
}
