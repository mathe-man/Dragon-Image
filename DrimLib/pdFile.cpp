#include <fstream>
#include <vector>
#include <iostream>
#include "DrimParser.h"
#include "pdFile.h"


pdFile::pdFile(std::string path, bool existent, uint16_t x_size, uint16_t y_size)
{
    file_path = path;

    if (existent)
        return;
    // Reset/create the file
    std::ofstream f(file_path, std::ios::trunc);
    f.close();

    // Those process will create a new drim file with
    // Signature, version and dimensions
    SetSignature();
    SetVersion(0x00, 0x01);
    SetDimensions(x_size, y_size);

    // Add 'TAGS' in ascii,indicate the start of the tags section

    std::vector<uint8_t> TAGS_ascii_hex = { 0x54, 0x41, 0x47, 0x53 };
    std::vector<uint8_t> binairies = ReadBinairies();
    for (uint8_t u : TAGS_ascii_hex)
        binairies.push_back(u);

    // Add pixels start and end tag
    binairies.push_back(0x7E);  binairies.push_back(0x7E);
    binairies.push_back(0x40);  binairies.push_back(0x40);

    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    for (uint8_t u : binairies)
        file.put(u);


}

void pdFile::SetSignature()
{
    // Load file binaries
    std::vector<uint8_t> binary = this->ReadBinairies();

    // Check if it's a new file (when there is still 4 bytes)
    if (binary.size() == 0) {
        binary.push_back(0x44);
        binary.push_back(0x52);
        binary.push_back(0x49);
        binary.push_back(0x4D);
    }
    else
    {
        binary[0] = 0x44;
        binary[1] = 0x52;
        binary[2] = 0x49;
        binary[3] = 0x4D;
    }


    // Reset the file
    std::ofstream f(file_path, std::ios::trunc);
    f.close();

    // Rewrite with insertion
    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    for (uint8_t value : binary)
        file.put(value);
}
void pdFile::SetVersion(uint8_t version, uint8_t subversion)
{
    // Load file binaries
    std::vector<uint8_t> binary = this->ReadBinairies();

    // Check if it's a new file (when there is still 4 bytes)
    if (binary.size() == 4) {
        binary.push_back(version);
        binary.push_back(subversion);
    }
    else
    {
        binary[4] = version;
        binary[5] = version;
    }


    // Reset the file
    std::ofstream f(file_path, std::ios::trunc);
    f.close();

    // Rewrite with insertion
    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    for (uint8_t value : binary)
        file.put(value);
}
void pdFile::SetDimensions(uint16_t x_size, uint16_t y_size)
{
    // convert uint16_t to two uint8_t
    // X size
    uint8_t xs_byte1 = (x_size >> 8) & 0xFF;
    uint8_t xs_byte2 = x_size & 0xFF;
    // Y size
    uint8_t ys_byte1 = (y_size >> 8) & 0xFF;
    uint8_t ys_byte2 = y_size & 0xFF;


    // Load file binaries
    std::vector<uint8_t> binary = this->ReadBinairies();

    // Check if it's a new file (when there is still 6 bytes)
    if (binary.size() == 6) {
        binary.push_back(xs_byte1);
        binary.push_back(xs_byte2);

        binary.push_back(ys_byte1);
        binary.push_back(ys_byte2);
    }
    else
    {
        binary[6] = xs_byte1;
        binary[7] = xs_byte2;
        binary[8] = ys_byte1;
        binary[9] = ys_byte2;
    }


    // Reset the file
    std::ofstream f(file_path, std::ios::trunc);
    f.close();

    // Rewrite with insertion
    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    for (uint8_t value : binary)
        file.put(value);
}


void pdFile::Insert(int index, std::vector<uint8_t> data)
{
    std::vector<uint8_t> binary = this->ReadBinairies();

    // make change
    binary.insert(binary.begin() + index, data.begin(), data.end());


    // Reset the file
    std::ofstream f(file_path, std::ios::trunc);
    f.close();

    // Rewrite with insertion
    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    for (uint8_t value : binary)
        file.put(value);
}

void pdFile::WriteTag(uint8_t identifier, std::vector<uint8_t> data)
{
    std::vector<uint8_t> to_add = data;

    //Add size and identifier of the tag, lines is in reversed order
    // because it will add to the begin.
    to_add.insert(to_add.begin(), (uint8_t)data.size());
    to_add.insert(to_add.begin(), identifier);

    std::vector<uint8_t> binairies = ReadBinairies();


    this->Insert(GetPixelSectionStartIndex() - 2, to_add);
}



std::vector<uint8_t> pdFile::ReadBinairies()
{
    std::ifstream file(file_path, std::ios::binary);

    return std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}


int pdFile::GetPixelSectionStartIndex()
{
    // search the first 0x7E;0x7E (start of pixels data) in binairies

    std::vector<uint8_t> binairies = ReadBinairies();

    for (int i = 1; i < (int)binairies.size(); i++)
        if (binairies[i] == 0x7E && binairies[i - 1] == 0x7E)
            return i + 1;

    return int(binairies.size());
}

int pdFile::GetPixelSectionEndIndex()
{
    // search the last 0X40 (end of pixels data) in binairies

    std::vector<uint8_t> binairies = ReadBinairies();
    int pixels_end_index = binairies.size();

    for (int i = 0; i < int(binairies.size() - 1); i++)
        if (binairies[i] == 0x40 && binairies[i + 1] == 0x40)
            pixels_end_index = i;

    return pixels_end_index;
}


uint8_t pdFile::GetVersion()
{
    return      ReadBinairies()[4];
}
void    pdFile::GetVersion(uint8_t* version)
{
    *version = ReadBinairies()[4];
}

uint8_t pdFile::GetSubVersion()
{
    return      ReadBinairies()[5];
}
void    pdFile::GetSubVersion(uint8_t* version)
{
    *version = ReadBinairies()[5];
}