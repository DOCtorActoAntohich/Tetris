#include "GameField.h"

#include <vector>

#include "Helper.h"

using namespace tetris;



GameField::GameField() {
	this->initializePieces();

	this->clear();
}



void GameField::moveFigure(Direction direction) {
	if (!this->currentPieceExists) {
		return;
	}

	sf::Vector2i newCenter(
		this->currentPieceCenter.x + int32_t(direction),
		this->currentPieceCenter.y
	);
	bool isPositionCorrect = this->isCenterPositionCorrect(newCenter);
	bool isCollisionFound = doesFigureCollide(this->currentPiece.getCurrentMatrix(), newCenter);
	if (isPositionCorrect & !isCollisionFound) {
		this->currentPieceCenter = newCenter;
	}
}



void GameField::rotateFigure(Rotation rotation) {
	if (!this->currentPieceExists) {
		return;
	}

	bool isCollisionFound;

	switch (rotation)
	{
	case Rotation::CLOCKWISE :
		this->currentPiece.switchToNextMatrix();
		isCollisionFound = doesFigureCollide(
			this->currentPiece.getCurrentMatrix(),
			this->currentPieceCenter
		);
		if (isCollisionFound) {
			this->currentPiece.switchToPreviousMatrix();
		}
		break;

	case Rotation::COUNTERCLOCKWISE :
		this->currentPiece.switchToPreviousMatrix();
		isCollisionFound = doesFigureCollide(
			this->currentPiece.getCurrentMatrix(),
			this->currentPieceCenter
		);
		if (isCollisionFound) {
			this->currentPiece.switchToNextMatrix();
		}
		break;
	default:
		break;
	}
}



void GameField::dropFigureDown() {
	if (!this->currentPieceExists) {
		return;
	}

	sf::Vector2i newCenter(
		this->currentPieceCenter.x,
		this->currentPieceCenter.y + 1
	);
	bool isPositionCorrect = this->isCenterPositionCorrect(newCenter);
	bool isCollisionFound = doesFigureCollide(this->currentPiece.getCurrentMatrix(), newCenter);
	if (isPositionCorrect & !isCollisionFound) {
		this->currentPieceCenter = newCenter;
	}
	else {
		this->placeCurrentPiece();
	}
}



const std::vector<std::vector<Tetromino::Type>>& GameField::getBlocks() const {
	return this->game_field;
}



void GameField::clear() {
	this->game_field = std::vector<std::vector<Tetromino::Type>>(
		FIELD_Y_SIZE, std::vector<Tetromino::Type>(FIELD_X_SIZE, Tetromino::Type::E)
	);
	this->currentPieceExists = false;
	this->canClearLines = false;
}



void GameField::clearUpperLines() {
	for (size_t y = 0; y < this->UPPER_LINES_TO_CLEAR; ++y) {
		for (size_t x = 0; x < this->FIELD_X_SIZE; ++x) {
			this->game_field[y][x] = Tetromino::Type::E;
		}
	}
}



bool GameField::isCenterPositionCorrect(const sf::Vector2i& center) const {
	bool xMinCorrect = center.x >= this->CENTER_X_POSITION_MIN;
	bool xMaxCorrect = center.x <= this->CENTER_X_POSITION_MAX;

	bool yMinCorrect = center.y >= this->CENTER_Y_POSITION_MIN;
	bool yMaxCorrect = center.y <= this->CENTER_Y_POSITION_MAX;

	return xMinCorrect && xMaxCorrect && yMinCorrect && yMaxCorrect;
}



bool GameField::doesFigureCollide(const Tetromino::Matrix::Array& figureMatrix,
								  const sf::Vector2i& figureCenter) const {

	sf::Vector2i fieldStartPosition = figureCenter - Tetromino::Matrix::CENTER;

	for (int32_t y = 0; y < Tetromino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetromino::Matrix::SIZE; ++x) {
			auto posToCheck = sf::Vector2i(
				fieldStartPosition.x + x,
				fieldStartPosition.y + y
			);
			bool xOut = posToCheck.x < 0 || posToCheck.x >= this->FIELD_X_SIZE;
			bool yOut = posToCheck.y < 0 || posToCheck.y >= this->FIELD_Y_SIZE;
			bool currentBlockCollides = figureMatrix[y][x] != Tetromino::Type::E;
			if (!(xOut || yOut)) {
				bool collidesWithBlock = this->game_field[posToCheck.y][posToCheck.x] != Tetromino::Type::E;
				if (currentBlockCollides && collidesWithBlock) {
					return true;
				}
			}
			else if ((xOut || yOut) && currentBlockCollides) {
				return true;
			}
		}
	}
	return false;
}



void GameField::placeCurrentPiece() {
	sf::Vector2i fieldStartPosition = currentPieceCenter - Tetromino::Matrix::CENTER;

	const auto& matrix = this->currentPiece.getCurrentMatrix();

	for (int32_t y = 0; y < Tetromino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetromino::Matrix::SIZE; ++x) {
			auto pos = sf::Vector2i(
				fieldStartPosition.x + x,
				fieldStartPosition.y + y
			);
			if (matrix[y][x] != Tetromino::Type::E) {
				this->game_field[pos.y][pos.x] = matrix[y][x];
			}
		}
	}
	this->currentPieceExists = false;

	this->clearUpperLines();
}



