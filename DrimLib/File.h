#pragma once
#include <string>
#include <vector>
class File
{
public:
	//Constructors
	static File CreateFile(std::string path);
	static File* CreateFile_ptr(std::string path);
	static File OpenFile(std::string path);
	static File* OpenFile_ptr(std::string path);

	// Attributs
	std::string file_path;			// Path to the file represented by this class instance
	std::vector<uint8_t> binairies; // Make change on this to avoid writting too many time in the file
	
	std::vector<uint8_t> ReadFile();	// Read the file bin
	std::vector<uint8_t> UpdateBinairies();	// Read the file bin then put the result into binairies
	void WriteFile(std::vector<uint8_t> data);	// Write 'data' in the file
	void UpdateFile();							// Write binairies in the fil

	// Write in binairies
	bool Push(std::vector<uint8_t> content, int index);	bool Push(uint8_t content, int index);		// Write in binairies
	// Write at the end of binairies
	void PushBack(std::vector<uint8_t> content); void PushBack(uint8_t content);
	// Write at the begin of binairies
	void PushFront(std::vector<uint8_t> content); void PushFront(uint8_t content);

	// Remove a number of element in binairies,
	bool Pop(int index, int count); bool Pop(int index);
	// Remove a number of element at the end of binairies
	bool PopBack(int count); bool PopBack();
	// Remove a number of element at the start of binairies
	bool PopFront(int count); bool PopFront();


	std::vector<uint8_t> Get(int index, int count); uint8_t Get(int index);
	std::vector<uint8_t> GetFront(int count); uint8_t GetFront();
	std::vector<uint8_t> GetBack(int count);  uint8_t GetBack();


	// TODO add a description and definition for this method
	bool Replace(int index, std::vector<uint8_t> content); bool Replace(int index, uint8_t content);


	void Close();
	File();		//Return should return an error: use CreateFile or OpenFile instead.
	~File();	//Save binairies in the file for the last time

private:
	File(std::string path);	// Can only be used in OpenFile or CreateFile
};

