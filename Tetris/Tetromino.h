#pragma once

#include <vector>
#include <array>


namespace tetris {
	enum class TetrominoType : int32_t {
		// No figure.
		None = 0,

		// Bright figure (for J and S pieces).
		Bright = 1,

		// Dark figure (for L and Z pieces).
		Dark = 2,

		// Neutral white figure (for O, T and I pieces).
		White = 3,
	};

	

	class Tetromino {
	public:
		static const size_t MATRIX_SIZE = 5;
		using Matrix = std::array<std::array<TetrominoType, MATRIX_SIZE>, MATRIX_SIZE>;


		// Adds given matrix to the list of matrices.
		// If given matrix exceeds capacity of internal storage, this matrix is ignored.
		void addMatrix(Matrix matrix);

		// Removes matrix of given index from a list.
		// If index if greater than MAX_MATRICES, this function will not cause effects.
		void removeMatrix(size_t index);

		// Switches current active matrix to the next one. Matrices are cycled.
		void switchToNextMatrix();

		// Switches current active matrix to the previous one. Matrices are cycled.
		void switchToPreviousMatrix();
	private:
		static const size_t MAX_MATRICES = 4;
		std::vector<Matrix> matrices;
		size_t currentMatrixIndex;
	};
}