#include "GameField.h"

#include <vector>

#include "Helper.h"

using namespace tetris;



GameField::GameField() {
	this->initializePieces();

	this->clear();
}


#pragma region Controls


bool GameField::moveFigure(Direction direction) {
	if (!this->doesCurrentPieceExist || direction == Direction::NONE) {
		return false;
	}

	sf::Vector2i newCenter(
		this->currentPieceCenter.x + int32_t(direction),
		this->currentPieceCenter.y
	);
	bool isPositionCorrect = this->isCenterPositionCorrect(newCenter);
	bool isCollisionFound = doesPieceCollide(this->currentPiece.getCurrentMatrix(), newCenter);
	if (isPositionCorrect & !isCollisionFound) {
		this->currentPieceCenter = newCenter;
		return true;
	}
	return false;
}



bool GameField::rotateFigure(Rotation rotation) {
	if (!this->doesCurrentPieceExist) {
		return false;
	}

	bool isCollisionFound;

	switch (rotation)
	{
	case Rotation::CLOCKWISE :
		this->currentPiece.switchToNextMatrix();
		isCollisionFound = doesPieceCollide(
			this->currentPiece.getCurrentMatrix(),
			this->currentPieceCenter
		);
		if (isCollisionFound) {
			this->currentPiece.switchToPreviousMatrix();
			return false;
		}
		return true; // We don't need break here.

	case Rotation::COUNTERCLOCKWISE :
		this->currentPiece.switchToPreviousMatrix();
		isCollisionFound = doesPieceCollide(
			this->currentPiece.getCurrentMatrix(),
			this->currentPieceCenter
		);
		if (isCollisionFound) {
			this->currentPiece.switchToNextMatrix();
			return false;
		}
		return true; // We don't need break here.
	default:
		return false;
	}
}



void GameField::dropFigureDown(bool isSoftDrop) {
	if (!this->doesCurrentPieceExist) {
		return;
	}

	sf::Vector2i newCenter(
		this->currentPieceCenter.x,
		this->currentPieceCenter.y + 1
	);
	bool isPositionCorrect = this->isCenterPositionCorrect(newCenter);
	bool isCollisionFound = doesPieceCollide(this->currentPiece.getCurrentMatrix(), newCenter);
	if (isPositionCorrect & !isCollisionFound) {
		this->currentPieceCenter = newCenter;
	}
	else {
		this->placeCurrentPiece();
	}
}



void GameField::clear() {
	this->field = std::vector<std::vector<Tetrimino::Type>>(
		FIELD_Y_SIZE, std::vector<Tetrimino::Type>(FIELD_X_SIZE, Tetrimino::Type::E)
		);
	this->doesCurrentPieceExist = false;
	this->canClearLines = false;

	this->lines = 0;
	this->tetrises = 0;
	this->burn = 0;
	this->tetrisRate = 0;
	this->drought = 0;

	this->level = 0;

	this->piecesAmount = std::vector<int32_t>(this->DIFFERENT_PIECES, 0);

	this->prepareNextPiece();
}



bool GameField::spawnNewFigure() {
	if (this->doesCurrentPieceExist) {
		return false;
	}

	if (this->nextPiece == this->piece_I) {
		this->drought = 0;
	}
	else {
		++this->drought;
	}

	this->currentPiece = this->nextPiece;
	this->currentPieceCenter = this->PIECE_SPAWN;
	this->doesCurrentPieceExist = true;
	this->currentPieceType = this->nextPieceType;

	++this->piecesAmount[int32_t(this->currentPieceType)];

	this->prepareNextPiece();

	bool collides = this->doesPieceCollide(
		this->currentPiece.getCurrentMatrix(),
		this->currentPieceCenter
	);
	return !collides;


}


