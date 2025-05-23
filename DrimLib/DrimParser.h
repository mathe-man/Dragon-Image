#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

class DrimParser {
public:
    static bool Parse(std::vector<uint8_t> data, bool* isDrimFile, std::string* version);
    static void ReadTag(std::vector<uint8_t> data, uint8_t tag);
    static uint16_t GetWidth(std::vector<uint8_t> data);
    static uint16_t GetHeight(std::vector<uint8_t> data);

    static void CoutHexArray(std::vector<uint8_t> data, int column_number = 2, bool show_bin = false);
};