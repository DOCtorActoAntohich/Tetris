#include "tetris/Tetrimino.h"

#include "tetris/helper/Helper.h"

using namespace tetris;


#pragma region Tetrimino Matrix


sf::Vector2i Tetrimino::Matrix::CENTER(2, 2);

const Tetrimino::Matrix::Array Tetrimino::Matrix::EMPTY = {
	Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E,
	Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E,
	Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E,
	Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E,
	Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E, Tetrimino::Type::E
};



Tetrimino::Matrix::Matrix() {
	this->matrix = std::array<std::array<Type, SIZE>, SIZE>();
	for (size_t y = 0; y < this->SIZE; ++y) {
		for (size_t x = 0; x < this->SIZE; ++x) {
			this->matrix[y][x] = Type::E;
		}
	}
}

Tetrimino::Matrix::Matrix(std::vector<Type>& matrix) {
	this->matrix = std::array<std::array<Type, SIZE>, SIZE>();
	this->setArray(matrix);
	
}



tetris::Tetrimino::Matrix::Matrix(Array& matrix) {
	this->matrix = matrix;
}



void Tetrimino::Matrix::setArray(std::vector<Type>& array) {
	for (size_t y = 0; y < this->SIZE; ++y) {
		for (size_t x = 0; x < this->SIZE; ++x) {
			size_t index = this->SIZE * y + x;
			if (index < array.size()) {
				this->matrix[y][x] = array[index];
			}
			else {
				this->matrix[y][x] = Type::E;
			}
		}
	}
}



void Tetrimino::Matrix::setArray(Array& array) {
	this->matrix = array;
}



const Tetrimino::Matrix::Array& Tetrimino::Matrix::getArray() const{
	return this->matrix;
}


#pragma /* Tetrimino Matrix */ endregion



#pragma region Tetrimino



Tetrimino::Tetrimino() {
	this->currentMatrixIndex = 0;
}



void Tetrimino::addMatrix(std::vector<Type>& array) {
	if (this->matrices.size() < this->MAX_MATRICES) {
		Matrix matrix(array);
		this->matrices.push_back(matrix);
	}
}



void Tetrimino::addMatrix(Matrix& matrix) {
	if (this->matrices.size() < this->MAX_MATRICES) {
		this->matrices.push_back(matrix);
	}
}



void Tetrimino::addMatrix(Matrix::Array& array) {
	if (this->matrices.size() < this->MAX_MATRICES) {
		Matrix matrix(array);
		this->matrices.push_back(matrix);
	}
}



void Tetrimino::removeMatrix(size_t index) {
	if (index < this->matrices.size()) {
		this->matrices.erase(this->matrices.begin() + index);
	}

	if (this->currentMatrixIndex > index) {
		--this->currentMatrixIndex;
	}
}



void Tetrimino::switchToNextMatrix() {
	this->currentMatrixIndex = helper::mod(
		this->currentMatrixIndex + 1, this->matrices.size()
	);
}

void Tetrimino::switchToPreviousMatrix() {
	this->currentMatrixIndex = helper::mod(
		this->currentMatrixIndex - 1, this->matrices.size()
	);
}


const Tetrimino::Matrix::Array& Tetrimino::getCurrentMatrix() const {
	return this->matrices[this->currentMatrixIndex].getArray();
}



Tetrimino& Tetrimino::operator=(const Tetrimino& otherTetrimino) {
	this->matrices = otherTetrimino.matrices;
	this->currentMatrixIndex = 0;
	return *this;
}



bool tetris::operator==(const Tetrimino::Matrix& firstMatrix, const Tetrimino::Matrix& secondMatrix) {
	return firstMatrix.matrix == secondMatrix.matrix;
}



bool tetris::operator!=(const Tetrimino::Matrix& firstMatrix, const Tetrimino::Matrix& secondMatrix) {
	return firstMatrix.matrix != secondMatrix.matrix;
}



bool tetris::operator==(const Tetrimino& firstTetrimino, const Tetrimino& secondTetrimino) {
	return firstTetrimino.matrices == secondTetrimino.matrices;
}



bool tetris::operator!=(const Tetrimino& firstTetrimino, const Tetrimino& secondTetrimino) {
	return firstTetrimino.matrices != secondTetrimino.matrices;
}


#pragma /* Tetrimino */ endregion
