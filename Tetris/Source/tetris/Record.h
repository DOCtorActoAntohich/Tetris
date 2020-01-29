#pragma once

#include <string>

namespace tetris {
	class Record {
	public:
		static const int32_t NAME_LENGTH = 6;
		static const uint32_t MAX_SCORE = 999999;
		static const uint8_t MAX_LEVEL = 99;

		Record();
		Record(std::string name, uint32_t score, uint8_t level);


		std::string getName();
		void setName(std::string name);

		uint32_t getScore();
		void setScore(uint32_t score);

		uint8_t getLevel();
		void setLevel(uint8_t level);

		void clear();
	private:
		std::string name;
		uint32_t score;
		uint8_t level;

		friend std::ostream& operator<<(std::ostream& output, Record& record);
		friend std::istream& operator>>(std::istream& input, Record& record);
	};
}