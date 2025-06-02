#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdint>
#include <thread>
#include <bitset>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>



#include "DrimDebugger/ImageGenerator.h"
#include "DrimEditor/Editor.h"
#include "DrimLib/DrimFile.h"
#include "DrimLib/File.h"
#include "DrimDebugger/Debug.h";


#ifdef _WIN32
#include <windows.h>
#endif

void EnableAnsiEscapeCodes() {
#ifdef _WIN32
    // Sur Windows, il faut activer les séquences ANSI dans la console
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

#endif
}
//	<<>>


int main()
{
    Editor editor;
    editor.Init("Editor");

    // Generate a gradient image vector
    std::vector<uint8_t> gradient_vector;
    gradient_vector = GradientGenerator::GenerateBlueGradientImage(1080, 720);
    
    DrimFile file = DrimFile::CreateDrimFile("file.drim", 1080, 720);
    
	std::cout << static_cast<int>(file.GetXDimensions()) << "x" << static_cast<int>(file.GetYDimensions()) << std::endl;
}
