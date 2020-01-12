#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>

#include "Tetromino.h"

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
		static const int32_t TETRIS_LINES = 4;

		GameField();

		// Moves figure 1 block left of right.
		void moveFigure(Direction direction);

		// Rotates figure clockwise or counterclockwise.
		void rotateFigure(Rotation rotation);
		
		// Moves figure 1 block down.
		void dropFigureDown();
		const std::vector<std::vector<Tetromino::Type>>& getBlocks() const;

		void clear();
		void clearUpperLines();

		// Spawns new figure.
		// Returns true is it is possible, false is figure collides on spawn.
		bool spawnNewFigure();

		bool doesActivePieceExist() const;

		const Tetromino::Matrix::Array& getCurrentPieceMatrix() const;
		const sf::Vector2i& getCurrentPieceCenter() const;

		void checkFullLines();
		void clearLines();
		std::vector<int32_t> getLinesToClear();

		static const int32_t UPPER_LINES_TO_CLEAR = 2;

		static const int32_t FIELD_X_SIZE = 10;
		static const int32_t FIELD_Y_SIZE = 22;
	private:
		std::vector<std::vector<Tetromino::Type>> game_field;


		static const int32_t CENTER_X_POSITION_MIN = -2;
		static const int32_t CENTER_X_POSITION_MAX = FIELD_X_SIZE + 2;
		static const int32_t CENTER_Y_POSITION_MIN = -2;
		static const int32_t CENTER_Y_POSITION_MAX = FIELD_Y_SIZE + 2;

		


		bool currentPieceExists;
		Tetromino currentPiece;
		sf::Vector2i currentPieceCenter;
		bool isCenterPositionCorrect(const sf::Vector2i& center) const;
		bool doesFigureCollide(
			const Tetromino::Matrix::Array& figureMatrix,
			const sf::Vector2i& figureCenter
		) const;

		void placeCurrentPiece();

		std::vector<int32_t> linesToClear;
		bool canClearLines;

		Tetromino piece_T;
		Tetromino piece_J;
		Tetromino piece_Z;
		Tetromino piece_O;
		Tetromino piece_S;
		Tetromino piece_L;
		Tetromino piece_I;
		void initializePiece_T();
		void initializePiece_J();
		void initializePiece_Z();
		void initializePiece_O();
		void initializePiece_S();
		void initializePiece_L();
		void initializePiece_I();

		void initializePieces();

		// Coordinates of every new figure matrix center.
		const sf::Vector2i FIGURE_SPAWN = sf::Vector2i(5, 2);
	};
}
