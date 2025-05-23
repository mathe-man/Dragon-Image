#include "File.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


File File::CreateFile(std::string path)
{
	return *CreateFile_ptr(path);
}
File* File::CreateFile_ptr(std::string path)
{
	// Reset/Create the file
	std::ofstream f(path, std::ios::trunc);
	f.close();

	return OpenFile_ptr(path);
}

File File::OpenFile(std::string path)
{
	return *OpenFile_ptr(path);
}
File* File::OpenFile_ptr(std::string path)
{
	File* file = new File(path);
	(*file).UpdateBinairies();
	return file;
}

File::File()
{
	throw std::exception("Please use the static methods OpenFile or CreateFile of the File class to avoid getting this error.");
}
File::File(std::string path)
{
	this->file_path = path;
	// TODO add the binairies read on init here
}



std::vector<uint8_t> File::ReadFile()
{
	// Open a In file stream
	std::ifstream file(file_path, std::ios::binary);

	std::vector<uint8_t> content;
	
	// Read
	content = std::vector<uint8_t>(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	);

	return content;
}
std::vector<uint8_t> File::UpdateBinairies()
{
	binairies = ReadFile();
	return binairies;
}
 
void File::WriteFile(std::vector<uint8_t> data)
{
	// Clear the file content
	std::ofstream f(file_path, std::ios::trunc);
	f.close();


	// Open an Out file stream in binary
	std::ofstream file(file_path, std::ios::out | std::ios::binary);
	// Write the data
	for (uint8_t value : data)
		file.put(value);

	
	file.close();

	// Update binairies if needed
	if (data != binairies)
		UpdateBinairies();
}
void File::UpdateFile()
{
	WriteFile(binairies);
}



bool File::Push(std::vector<uint8_t> content, int index)
{
	// Return false if index is out of range
	if (index < 0 || index > binairies.size())
		return false;
	
	// Add each elemnt of content
	binairies.insert(
		binairies.begin() + index,
		content.begin(),
		content.end()
	);
	return true;
}
bool File::Push(uint8_t content, int index)
{
	// Create a vector based on content
	return Push(std::vector<uint8_t> {content}, index);
}

void File::PushBack(std::vector<uint8_t> content)
{
	Push(content, binairies.size());
}
void File::PushBack(uint8_t content)
{
	// Create a vector based on content
	PushBack(std::vector<uint8_t> {content});
}

void File::PushFront(std::vector<uint8_t> content)
{
	Push(content, 0);
}
void File::PushFront(uint8_t content)
{
	// Create a vector based on content
	PushFront(std::vector<uint8_t> {content});
}



bool File::Pop(int index, int count)
{
	if (!(index + count -1 < binairies.size()))
		return false;

	binairies.erase(binairies.begin() + index, binairies.begin() + index + count);
	return true;
}
bool File::Pop(int index)
{
	return Pop(index, 1);
}

bool File::PopBack(int count)
{
	if (count > binairies.size())
		return false;

	return Pop(binairies.size() - count, count);
}
bool File::PopBack()
{
	return PopBack(1);
}

bool File::PopFront(int count)
{
	return Pop(0, count);
}
bool File::PopFront()
{
	return PopFront(1);
}


std::vector<uint8_t> File::Get(int index, int count)
{
	if (index + count > binairies.size())
		throw std::exception("There is no enought content to return");


	std::vector<uint8_t> found;
	for (int i = index; i < index + count; i++)
		found.push_back(binairies[i]);

	return found;
}
uint8_t File::Get(int index)
{
	return Get(index, 1)[0];
}

std::vector<uint8_t> File::GetFront(int count)
{
	return Get(0, count);
}
uint8_t File::GetFront()
{
	return Get(0);
}

std::vector<uint8_t> File::GetBack(int count)
{
	return Get(binairies.size() - count, count);
}
uint8_t File::GetBack()
{
	return Get(binairies.size() - 1);
}



void File::Close()
{
	UpdateFile();
}

File::~File()
{
	Close();
}
