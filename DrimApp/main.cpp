#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdint>
#include <bitset>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>



#include "ImageGenerator.h"
#include "Viewer.h"
#include "DrimFile.h"
#include "File.h"
#include "DrimParser.h"

//	<<>>
// === Debugging methods === 
class Debug {
public:
	static int DebugViewer()
	{
		// Init a viewer
		Viewer v;
		v.Init(1080, 720);


		/*  === Some gray scale images === */
		std::vector<uint8_t> big_image;
		// Fill big_image
		for (int i = 0; i < 97200; i++) {
			for (int j = 0; j < 4; j++)
				big_image.push_back(0xFF);
			for (int j = 0; j < 4; j++)
				big_image.push_back(0x00);
		}

		std::vector<uint8_t> rand_image;
		// Fill rand_image
		for (int i = 0; i < 97200; i++) {
			for (int j = 0; j < 4; j++)
				rand_image.push_back(rand() % 256);
			for (int j = 0; j < 4; j++)
				rand_image.push_back(rand() % 256);
		}

		std::vector <uint8_t> image_2by2 = { 0, 255,
											  255, 0 };
		std::vector <uint8_t> image_4by4 = { 0, 255, 255, 0,
											  255, 0, 0, 255,
											  255, 0, 0, 255,
											  0, 255, 255, 0, };

		v.OpenWiewer(v.GetByteTexture_Gray(big_image, 1080, 720));
		v.OpenWiewer(v.GetByteTexture_Gray(rand_image, 1080, 720));
		v.OpenWiewer(v.GetByteTexture_Gray(image_2by2, 2, 2));
		v.OpenWiewer(v.GetByteTexture_Gray(image_4by4, 4, 4));


		/*  === Various RGB images vector ===  */
		std::vector<uint8_t> rgb_image_2by2 =
		{
			255, 255, 255,   0, 255, 0,
			255, 255, 0,     0, 0,   0
		};

		std::vector <uint8_t> rgb_image_3by2 = {
			255, 0, 255,    0, 0, 255,   0, 255, 0,
			255, 0, 0,      255, 255, 0,  255, 0, 255
		};
		std::vector<uint8_t> rgb_image_4by4 =
		{
			255, 255, 0,    0, 255, 0,    0, 255, 0,    0, 255, 255,
			255, 0,   0,    255, 255, 0,   0, 255, 255,  0,   0, 255,
			255, 0,   0,    255, 255, 0,   0, 255, 255,  0,   0, 255,
			255, 255, 0,    0, 255, 0,    0, 255, 0,    0, 255, 255,
		};
		std::vector<uint8_t> rgb_blue_gradient = GradientGenerator::GenerateBlueGradientImage(540, 360);
		std::vector<uint8_t> rgb_yellow_gradient = GradientGenerator::GenerateYellowToBlueGradientImage(540, 360);
		std::vector<uint8_t> rgb_red_blue_gradient = GradientGenerator::GenerateGradientImage(540, 360, 255, 0, 0, 0, 0, 255);


		v.OpenWiewer(v.GetByteTexture_RGB(rgb_image_2by2, 2, 2));
		v.OpenWiewer(v.GetByteTexture_RGB(rgb_image_3by2, 3, 2));
		v.OpenWiewer(v.GetByteTexture_RGB(rgb_image_4by4, 4, 4));
		v.OpenWiewer(v.GetByteTexture_RGB(rgb_blue_gradient, 540, 360));
		v.OpenWiewer(v.GetByteTexture_RGB(rgb_yellow_gradient, 540, 360));
		v.OpenWiewer(v.GetByteTexture_RGB(rgb_red_blue_gradient, 540, 360));

		v.Free();

		return 0;
	}
	static int DebugDrimFile()
	{
		DrimFile* drim = DrimFile::CreateDrimFile_ptr("file.drim", 1080, 720);
		//DrimFile* drim = DrimFile::OpenDrimFile_ptr("file.drim");

		File* f = drim->file;


		drim->AddToPixelSection({ 0x01, 0x02, 0x03, 0x04 });

		std::cout
			<< "\nProperties of file.drim: " << std::endl
			<< "Version: " << drim->GetVersion() << std::endl
			<< "Pixel section (start-end): " << drim->GetPixelSectionStartIndex() << "-" << drim->GetPixelSectionEndIndex() << std::endl
			<< "Size: " << f->binairies.size() << std::endl
			<< "Raw hex content:" << std::endl;

		DrimParser::CoutHexArray(f->binairies, 4);

		std::cout << std::endl << std::endl;

		drim->file->Close();

		return 0;
	}

};


int main()
{
	DrimFile file = DrimFile::OpenDrimFile("image.drim");

	Viewer view;
	view.Init();
	view.OpenWiewer(view.GetByteTexture_RGB(file.file->Get(file.GetPixelSectionStartIndex(), 540* 360*3), 540, 360));

	return 0;
}
