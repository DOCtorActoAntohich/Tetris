#include "Helper.h"

#include <fstream>
#include <random>

using namespace tetris;



std::vector<byte> helper::loadEmbeddedResource(int32_t id) {
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



std::filesystem::path helper::getProgramDataPath() {
	PWSTR pathPointer;
	SHGetKnownFolderPath(FOLDERID_ProgramData, 0, 0, &pathPointer);
	std::wstring pathString(pathPointer);
	CoTaskMemFree(static_cast<void*>(pathPointer));
	return std::filesystem::path(pathString);
}



std::filesystem::path helper::getGameFolderPath() {
	static const std::string GAME_DATA_FOLDER = "Tetris by DOCtorActoAntohich";
	auto path = helper::getProgramDataPath();
	path /= GAME_DATA_FOLDER;
	return path;
}



void helper::writeToBinaryFile(const std::filesystem::path& path, const std::vector<byte>& bytes) {
	std::ofstream file(path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
	file.write((char*)&bytes[0], bytes.size());
	file.close();
}



void helper::deleteFile(const std::filesystem::path& path) {
	std::filesystem::remove(path);
}



void helper::createDirectory(const std::filesystem::path& path) {
	std::filesystem::create_directories(path);
}



std::string helper::replace(std::string string, const std::string& from, const std::string& to) {
	uint32_t index = 0;
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



std::string helper::toUpperCase(std::string string) {
	for (auto& chr : string) {
		chr = std::toupper(chr);
	}
	return string;
}



uint32_t helper::countDigits(uint32_t number) {
	uint32_t digits = 0;
	while (number != 0) {
		number /= 10;
		++digits;
	}
	return digits;
}



int32_t helper::mod(int32_t number, int32_t module)
{
	return ((number % module) + module) % module;
}



int32_t helper::getRandomNumber(int32_t lowerBound, int32_t upperBound) {
	if (lowerBound < 0) {
		lowerBound = 0;
	}
	if (upperBound < 0) {
		upperBound = 0;
	}

	static std::random_device devive;
	static std::mt19937 rng(devive());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(lowerBound, upperBound);

	return distribution(rng);
}
