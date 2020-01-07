#include "Helper.h"

#include <fstream>


namespace tetris {
	Helper::Helper() { }



	std::vector<byte> Helper::loadEmbeddedResource(int32_t id) {
		// Took code from link bellow and rewrote a bit.
		// https://smacdo.com/programming/embedding-user-defined-resources-in-visual-c-binaries/

		std::vector<byte> output;

		HMODULE module = nullptr;
		LPCTSTR name = MAKEINTRESOURCE(id);
		LPCTSTR type = RT_RCDATA;

		// Raw data is used here because resource fails to be read otherwise.
		HRSRC resourceHandle = ::FindResource(module, name, type);
		if (resourceHandle == nullptr) {
			return output;
		}

		HGLOBAL dataHandle = ::LoadResource(module, resourceHandle);
		if (dataHandle == nullptr) {
			return output;
		}

		DWORD resourceSize = ::SizeofResource(module, resourceHandle);
		if (resourceSize == 0) {
			return output;
		}

		auto firstByte = reinterpret_cast<const char*>(::LockResource(dataHandle));
		if (firstByte == nullptr) {
			return output;
		}

		output.resize(resourceSize);
		std::copy(firstByte, firstByte + resourceSize, output.begin());

		return output;
	}



	std::filesystem::path Helper::getProgramDataPath() {
		PWSTR pathPointer;
		SHGetKnownFolderPath(FOLDERID_ProgramData, 0, 0, &pathPointer);
		std::wstring pathString(pathPointer);
		CoTaskMemFree(static_cast<void*>(pathPointer));
		return std::filesystem::path(pathString);
	}



	std::filesystem::path Helper::getGameFolderPath() {
		static const std::string GAME_DATA_FOLDER = "Tetris by DOCtorActoAntohich";
		auto path = Helper::getProgramDataPath();
		path /= GAME_DATA_FOLDER;
		return path;
	}



	void Helper::writeToBinaryFile(const std::filesystem::path& path, const std::vector<byte>& bytes) {
		std::ofstream file(path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
		file.write((char*)&bytes[0], bytes.size());
		file.close();
	}



	void Helper::deleteFile(const std::filesystem::path& path) {
		std::filesystem::remove(path);
	}



	void Helper::createDirectory(const std::filesystem::path& path) {
		std::filesystem::create_directories(path);
	}



	std::string Helper::replace(std::string string, const std::string& from, const std::string& to) {
		size_t index = 0;
		while (true) {
			index = string.find(from, index);
			if (index == std::string::npos) {
				break;
			}
			string.replace(index, from.size(), to);
			index += to.size();
		}
		return string;
	}



	std::string Helper::tuUpperCase(std::string string) {
		for (auto& chr : string) {
			chr = std::toupper(chr);
		}
		return string;
	}



	size_t Helper::countDigits(uint32_t number) {
		size_t digits = 0;
		while (number != 0) {
			number /= 10;
			++digits;
		}
		return digits;
	}
}