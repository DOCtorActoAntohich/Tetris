#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>

#include "Tetrimino.h"

namespace tetris {
	enum class Rotation : int32_t {
		CLOCKWISE = 1,
		COUNTERCLOCKWISE = -1
	};



	enum class Direction : int32_t {
		LEFT = -1,
		RIGHT = 1,

		NONE = 0
	};



	enum class Piece {
		T = 0,
		J = 1,
		Z = 2,
		O = 3,
		S = 4,
		L = 5,
		I = 6
	};



	class GameField {
	public:
#pragma region Constants

		static const int32_t FIELD_X_SIZE = 10;
		static const int32_t FIELD_Y_SIZE = 22;

		static const int32_t MAX_LINES = 999;
		static const int32_t MAX_TETRISES = 999;
		static const int32_t MAX_BURN = 999;
		static const int32_t MAX_TETRIS_RATE = 100;
		static const int32_t MAX_DROUGHT = 99;
		static const int32_t MAX_PIECE_AMOUNT = 999;

		static const int32_t UPPER_LINES_TO_CLEAR = 2;
		static const int32_t LINES_PER_TETRIS = 4;
		static const int32_t DIFFERENT_PIECES = 7;

#pragma /* Constants */ endregion

		GameField();

#pragma region Controls

		// Moves figure 1 block left of right.
		// Returns true if successfully moved.
		bool moveFigure(Direction direction);

		// Rotates figure clockwise or counterclockwise.
		// Returns true if successfully rotated.
		bool rotateFigure(Rotation rotation);
		
		// Moves piece 1 block down.
		// If piece is being soft dropped, score is increased.
		void dropFigureDown(bool isSoftDrop = false);

		// Clears the field from any blocks and resets counters.
		void clear();

		// Spawns new figure.
		// Returns true is it is possible, false is figure collides on spawn.
		bool spawnNewFigure();

		// Checks if there are lines to be cleared.
		// Has no effect if lines are not cleared previously.
		void checkFullLines();

		// Clears lines that are marked to be cleared.
		// Call checkFullLines() before cleaning lines.
		// If no lines to clear, it will not cause effects.
		void clearLines();

#pragma /* Controls */ endregion



#pragma region Accessing Attributes


#pragma region Blocks And Lines

		// Returns the game field.
		const std::vector<std::vector<Tetrimino::Type>>& getBlocks() const;

		// Returns true if piece was spawned.
		bool doesActivePieceExist() const;

		// Returns the position of piece center.
		const sf::Vector2i& getCurrentPieceCenter() const;

		// Returns the matrix of the current piece.
		const Tetrimino::Matrix::Array& getCurrentPieceMatrix() const;

		// Returns the type (Letter) of the current piece.
		Piece getCurrentPieceType() const;

		// Returns the matrix of the next piece.
		const Tetrimino::Matrix::Array& getNextPieceMatrix() const;

		// Returns the type (Letter) of the next piece.
		Piece getNextPieceType() const;

		// Returns the indexes of lines to be cleared.
		const std::vector<int32_t>& getLinesToClear() const;

#pragma /* Blocks And Lines */ endregion



#pragma region Statistics

		// Returns how many lines are cleared.
		int32_t getLines() const;

		// Returns how many tetrises are cleared.
		int32_t getTetrises() const;

		// Returns how many lines (not tetrises) are cleared since last tetris.
		int32_t getBurn() const;

		// Returns the percentage of tetrises.
		int32_t getTetrisRate() const;

		// Returns for how many pieces there were not I piece.
		int32_t getDrought() const;

		// Returns the amount of specified pieces.
		int32_t getPieceAmount(Piece piece) const;

		// Returns the matrix of a specified piece.
		const Tetrimino::Matrix::Array* getPieceMatrix(Piece piece) const;

#pragma /* Statistics */ endregion


#pragma /* Accessing Attributes */ endregion
		

///////////////////////////////////////////////////////////////////////////////


	private:
		// Game field containing static blocks.
		std::vector<std::vector<Tetrimino::Type>> field;

#pragma region Collision And Management

		// Coordinates of every new figure matrix center.
		const sf::Vector2i PIECE_SPAWN = sf::Vector2i(5, 2);

		static const int32_t CENTER_X_POSITION_MIN = -2;
		static const int32_t CENTER_X_POSITION_MAX = FIELD_X_SIZE + 2;
		static const int32_t CENTER_Y_POSITION_MIN = -2;
		static const int32_t CENTER_Y_POSITION_MAX = FIELD_Y_SIZE + 2;

		bool canClearLines;

		std::vector<int32_t> linesToClear;

		// Clears upper lines to prevent collision on spawn.
		void clearUpperLines();



		Tetrimino nextPiece;
		Piece nextPieceType;

		// Randomly chooses next piece to be spawned.
		// Does not spawn the piece.
		void prepareNextPiece();



		bool doesCurrentPieceExist;
		Piece currentPieceType;
		Tetrimino currentPiece;
		sf::Vector2i currentPieceCenter;

		// Places current piece to a field.
		void placeCurrentPiece();



		// Checks if position of given center is correct.
		bool isCenterPositionCorrect(const sf::Vector2i& center) const;

		// Returns true if piece with given matrix and center collides with something
		bool doesPieceCollide(const Tetrimino::Matrix::Array& pieceMatrix,
							   const sf::Vector2i& pieceCenter) const;

#pragma /* Collision And Management */

		
#pragma region Statistics

		int32_t lines;
		int32_t tetrises;
		int32_t burn;
		int32_t tetrisRate;
		int32_t drought;

		std::vector<int32_t> piecesAmount;

#pragma /* Statistics */ endregion 
		

#pragma region Tetriminoes

		// Contains pointers to main pieces.
		std::vector<Tetrimino*> PIECES_LIST;

		// Initializes all pieces by assigning matrices.
		void initializePieces();

		Tetrimino      piece_T;
		void initializePiece_T();

		Tetrimino      piece_J;
		void initializePiece_J();

		Tetrimino      piece_Z;
		void initializePiece_Z();

		Tetrimino      piece_O;
		void initializePiece_O();

		Tetrimino      piece_S;
		void initializePiece_S();

		Tetrimino      piece_L;
		void initializePiece_L();

		Tetrimino      piece_I;
		void initializePiece_I();

#pragma /* Tetriminoes */ endregion

	};
}
