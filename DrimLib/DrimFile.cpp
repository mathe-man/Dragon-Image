#include <fstream>
#include <iostream>
#include <vector>
#include "DrimLib/DrimFile.h"


DrimFile* DrimFile::CreateDrimFile_ptr(std::string path, uint16_t x_size, uint16_t y_size)
{	
	DrimFile* drim_file = new DrimFile(File::CreateFile_ptr(path));
	
	// Add DRIM head signature
	drim_file->file->PushBack({ 0x44, 0x52, 0x49, 0x4D });
	// Add version (0.1)
	drim_file->SetVersion(0x00, 0x01);

	// Set dimensions
	drim_file->SetDimensions(x_size, y_size);
	// Add the TAGS sectionn
	drim_file->file->PushBack({ 0x54, 0x41, 0x47, 0x53 });
	
	// Add pixels section start and end
	drim_file->file->PushBack({ 0x7E, 0x7E, 0x40, 0x40 });
	return drim_file;
}
DrimFile DrimFile::CreateDrimFile(std::string path, uint16_t x_size, uint16_t y_size)
{
	return *CreateDrimFile_ptr(path, x_size, y_size);
}

DrimFile* DrimFile::OpenDrimFile_ptr(std::string path)
{
	File file = File::OpenFile(path);
	// TODO search a cleaner method to compare the signature
	if (file.binairies[0] == 0x44)
		if (file.binairies[1] == 0x52)
			if (file.binairies[2] == 0x49)
				if (file.binairies[3] == 0x4D)
					return new DrimFile(File::OpenFile_ptr(path));

	throw std::exception("This file do not have a correct DRIM signature.");
}
DrimFile DrimFile::OpenDrimFile(std::string path)
{
	return *OpenDrimFile_ptr(path);
}


void DrimFile::SetVersion(uint8_t global_version, uint8_t subversion)
{
	if (file->binairies.size() >= 6)
		file->Pop(4, 2);

	else
		file->Pop(4, file->binairies.size() - 4);
	
	file->Push({ global_version, subversion }, 4);
}
int DrimFile::GetGlobalVersion()
{
	if (file->binairies.size() < 6)
		throw std::exception("This drim file as not been initied correctly !");

	return static_cast<int>(file->Get(4));
}
int DrimFile::GetSubVersion()
{
	if (file->binairies.size() < 6)
		throw std::exception("This drim file as not been initied correctly !");
	
	return static_cast<int>(file->Get(5));
}
float DrimFile::GetVersion()
{
	return GetGlobalVersion() + (float)GetSubVersion() / 10;
}

void DrimFile::SetDimensions(uint16_t x_size, uint16_t y_size)
{
	// convert uint16_t to two uint8_t
	// X size
	uint8_t xs_byte1 = (x_size >> 8) & 0xFF;
	uint8_t xs_byte2 = x_size & 0xFF;
	// Y size
	uint8_t ys_byte1 = (y_size >> 8) & 0xFF;
	uint8_t ys_byte2 = y_size & 0xFF;


	if (file->binairies.size() >= 10)
		file->Pop(6, 4);

	else
		file->Pop(6, file->binairies.size() - 6);



	file->Push({ xs_byte1, xs_byte2, ys_byte1, ys_byte2 }, 6);
}
void DrimFile::GetDimensions(uint16_t* out_x_size, uint16_t* out_y_size)
{
	if (file->binairies.size() < 10)
		throw std::exception("This drim file as not been initied correctly !");
	// Get X size
	*out_x_size = (file->Get(6) << 8) | file->Get(7);
	// Get Y size
	*out_y_size = (file->Get(8) << 8) | file->Get(9);
}
uint16_t DrimFile::GetXDimensions()
{
	if (file->binairies.size() < 10)
		throw std::exception("This drim file as not been initied correctly !");
	return (file->Get(6) << 8) | file->Get(7);
}
uint16_t DrimFile::GetYDimensions()
{
	if (file->binairies.size() < 10)
		throw std::exception("This drim file as not been initied correctly !");
	return (file->Get(8) << 8) | file->Get(9);
}

void DrimFile::WriteTag(std::vector<uint8_t> identifier, std::vector<uint8_t> value)
{
	std::vector<uint8_t> to_add;

	// Add the identifier
	for (uint8_t e : identifier)
		to_add.push_back(e);

	// Add the lenght of data
	to_add.push_back(uint8_t(value.size()));

	// Add the content
	for (uint8_t e : value)
		to_add.push_back(e);

	file->Push(to_add, GetPixelSectionStartIndex() - 2);
}
void DrimFile::WriteTag(std::vector<uint8_t> identifier, uint8_t value)
{
	WriteTag(std::vector<uint8_t>{ identifier }, std::vector<uint8_t>{ value });
}
void DrimFile::WriteTag(uint8_t identifier, std::vector<uint8_t> value)
{
	WriteTag(std::vector<uint8_t>{ identifier }, std::vector<uint8_t>{ value });
}
void DrimFile::WriteTag(uint8_t identifier, uint8_t value)
{
	WriteTag(std::vector<uint8_t>{ identifier }, std::vector<uint8_t>{ value });
}


std::vector<uint8_t> DrimFile::SearchTag(std::vector<uint8_t> tag)
{
	int tag_index = tag.size();
	int tag_data_size;
	std::vector<uint8_t> bins = file->binairies;

	// TODO change int i = 0 to the start of TAGS section
	for (int i = 0; i < int(bins.size()) - tag.size(); i++)
	{
		if (file->Get(i, tag.size()) == tag)
			tag_index = i;
	}

	tag_data_size = tag_index + tag.size();
	std::vector<uint8_t> data = file->Get(tag_index + tag.size() + 1, file->Get(tag_data_size));
	return data;
}


void DrimFile::AddToPixelSection(std::vector<uint8_t> value)
{
	file->Push(value, GetPixelSectionEndIndex());
}
void DrimFile::AddToPixelSection(uint8_t value)
{
	AddToPixelSection({ value });
}

int DrimFile::GetPixelSectionStartIndex()
{
	// Search the first 0x7E;0x7E (start of pixels section)
	for (int i = 1; i < file->binairies.size(); i++)
		if (file->Get(i) == 0x7E && file->Get(i - 1) == 0x7E)
			return i + 1;

	return int(file->binairies.size());
}

int DrimFile::GetPixelSectionEndIndex()
{
	// Search the last 0x40;0x40 (end of pixels section)
	int last_index_found = file->binairies.size();
	for (int i = 0; i < file->binairies.size() -1; i++)
		if (file->Get(i) == 0x40 && file->Get(i + 1) == 0x40)
			last_index_found = i;

	return last_index_found;
}



DrimFile::DrimFile()
{ throw new std::exception("Please use the static methods OpenDrimFIle or CreateDrimFile of the File class to avoid getting this error."); }

DrimFile::~DrimFile()
{ 
	file->Close();
	delete file;
}

DrimFile::DrimFile(File* _file)
{
	file = _file;
}
