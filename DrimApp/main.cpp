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
#include "DrimDebugger/Debug.h"


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

	std::vector<uint8_t> png_bytes = editor.ConvertSurfaceToBytes(editor.GetPngSurface("test.png"));
    
    DrimFile file = DrimFile::OpenDrimFile("converted.drim");

    editor.OpenViewer(editor.GetByteTexture_RGB(file.GetPixelSectionContent(), 512, 512), true);

    /*
    for (int i = 2; i < png_bytes.size(); i += 3)
        std::cout   << static_cast<int>(png_bytes[i - 2]) 
                    << static_cast<int>(png_bytes[i - 1]) 
                    << static_cast<int>(png_bytes[i - 0]) << std::endl;*/
}
