#pragma once
#include <fstream>
#include <vector>
#include <iostream>


class pdFile {
public:
    pdFile(std::string path, bool existent, uint16_t x_size, uint16_t y_size);

    void Insert(int index, std::vector<uint8_t> data);

    void SetSignature();
    void SetDimensions(uint16_t x_size, uint16_t y_size);
    void SetVersion(uint8_t version, uint8_t subversion);

    void WriteTag(uint8_t identifier, std::vector<uint8_t> data);

    std::vector<uint8_t> ReadBinairies();

    uint8_t GetVersion();       void GetVersion(uint8_t* version);
    uint8_t GetSubVersion();    void GetSubVersion(uint8_t* subVersion);


    int GetPixelSectionStartIndex();
    int GetPixelSectionEndIndex();


    std::string file_path;

};


