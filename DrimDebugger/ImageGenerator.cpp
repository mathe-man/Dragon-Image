#include <cstdint>
#include <vector>

#include "DrimDebugger/ImageGenerator.h"





std::vector<uint8_t> GradientGenerator::GenerateGradientImage(int w, int h,
	uint8_t r_start, uint8_t g_start, uint8_t b_start,
	uint8_t r_end, uint8_t g_end, uint8_t b_end
) {
	const int width = w;
	const int height = h;
	std::vector<uint8_t> rgb_image;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float t = static_cast<float>(x + y) / (width + height - 2);  // [0, 1]

			uint8_t r = static_cast<uint8_t>(r_start + t * (r_end - r_start));
			uint8_t g = static_cast<uint8_t>(g_start + t * (g_end - g_start));
			uint8_t b = static_cast<uint8_t>(b_start + t * (b_end - b_start));

			rgb_image.push_back(r);
			rgb_image.push_back(g);
			rgb_image.push_back(b);
		}
	}

	return rgb_image;
}

std::vector<uint8_t> GradientGenerator::GenerateGradientImage(int w, int h,
	uint8_t start,
	uint8_t end
)
{
	return GradientGenerator::GenerateGradientImage(x, h,
		start, start, start,
		end, end, end);
}