void GameField::checkFullLines() {
	if (this->linesToClear.size() != 0) {
		return;
	}

	int32_t startPos = this->currentPieceCenter.y - Tetrimino::Matrix::CENTER.y;
	int32_t endPos = this->currentPieceCenter.y + Tetrimino::Matrix::CENTER.y;
	for (int32_t y = startPos; y <= endPos; ++y) {
		if (y >= 0 && y < FIELD_Y_SIZE) {
			bool isLineFull = true;
			for (int32_t x = 0; x < this->FIELD_X_SIZE; ++x) {
				if (this->field[y][x] == Tetrimino::Type::E) {
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
			this->field[y] = this->field[y - 1];
		}
	}


	this->updateStatistics();


	this->linesToClear.clear();
	this->canClearLines = false;
}


#pragma /* Controls */ endregion



#pragma region Accessing Attributes


#pragma region Blocks And Lines


const std::vector<std::vector<Tetrimino::Type>>& GameField::getBlocks() const {
	return this->field;
}



bool GameField::doesActivePieceExist() const {
	return this->doesCurrentPieceExist;
}



const sf::Vector2i& GameField::getCurrentPieceCenter() const {
	return this->currentPieceCenter;
}



const Tetrimino::Matrix::Array& GameField::getCurrentPieceMatrix() const {
	return this->currentPiece.getCurrentMatrix();
}



Piece GameField::getCurrentPieceType() const {
	return this->currentPieceType;
}



const Tetrimino::Matrix::Array& GameField::getNextPieceMatrix() const {
	return this->nextPiece.getCurrentMatrix();
}



Piece GameField::getNextPieceType() const {
	return this->nextPieceType;
}



const std::vector<int32_t>& GameField::getLinesToClear() const {
	return this->linesToClear;
}


#pragma /* Blocks And Lines */ endregion



#pragma region Statistics


void GameField::setLevel(int32_t level) {
	this->level = level;
}



int32_t GameField::getLevel() const {
	return this->level;
}



int32_t GameField::getLines() const {
	return this->lines;
}



int32_t GameField::getTetrises() const {
	return this->tetrises;
}



int32_t GameField::getBurn() const {
	return this->burn;
}



int32_t GameField::getTetrisRate() const {
	return this->tetrisRate;
}



int32_t GameField::getDrought() const {
	return this->drought;
}



int32_t GameField::getScore() const {
	return this->score;
}



int32_t GameField::getTopScore() const {
	return this->topScore;
}



void GameField::setTopScore(int32_t topScore) {
	if (topScore > this->MAX_SCORE) {
		topScore = this->MAX_SCORE;
	}
	else if (topScore < 0) {
		topScore = 0;
	}
	this->topScore = topScore;
}



int32_t GameField::getPieceAmount(Piece piece) const {
	return this->piecesAmount[int32_t(piece)];
}



const Tetrimino::Matrix::Array* GameField::getPieceMatrix(Piece piece) const {
	return &this->PIECES_LIST[int32_t(piece)]->getCurrentMatrix();
}


#pragma /* Statistics */ endregion


#pragma /* Accessing Attributes */ endregion



#pragma region Collision And Management


void GameField::clearUpperLines() {
	for (size_t y = 0; y < this->UPPER_LINES_TO_CLEAR; ++y) {
		for (size_t x = 0; x < this->FIELD_X_SIZE; ++x) {
			this->field[y][x] = Tetrimino::Type::E;
		}
	}
}



void GameField::prepareNextPiece() {
	size_t type = Helper::getRandomNumber(0, this->PIECES_LIST.size() - 1);
	this->nextPiece = *(this->PIECES_LIST[type]);
	this->nextPieceType = Piece(type);
}



void GameField::placeCurrentPiece() {
	sf::Vector2i fieldStartPosition = currentPieceCenter - Tetrimino::Matrix::CENTER;

	const auto& matrix = this->currentPiece.getCurrentMatrix();

	for (int32_t y = 0; y < Tetrimino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetrimino::Matrix::SIZE; ++x) {
			auto pos = sf::Vector2i(
				fieldStartPosition.x + x,
				fieldStartPosition.y + y
			);
			if (matrix[y][x] != Tetrimino::Type::E) {
				this->field[pos.y][pos.x] = matrix[y][x];
			}
		}
	}
	this->doesCurrentPieceExist = false;

	this->clearUpperLines();
}



bool GameField::isCenterPositionCorrect(const sf::Vector2i& center) const {
	bool xMinCorrect = center.x >= this->CENTER_X_POSITION_MIN;
	bool xMaxCorrect = center.x <= this->CENTER_X_POSITION_MAX;

	bool yMinCorrect = center.y >= this->CENTER_Y_POSITION_MIN;
	bool yMaxCorrect = center.y <= this->CENTER_Y_POSITION_MAX;

	return xMinCorrect && xMaxCorrect && yMinCorrect && yMaxCorrect;
}



bool GameField::doesPieceCollide(const Tetrimino::Matrix::Array& pieceMatrix,
								 const sf::Vector2i& pieceCenter) const {

	sf::Vector2i fieldStartPosition = pieceCenter - Tetrimino::Matrix::CENTER;

	for (int32_t y = 0; y < Tetrimino::Matrix::SIZE; ++y) {
		for (int32_t x = 0; x < Tetrimino::Matrix::SIZE; ++x) {
			auto posToCheck = sf::Vector2i(
				fieldStartPosition.x + x,
				fieldStartPosition.y + y
			);
			bool xOut = posToCheck.x < 0 || posToCheck.x >= this->FIELD_X_SIZE;
			bool yOut = posToCheck.y < 0 || posToCheck.y >= this->FIELD_Y_SIZE;
			bool currentBlockCollides = pieceMatrix[y][x] != Tetrimino::Type::E;
			if (!(xOut || yOut)) {
				bool collidesWithBlock = this->field[posToCheck.y][posToCheck.x] != Tetrimino::Type::E;
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


#pragma /* Collision And Management */ endregion


#pragma region Statistics


void GameField::updateStatistics() {
	if (this->linesToClear.size() == this->LINES_PER_TETRIS) {
		++this->tetrises;
		this->burn = 0;
	}
	else {
		this->burn += this->linesToClear.size();
	}
	this->lines += this->linesToClear.size();
	this->tetrisRate = 100 * (this->tetrises * this->LINES_PER_TETRIS) / this->lines;


	// Coefficients for formula. Scoring depends on lines burnt.
	static std::vector<int32_t> coefficients = {
		40,
		100,
		300,
		1200,
		0
	};
	this->score += coefficients[this->linesToClear.size() - 1] * (this->level + 1);
	if (this->score > this->MAX_SCORE) {
		this->score = this->MAX_SCORE;
	}
	if (this->score >= this->topScore) {
		this->topScore = this->score;
	}


	int32_t newLevel = lines / this->LINES_PER_LEVEL;
	this->level = newLevel > this->level ? newLevel : this->level;
	if (this->level > this->MAX_LEVEL) {
		this->level = this->MAX_LEVEL;
	}
}


#pragma /* Statistics */ endregion



#pragma region Tetriminoes


void GameField::initializePieces() {
	this->initializePiece_T();
	this->initializePiece_J();
	this->initializePiece_Z();
	this->initializePiece_O();
	this->initializePiece_S();
	this->initializePiece_L();
	this->initializePiece_I();

	this->PIECES_LIST = {
		&this->piece_T,
		&this->piece_J,
		&this->piece_Z,
		&this->piece_O,
		&this->piece_S,
		&this->piece_L,
		&this->piece_I
	};
}



void GameField::initializePiece_T() {
	using T = Tetrimino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::W, T::W, T::W, T::E,
		T::E, T::E, T::W, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetrimino::Matrix matrix(array);
	Tetrimino::Matrix::Array matrixArray = matrix.getArray();

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
	using T = Tetrimino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::B, T::B, T::B, T::E,
		T::E, T::E, T::E, T::B, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetrimino::Matrix matrix(array);
	Tetrimino::Matrix::Array matrixArray = matrix.getArray();

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
	using T = Tetrimino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::D, T::D, T::E, T::E,
		T::E, T::E, T::D, T::D, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetrimino::Matrix matrix(array);
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
	using T = Tetrimino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::W, T::W, T::E, T::E,
		T::E, T::W, T::W, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetrimino::Matrix matrix(array);
	this->piece_O.addMatrix(matrix);
}



void GameField::initializePiece_S() {
	using T = Tetrimino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::B, T::B, T::E,
		T::E, T::B, T::B, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetrimino::Matrix matrix(array);
	this->piece_S.addMatrix(matrix); // S horizontal.

	array = {
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::B, T::E, T::E,
		T::E, T::E, T::B, T::B, T::E,
		T::E, T::E, T::E, T::B, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	matrix.setArray(array);
	this->piece_S.addMatrix(matrix); // S vertical.
}



void GameField::initializePiece_L() {
	using T = Tetrimino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::D, T::D, T::D, T::E,
		T::E, T::D, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetrimino::Matrix matrix(array);
	Tetrimino::Matrix::Array matrixArray = matrix.getArray();

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
	using T = Tetrimino::Type;
	std::vector<T> array{
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::W, T::W, T::W, T::W, T::E,
		T::E, T::E, T::E, T::E, T::E,
		T::E, T::E, T::E, T::E, T::E,
	};
	Tetrimino::Matrix matrix(array);
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


#pragma /* Tetriminoes */ endregion
