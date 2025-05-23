#pragma once

#include <vector>

class GradientGenerator {
public:
	static std::vector<uint8_t> GenerateBlueGradientImage(int w, int h);
	static std::vector<uint8_t> GenerateYellowToBlueGradientImage(int w, int h);
	
	static std::vector<uint8_t> GenerateGradientImage(int w, int h,
		uint8_t r_start, uint8_t g_start, uint8_t b_start,
		uint8_t r_end, uint8_t g_end, uint8_t b_end
	);
};