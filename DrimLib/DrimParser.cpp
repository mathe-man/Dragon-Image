#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <vector>
#include <unordered_map>
#include "DrimParser.h"



bool DrimParser::Parse(std::vector<uint8_t> data, bool* isDrimFile, std::string* version)
{
    std::cout << "Starting parsing with " << data.size() << " bytes:" << std::endl;
    CoutHexArray(data);

    // Check the drim signature
    std::string file_signature(data.begin(), data.begin() + 4);
    *isDrimFile = false;
    // return with false if it's not good
    if (file_signature != "DRIM") return false;
    *isDrimFile = true;


    *version = std::to_string(data[4]) + "." + std::to_string(data[5]);



    // cout the result of parsing
    std::cout << "File is in drim format: " << std::boolalpha << *isDrimFile << std::noboolalpha << "\n";
    std::cout << "drim format version: " << *version << "\n\n";

    return true;
}

void DrimParser::ReadTag(std::vector<uint8_t> data, uint8_t tag)
{
    int tag_index = 0;
    int tag_data_size;

    std::cout << "Seaching for tag '0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(tag) << "'.";
    std::cout << std::dec << std::nouppercase << std::endl;

    for (int i = 0; i < (int)data.size() - 1; i++)
    {
        if (data[i] == tag)
        {
            tag_index = (int)i;
            std::cout << "Index: " << i << std::endl;
            tag_data_size = (int)data[i + 1];
            break;
        }
    }


    std::cout << "Length:" << tag_data_size << std::endl;
    std::cout << "Found:" << std::endl;

    std::vector<uint8_t> tag_data;
    for (int i = tag_index + 2; i < tag_data_size + tag_index + 2; i++)
        tag_data.push_back(data[i]);

    CoutHexArray(tag_data);


}
uint16_t DrimParser::GetWidth(std::vector<uint8_t> data)
{
    if (data.size() < 8)
        return 0;

    uint8_t xs_byte1 = data[6];
    uint8_t xs_byte2 = data[7];

    uint16_t x_size = (xs_byte1 << 8) | xs_byte2;

    return x_size;
}
uint16_t DrimParser::GetHeight(std::vector<uint8_t> data)
{
    if (data.size() < 10)
        return 0;

    uint8_t ys_byte1 = data[8];
    uint8_t ys_byte2 = data[9];

    uint16_t y_size = (ys_byte1 << 8) | ys_byte2;

    return y_size;
}



void DrimParser::CoutHexArray(std::vector<uint8_t> data, int column_number, bool show_bin)
{
    //
    std::cout << "\n " << std::setfill('_') << std::setw(6 + (show_bin ? 18 * column_number : 6 * column_number)) << "" << std::endl;

    std::cout << std::hex <<  std::uppercase << std::setfill('0');

    


    for (int raw = 0; raw < int(data.size()) / column_number; raw += 1)
    {
        std::cout << " |";
        // Show hex values
        for (int i = 0; i < column_number; i++)
        {
            uint8_t byte = data[raw * column_number + i];
            std::cout << " 0x" << std::setw(2) << static_cast<int>(byte);
        }

        std::cout << " | ";
        // Show ASCII (char) values
        for (int i = 0; i < column_number; i++)
        {
            uint8_t byte = data[raw * column_number + i];
            std::cout << ((byte >= 32 && byte <= 126) ? static_cast<char>(byte) : '.');
        }

        std::cout << " |";

        // Show bin values
        if (show_bin)
        {
            for (int i = 0; i < column_number; i++)
            {
                uint8_t byte = data[raw * column_number + i];
                std::cout << "0b" << std::bitset<8>(byte) << " |";
            }
        }



        std::cout << std::endl;
    }
    
    int remaining_bytes = int(data.size()) % column_number;
    int unfilled_space = column_number - remaining_bytes;

    std::cout << " |";
    // Show hex values
    for (int i = 0; i < remaining_bytes; i++)
    { 
        uint8_t byte = data[int(data.size()) - int(data.size()) % column_number + i];
        std::cout << " 0x" << static_cast<int>(byte);
    }
    // Fill blank spaces
    for (int i = 0; i < unfilled_space; i++)
        std::cout << "     ";



    std::cout << " | ";
    // Show ASCII (char) values
    for (int i = 0; i < remaining_bytes; i++)
    {
        uint8_t byte = data[int(data.size()) - int(data.size()) % column_number + i];
        std::cout << ((byte >= 32 && byte <= 126) ? static_cast<char>(byte) : '.');
    }
    // Fill blank space
    for (int i = 0; i < unfilled_space; i++)
        std::cout << " ";
    
    

    std::cout << " |";
    // Show bin values
    if (show_bin)
    {
        for (int i = 0; i < remaining_bytes; i++)
        {
            uint8_t byte = data[int(data.size()) - int(data.size()) % column_number + i];
            std::cout << "0b" << std::bitset<8>(byte) << " |";
        }
        // Fill blank space
        for (int i = 0; i < unfilled_space; i++)
            std::cout << "           |";
    }

   
    
    
    std::cout << "\n " << std::setfill('-') << std::setw(6 + (show_bin ? 18 * column_number : 6 * column_number)) << "" << std::endl;
    std::cout << std::dec << std::nouppercase << std::setfill(' ');
}