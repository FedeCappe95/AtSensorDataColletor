#ifndef __GICAME__IO_H__
#define __GICAME__IO_H__


#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include "../common.h"


namespace Gicame::IO {

	using IoErrorEventHandler = std::function<void(const std::string&, const std::fstream*)>;

	static void standardIoErrorEventHandler(const std::string& filePath, const std::fstream*) {
		throw RUNTIME_ERROR(std::string("Gicame::IO IO error on file") + filePath);
	}

	static inline size_t fileSize(const std::string& filePath) {
		std::ifstream inFile(filePath, std::ifstream::ate | std::ifstream::binary);
		const size_t size = static_cast<size_t>(inFile.tellg());
		inFile.close();
		return size;
	}

    [[maybe_unused]] static std::vector<byte_t> readFileContent(
		const std::string& filePath,
		const IoErrorEventHandler& ioErrorEventHandler = standardIoErrorEventHandler
	) {
		std::fstream inFile;
		inFile.open(filePath, std::ifstream::ate | std::ios::binary | std::ios::in);
		if (!inFile.is_open()) {
			ioErrorEventHandler(filePath, &inFile);
			return std::vector<byte_t>();
		}
		const size_t size = static_cast<size_t>(inFile.tellg());
		inFile.seekg(0, std::ios_base::beg);
		std::vector<byte_t> content(size);
		inFile.read((char*)content.data(), size);
		inFile.close();
		return content;
	}

	[[maybe_unused]] static void writeFileContent(
		const std::string& filePath,
		const std::vector<byte_t>& content,
		const IoErrorEventHandler& ioErrorEventHandler = standardIoErrorEventHandler
	) {
		std::fstream outFile;
		outFile.open(filePath, std::ios::binary | std::ios::out);
		if (!outFile.is_open()) {
			ioErrorEventHandler(filePath, &outFile);
		}
		outFile.write((const char*)content.data(), content.size());
		outFile.close();
	}

};


#endif
