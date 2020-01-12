#include "Tetromino.h"

#include "Helper.h"

using namespace tetris;


#pragma region Tetromino Matrix


sf::Vector2i Tetromino::Matrix::CENTER(2, 2);

const Tetromino::Matrix::Array Tetromino::Matrix::EMPTY = {
	Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E,
	Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E,
	Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E,
	Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E,
	Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E, Tetromino::Type::E
};



Tetromino::Matrix::Matrix() {
	this->matrix = std::array<std::array<Type, SIZE>, SIZE>();
	for (size_t y = 0; y < this->SIZE; ++y) {
		for (size_t x = 0; x < this->SIZE; ++x) {
			this->matrix[y][x] = Type::E;
		}
	}
}

Tetromino::Matrix::Matrix(std::vector<Type>& matrix) {
	this->matrix = std::array<std::array<Type, SIZE>, SIZE>();
	this->setArray(matrix);
	
}



tetris::Tetromino::Matrix::Matrix(Array& matrix) {
	this->matrix = matrix;
}



void Tetromino::Matrix::setArray(std::vector<Type>& array) {
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



void Tetromino::Matrix::setArray(Array& array) {
	this->matrix = array;
}



const Tetromino::Matrix::Array& Tetromino::Matrix::getArray() const{
	return this->matrix;
}


#pragma /* Tetromino Matrix */ endregion



#pragma region Tetromino



Tetromino::Tetromino() {
	this->currentMatrixIndex = 0;
}



void Tetromino::addMatrix(std::vector<Type>& array) {
	if (this->matrices.size() < this->MAX_MATRICES) {
		Matrix matrix(array);
		this->matrices.push_back(matrix);
	}
}



void Tetromino::addMatrix(Matrix& matrix) {
	if (this->matrices.size() < this->MAX_MATRICES) {
		this->matrices.push_back(matrix);
	}
}



void Tetromino::addMatrix(Matrix::Array& array) {
	if (this->matrices.size() < this->MAX_MATRICES) {
		Matrix matrix(array);
		this->matrices.push_back(matrix);
	}
}



void Tetromino::removeMatrix(size_t index) {
	if (index < this->matrices.size()) {
		this->matrices.erase(this->matrices.begin() + index);
	}

	if (this->currentMatrixIndex > index) {
		--this->currentMatrixIndex;
	}
}



void Tetromino::switchToNextMatrix() {
	this->currentMatrixIndex = Helper::mod(
		this->currentMatrixIndex + 1, this->matrices.size()
	);
}

void Tetromino::switchToPreviousMatrix() {
	this->currentMatrixIndex = Helper::mod(
		this->currentMatrixIndex - 1, this->matrices.size()
	);
}


const Tetromino::Matrix::Array& Tetromino::getCurrentMatrix() const {
	return this->matrices[this->currentMatrixIndex].getArray();
}



Tetromino& Tetromino::operator=(const Tetromino& otherTetromino) {
	this->matrices = otherTetromino.matrices;
	this->currentMatrixIndex = 0;
	return *this;
}


#pragma /* Tetromino */ endregion