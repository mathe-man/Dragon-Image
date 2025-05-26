#include "DrimDebugger/Debug.h"
#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <vector>
#include "DrimDebugger/ImageGenerator.h"
#include "DrimEditor/Editor.h"
#include "DrimLib/DrimFile.h"
#include "DrimLib/File.h"

void Debug::CoutHexArray(std::vector<uint8_t> data, int column_number, bool show_bin)
{
    //
    std::cout << "\n " << std::setfill('_') << std::setw(6 + (show_bin ? 18 * column_number : 6 * column_number)) << "" << std::endl;

    std::cout << std::hex << std::uppercase << std::setfill('0');




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

int Debug::DebugEditor()
{
	// Init a viewer
	Editor editor;
	editor.Init(1080, 720);


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

	editor.OpenWiewer(editor.GetByteTexture_Gray(big_image, 1080, 720));
	editor.OpenWiewer(editor.GetByteTexture_Gray(rand_image, 1080, 720));
	editor.OpenWiewer(editor.GetByteTexture_Gray(image_2by2, 2, 2));
	editor.OpenWiewer(editor.GetByteTexture_Gray(image_4by4, 4, 4));


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


	editor.OpenWiewer(editor.GetByteTexture_RGB(rgb_image_2by2, 2, 2));
	editor.OpenWiewer(editor.GetByteTexture_RGB(rgb_image_3by2, 3, 2));
	editor.OpenWiewer(editor.GetByteTexture_RGB(rgb_image_4by4, 4, 4));
	editor.OpenWiewer(editor.GetByteTexture_RGB(rgb_blue_gradient, 540, 360));
	editor.OpenWiewer(editor.GetByteTexture_RGB(rgb_yellow_gradient, 540, 360));
	editor.OpenWiewer(editor.GetByteTexture_RGB(rgb_red_blue_gradient, 540, 360));

	editor.Free();

	return 0;
}
int Debug::DebugDrimFile()
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

	Debug::CoutHexArray(f->binairies);

	std::cout << std::endl << std::endl;

	drim->file->Close();

	return 0;
}
