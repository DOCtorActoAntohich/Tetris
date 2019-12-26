#pragma once

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
}