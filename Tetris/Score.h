#pragma once

#include <string>
#include <vector>
#include <list>

#include "Record.h"

namespace tetris {
	class Score {
	public:
		static const int32_t RECORDS = 3;


		// Creates default table with scores.
		void setDefaultScores();

		// Opens a record file.
		void loadFromFile(std::string path);

		// Checks if data from loaded file is valid,
		// And saves it in memory.
		void saveData(std::list<Record>& records);

		// Saves a record file.
		void saveToFile(std::string path);


		// Adds points to current record.
		void addPoints(uint32_t points);

		// Returns current score.
		uint32_t getCurrentScore();

		// Saves score in table
		void saveScore(std::string name, uint8_t level);
	private:
		std::list<Record> records;
		void insertRecord();

		Record currentRecord;
	};
}