#pragma once

#include <vector>

class GradientGenerator {
public:
	static std::vector<uint8_t> GenerateBlueGradientImage(int w, int h) {//150 200 255
		return GradientGenerator::GenerateGradientImage(w, h, 0, 0, 0, 150, 200, 255);
	}
	static std::vector<uint8_t> GenerateYellowToBlueGradientImage(int w, int h)
	{
		return GradientGenerator::GenerateGradientImage(w, h, 255, 255, 0, 0, 0, 255);
	}

	static std::vector<uint8_t> GenerateGradientImage(int w, int h,
		uint8_t r_start, uint8_t g_start, uint8_t b_start,
		uint8_t r_end, uint8_t g_end, uint8_t b_end
	);
	static std::vector<uint8_t> GenerateGradientImage(int w, int h,
		uint8_t start,
		uint8_t end
	);
};