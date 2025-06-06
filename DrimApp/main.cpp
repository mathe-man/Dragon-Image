#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdint>
#include <bitset>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>



#include "ImageGenerator.h"
#include "Editor.h"
#include "DrimFile.h"
#include "File.h"
#include "Debug.h";


#ifdef _WIN32
#include <windows.h>
#endif

void EnableAnsiEscapeCodes() {
#ifdef _WIN32
    // Sur Windows, il faut activer les s�quences ANSI dans la console
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
    EnableAnsiEscapeCodes();
	Debug::DebugEditor();
}
