#include "Score.h"

#include <filesystem>
#include <fstream>

#include "Helper.h"

using namespace tetris;



void Score::setDefaultScores() {
	this->records = std::list<Record>{
		Record("HOWARD", 10000, uint8_t(9)),
		Record("OTASAN",  7500, uint8_t(5)),
		Record("LANCE",   5000, uint8_t(0))
	};
}



void Score::loadFromFile(std::string fileName) {
	std::filesystem::path path = Helper::getGameFolderPath();
	path /= fileName;
	std::list<Record> records;
	std::ifstream in(path);
	if (!in.is_open()) {
		this->setDefaultScores();
		return;
	}
	while (!in.eof() && records.size() < this->RECORDS) {
		Record record;
		if (in >> record) {
			records.push_back(record);
		}
	}
	in.close();
	this->saveData(records);
}



void Score::saveData(std::list<Record>& records)
{
	while (records.size() != 0) {
		Record record = records.front();
		records.pop_front();
		if (this->records.size() == 0 ||
			record.getScore() <= this->records.back().getScore()) {
			this->records.push_back(record);
		}
		else {
			this->setDefaultScores();
			break;
		}
	}
}



void Score::saveToFile(std::string fileName) {
	std::filesystem::path path = Helper::getGameFolderPath();
	path /= fileName;
	std::ofstream out(path, std::ofstream::trunc);
	for (auto record : this->records) {
		out << record << "\n";
	}
	out.close();
}



void Score::addPoints(uint32_t points) {
	uint32_t newScore = this->currentRecord.getScore() + points;
	this->currentRecord.setScore(newScore);
}



uint32_t Score::getCurrentScore() {
	return this->currentRecord.getScore();
}



void Score::saveScore(std::string name, uint8_t level) {
	this->currentRecord.setName(name);
	this->currentRecord.setLevel(level);
	this->insertRecord();
	this->currentRecord.clear();
}



void Score::insertRecord() {
	for (auto i = this->records.begin(); i != this->records.end(); ++i) {
		if (this->currentRecord.getScore() >= (*i).getScore()) {
			this->records.insert(i, this->currentRecord);
			break;
		}
	}
	this->records.pop_back();
}
