#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <ResourceIndexer.h>
#include <ShlObj.h>

namespace tetris::helper {
	// Loads resources embedded into EXE.
	std::vector<byte> loadEmbeddedResource(int32_t id);

	// Returns expected path like "C:\ProgramData".
	std::filesystem::path getProgramDataPath();

	// Returns path to a game folder (in ProgramData).
	std::filesystem::path getGameFolderPath();

	// Writes given bytes to a file.
	void writeToBinaryFile(const std::filesystem::path& path, const std::vector<byte>& bytes);

	// Removes file from a disk.
	void deleteFile(const std::filesystem::path& path);

	// Creates new directories (recursively).
	void createDirectory(const std::filesystem::path& path);

	// Replaces substring in a string.
	std::string replace(std::string string, const std::string& from, const std::string& to);

	// Transforms string to upper case.
	std::string toUpperCase(std::string string);

	// Counts how many digits are there in number.
	size_t countDigits(uint32_t number);

	int32_t mod(int32_t number, int32_t module);

	template <class ArrayType>
	void rotateArray(ArrayType& array) {
		size_t n = array.size();
		size_t x = n / 2;
		size_t y = n - 1;
		for (size_t i = 0; i < x; ++i) {
			for (size_t j = i; j < y - i; ++j) {
				auto k = array[i][j];
				array[i][j] = array[y - j][i];
				array[y - j][i] = array[y - i][y - j];
				array[y - i][y - j] = array[j][y - i];
				array[j][y - i] = k;
			}
		}
	}

	int32_t getRandomNumber(int32_t lowerBound, int32_t upperBound);
}