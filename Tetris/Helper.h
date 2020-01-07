#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <ResourceIndexer.h>
#include <ShlObj.h>

namespace tetris {
	class Helper {
	public:
		// Loads resources embedded into EXE.
		static std::vector<byte> loadEmbeddedResource(int32_t id);

		// Returns expected path like "C:\ProgramData".
		static std::filesystem::path getProgramDataPath();

		// Returns path to a game folder (in ProgramData).
		static std::filesystem::path getGameFolderPath();

		// Writes given bytes to a file.
		static void writeToBinaryFile(const std::filesystem::path& path, const std::vector<byte>& bytes);

		// Removes file from a disk.
		static void deleteFile(const std::filesystem::path& path);

		// Creates new directories (recursively).
		static void createDirectory(const std::filesystem::path& path);

		// Replaces substring in a string.
		static std::string replace(std::string string, const std::string& from, const std::string& to);

		// Transforms string to upper case.
		static std::string tuUpperCase(std::string string);

		// Counts how many digits are there in number.
		static size_t countDigits(uint32_t number);
	private:
		// Haha, this class is static.
		Helper();
	};
}