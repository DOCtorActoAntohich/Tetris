#include "tetris/Record.h"

#include <fstream>

#include "tetris/helper/Helper.h"

using namespace tetris;



Record::Record() {
	this->clear();
}

Record::Record(std::string name, uint32_t score, uint8_t level) {
	this->setName(name);
	this->setScore(score);
	this->setLevel(level);
}



std::string Record::getName() {
	return helper::replace(this->name, "_", " ");
}



void Record::setName(std::string name) {
	if (name.size() < this->NAME_LENGTH) {
		std::string underscores(this->NAME_LENGTH - name.size(), '_');
		name += underscores;
	}
	else if (name.size() > this->NAME_LENGTH) {
		name = name.substr(0, this->NAME_LENGTH);
	}
	this->name = helper::replace(name, " ", "_");
}



uint32_t Record::getScore() {
	return this->score;
}



void Record::setScore(uint32_t score) {
	if (score >= this->MAX_SCORE) {
		score = this->MAX_SCORE;
	}
	this->score = score;
}



uint8_t Record::getLevel() {
	return this->level;
}



void Record::setLevel(uint8_t level) {
	if (level >= this->MAX_LEVEL) {
		level = this->MAX_LEVEL;
	}
	this->level = level;
}



void Record::clear() {
	this->setName("PERSON");
	this->setScore(uint32_t(0));
	this->setLevel(uint8_t(0));
}



std::ostream& tetris::operator<<(std::ostream& output, Record& record) {
	output	<<  record.name  << " "
			<< ~record.score << " "
			<< ~uint32_t(record.level);
	return output;
}



std::istream& tetris::operator>>(std::istream& input, Record& record) {
	std::string name;
	uint32_t score;
	uint32_t level;
	input >> name >> score >> level;
	record.setName(helper::toUpperCase(name));
	record.setScore(~score);
	record.setLevel(~static_cast<uint8_t>(level));
	return input;
}
