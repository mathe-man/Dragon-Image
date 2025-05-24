#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include "File.h"

class DrimFile {
public:
	static DrimFile* CreateDrimFile_ptr(std::string path, uint16_t x_size, uint16_t y_size);
	static DrimFile CreateDrimFile(std::string path, uint16_t x_size, uint16_t y_size);

	static DrimFile* OpenDrimFile_ptr(std::string path);
	static DrimFile OpenDrimFile(std::string path);

	File* file;
	
	void SetVersion(uint8_t global_version, uint8_t subversion);
	void SetDimensions(uint16_t x_size, uint16_t y_size);

	float GetVersion();
	int GetGlobalVersion();
	int GetSubVersion();


	void WriteTag(std::vector<uint8_t> identifier, std::vector<uint8_t> value);
	void WriteTag(std::vector<uint8_t> identifier, uint8_t value);
	void WriteTag(uint8_t identifier, std::vector<uint8_t> value);
	void WriteTag(uint8_t identifier, uint8_t value);

	void AddToPixelSection(std::vector<uint8_t> value);
	void AddToPixelSection(uint8_t value);
	
	int GetPixelSectionStartIndex();
	int GetPixelSectionEndIndex();

	std::vector<uint8_t> GetPixelSectionContent()
	{
		return file->Get(GetPixelSectionStartIndex(), GetPixelSectionEndIndex() - GetPixelSectionStartIndex());
	}

	DrimFile();
	~DrimFile();

private:
	DrimFile(File* _file);
	
};