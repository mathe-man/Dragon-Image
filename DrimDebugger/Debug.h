#pragma once

#include <vector>


class Debug
{
public:
	static void CoutHexArray(std::vector<uint8_t> data, int column_number = 2, bool show_bin = false);

	static int DebugEditor();
	static int DebugDrimFile();
};