void GameField::checkFullLines() {
	if (this->linesToClear.size() != 0) {
		return;
	}
	
	int32_t startPos = this->currentPieceCenter.y - Tetromino::Matrix::CENTER.y;
	int32_t endPos   = this->currentPieceCenter.y + Tetromino::Matrix::CENTER.y;
	for (int32_t y = startPos; y <= endPos; ++y) {
		if (y >= 0 && y < FIELD_Y_SIZE) {
			bool isLineFull = true;
			for (int32_t x = 0; x < this->FIELD_X_SIZE; ++x) {
				if (this->game_field[y][x] == Tetromino::Type::E) {
					isLineFull = false;
					break;
				}
			}
			if (isLineFull) {
				this->linesToClear.push_back(y);
			}
		}
	}
	if (this->linesToClear.size() != 0) {
		this->canClearLines = true;
	}
}



void GameField::clearLines() {
	if (!this->canClearLines) {
		return;
	}

	for (int32_t line : this->linesToClear) {
		for (int32_t y = line; y >= this->UPPER_LINES_TO_CLEAR; --y) {
			this->game_field[y] = this->game_field[y - 1];
		}
	}
	this->linesToClear.clear();

	this->canClearLines = false;
}



std::vector<int32_t> GameField::getLinesToClear() {
	return this->linesToClear;
}



bool GameField::spawnNewFigure() {
	if (this->currentPieceExists) {
		return false;
	}

	static std::vector<Tetromino*> tetrominoes = {
		&this->piece_T,
		&this->piece_J,
		&this->piece_Z,
		&this->piece_O,
		&this->piece_S,
		&this->piece_L,
		&this->piece_I
	};

	size_t index = Helper::getRandomNumber(0, tetrominoes.size() - 1);

	this->currentPiece = *(tetrominoes[index]);
	this->currentPieceCenter = this->FIGURE_SPAWN;

	this->currentPieceExists = true;

	bool collides = this->doesFigureCollide(
		this->currentPiece.getCurrentMatrix(),
		this->currentPieceCenter
	);
	return !collides;
}



bool GameField::doesActivePieceExist() const {
	return this->currentPieceExists;
}



const Tetromino::Matrix::Array& GameField::getCurrentPieceMatrix() const {
	return this->currentPiece.getCurrentMatrix();
}



const sf::Vector2i& GameField::getCurrentPieceCenter() const {
	return this->currentPieceCenter;
}



void GameField::initializePiece_T() {
	using T = Tetromino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::W, T::W, T::W, T::E,
		T::E, T::E, T::W, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetromino::Matrix matrix(array);
	Tetromino::Matrix::Array matrixArray = matrix.getArray();

	this->piece_T.addMatrix(matrixArray); // T down.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_T.addMatrix(matrixArray); // T left.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_T.addMatrix(matrixArray); // T up.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_T.addMatrix(matrixArray); // T right.
}

void GameField::initializePiece_J() {
	using T = Tetromino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::B, T::B, T::B, T::E,
		T::E, T::E, T::E, T::B, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetromino::Matrix matrix(array);
	Tetromino::Matrix::Array matrixArray = matrix.getArray();

	this->piece_J.addMatrix(matrixArray); // J down.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_J.addMatrix(matrixArray); // J left.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_J.addMatrix(matrixArray); // J up.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_J.addMatrix(matrixArray); // J right.
}



void GameField::initializePiece_Z() {
	using T = Tetromino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::D, T::D, T::E, T::E,
		T::E, T::E, T::D, T::D, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetromino::Matrix matrix(array);
	this->piece_Z.addMatrix(matrix); // Z horizontal.

	array = {
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::D, T::E,
		T::E, T::E, T::D, T::D, T::E,
		T::E, T::E, T::D, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	matrix.setArray(array);
	this->piece_Z.addMatrix(matrix); // Z vertical.
}



void GameField::initializePiece_O() {
	using T = Tetromino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::W, T::W, T::E, T::E,
		T::E, T::W, T::W, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetromino::Matrix matrix(array);
	this->piece_O.addMatrix(matrix);
}



void GameField::initializePiece_S() {
	using T = Tetromino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::B, T::B, T::E,
		T::E, T::B, T::B, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetromino::Matrix matrix(array);
	this->piece_S.addMatrix(matrix); // S horizontal.

	array = {
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::B, T::E, T::E, T::E,
		T::E, T::B, T::B, T::E, T::E,
		T::E, T::E, T::B, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	matrix.setArray(array);
	this->piece_S.addMatrix(matrix); // S vertical.
}



void GameField::initializePiece_L() {
	using T = Tetromino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::D, T::D, T::D, T::E,
		T::E, T::D, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetromino::Matrix matrix(array);
	Tetromino::Matrix::Array matrixArray = matrix.getArray();

	this->piece_L.addMatrix(matrixArray); // L down.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_L.addMatrix(matrixArray); // L left.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_L.addMatrix(matrixArray); // L up.

	Helper::rotateArray(matrixArray);
	matrix.setArray(matrixArray);
	this->piece_L.addMatrix(matrixArray); // L right.
}



void GameField::initializePiece_I() {
	using T = Tetromino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::W, T::W, T::W, T::W, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetromino::Matrix matrix(array);
	this->piece_I.addMatrix(matrix); // I horizontal.

	array = {
		T::E, T::E, T::W, T::E, T::E,
		T::E, T::E, T::W, T::E, T::E,
		T::E, T::E, T::W, T::E, T::E,
		T::E, T::E, T::W, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	matrix.setArray(array);
	this->piece_I.addMatrix(matrix); // I vertical.
}



void GameField::initializePieces() {
	this->initializePiece_T();
	this->initializePiece_J();
	this->initializePiece_Z();
	this->initializePiece_O();
	this->initializePiece_S();
	this->initializePiece_L();
	this->initializePiece_I();
}
