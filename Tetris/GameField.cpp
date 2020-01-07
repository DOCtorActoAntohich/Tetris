#include "GameField.h"

namespace tetris {
	GameField::GameField() {
		this->FIGURE_SPAWN = sf::Vector2<int32_t>(5, 2);

		this->field = std::vector<std::vector<FigureType>>(
			FIELD_Y_SIZE, std::vector<FigureType>(FIELD_X_SIZE)
		);
	}



	void GameField::moveFigure(Direction direction) {

	}



	void GameField::rotateFigure(Rotation rotation) {

	}



	void GameField::dropFigureDown() {

	}



	const std::vector<std::vector<FigureType>>& GameField::getBlocks()
	{
		return this->field;
	}
}