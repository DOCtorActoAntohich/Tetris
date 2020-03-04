#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <array>


namespace tetris::scene_handling {
	class Tetrimino {
	public:
		enum class Type : int32_t {
			// Empty block (does not collide).
			E = -1,

			// First color (for J and S pieces).
			B = 0,

			// Second color (for L and Z pieces).
			D = 1,

			// White color (for O, T and I pieces).
			W = 2,
		};



		class Matrix {
		public:
			static const size_t SIZE = 5;
			static sf::Vector2i CENTER;

			using Array = std::array<std::array<Type, SIZE>, SIZE>;

			static const Array EMPTY;

			// Constructs empty matrix.
			Matrix();

			// Constructs matrix from 1 dimensional array on integers.
			// Transforms given numbers into Tetrimino::Type.
			Matrix(std::vector<Type>& matrix);

			// Constructs matrix from 1 dimensional array on integers.
			// Transforms given numbers into Tetrimino::Type.
			Matrix(Array& matrix);

			// Transforms given 1D array into 2D matrix
			void setArray(std::vector<Type>& array);

			void setArray(Array& array);

			// Returns curent matrix;
			const Array& getArray() const;


			friend bool operator==(const Matrix& firstMatrix, const Matrix& secondMatrix);
			friend bool operator!=(const Matrix& firstMatrix, const Matrix& secondMatrix);
		private:
			Array matrix;
		};


		static const size_t MAX_MATRICES = 4;

		Tetrimino();

		// Adds given matrix to the list of matrices.
		// If given matrix exceeds capacity of internal storage, this matrix is ignored.
		void addMatrix(std::vector<Type>& array);

		// Adds given matrix to the list of matrices.
		// If given matrix exceeds capacity of internal storage, this matrix is ignored.
		void addMatrix(Matrix& array);

		// Adds given matrix to the list of matrices.
		// If given matrix exceeds capacity of internal storage, this matrix is ignored.
		void addMatrix(Matrix::Array& array);

		// Removes matrix of given index from a list.
		// Next matrices are shifted to the left.
		// If index if greater than MAX_MATRICES, this function will not cause effects.
		void removeMatrix(size_t index);

		// Switches current active matrix to the next one. Matrices are cycled.
		void switchToNextMatrix();

		// Switches current active matrix to the previous one. Matrices are cycled.
		void switchToPreviousMatrix();

		// Returns the current matrix of tetrimino.
		const Matrix::Array& getCurrentMatrix() const;

		Tetrimino& operator=(const Tetrimino& otherTetrimino);
		friend bool operator==(const Tetrimino& firstTetrimino, const Tetrimino& secondTetrimino);
		friend bool operator!=(const Tetrimino& firstTetrimino, const Tetrimino& secondTetrimino);
	private:
		std::vector<Matrix> matrices;
		size_t currentMatrixIndex;
	};

	
}