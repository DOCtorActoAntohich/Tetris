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

		// Writes given bytes to a file.
		static void writeToBinaryFile(const std::filesystem::path& path, const std::vector<byte>& bytes);

		static void deleteFile(const std::filesystem::path& path);

		static void createDirectory(const std::filesystem::path& path);
	private:
		Helper();
	};
}