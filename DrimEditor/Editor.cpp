#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>   
#include <string>

#include <iomanip>
#include <iostream>
#include "Editor.h"

bool Editor::Init(std::string title, int w, int h)
{
    // Init SDL (only video subsystem)
    if (SDL_Init(SDL_INIT_VIDEO) != true) {
        std::cerr << "SDL_Init Video error: " << SDL_GetError() << std::endl;
        //return false;  // On arr�te vraiment si init �choue
    }

    // Create SDL_Window
    window = SDL_CreateWindow(title.c_str(), w, h, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create SDL_Renderer
    renderer = SDL_CreateRenderer(window, "opengl");
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        return false;
    }

    image_area = new SDL_FRect();
    if (!image_area)
    {
        std::cerr << "Error while creating rect : image_area" << std::endl;
        return false;
    }
    ui_area = new SDL_FRect();
    if (!ui_area)
    {
        std::cerr << "Error while creating rect : ui_area" << std::endl;
        return false;
    }


    return true;
}
bool Editor::Init(std::string title)
{
    return Init(title, 1080, 720);
}
bool Editor::Init(int w, int h)
{
    return Init("Drim Editor", w, h);
}
bool Editor::Init()
{
    return Init("Drim Editor");
}



SDL_Texture* Editor::GetPngTexture(std::string path)
{

    // Load file into a texture
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture)
    {
        std::cerr << "IMG_LoadTexture error: " << SDL_GetError() << std::endl;
        return nullptr;
    }


    return texture;
}
SDL_Texture* Editor::GetByteTexture_Gray(const std::vector<uint8_t>& bytes, int width, int height)
{
    //  Check parameters
    if (bytes.size() != (size_t)(width * height) || width <= 0 || height <= 0) {
        std::cerr << "Param�tres invalides - byte size=" << bytes.size() << " w*h=" << width*height << std::endl;
        return nullptr;
    }


    // Create new texture
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        std::cerr << "Erreur cr�ation de texture: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);


    // Resize window size
    float window_ratio = float(width) / float(height);
    SDL_SetWindowAspectRatio(window, window_ratio, window_ratio);

    // Temporary RGBA32 buffer (4bytes by pixel)
    std::vector<uint8_t> pixels_rgba(width * height * 4);

    // Fill pixels_rgba with gray 
    for (int i = 0; i < width * height; ++i) {
        uint8_t v = bytes[i];
        pixels_rgba[i * 4 + 0] = 255;   // A
        pixels_rgba[i * 4 + 1] = v;     // B
        pixels_rgba[i * 4 + 2] = v;     // G
        pixels_rgba[i * 4 + 3] = v;     // R
    }


    // Lock texture to write in
    void* texture_pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(texture, nullptr, &texture_pixels, &pitch) < 0) {
        std::cerr << "Erreur lock texture: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(texture);
        return nullptr;
    }


    // Copy data in the texture
    // should be pitch = width * 4, but here it copy line by line for safety
    for (int y = 0; y < height; ++y) {
        memcpy(
            (uint8_t*)texture_pixels + y * pitch,
            pixels_rgba.data() + y * width * 4,
            width * 4
        );
    }

    SDL_UnlockTexture(texture);


    return texture;
}


SDL_Texture* Editor::GetByteTexture_RGB(const std::vector<uint8_t>& bytes, int width, int height)
{
    //  Check parameters
    if (bytes.size()/3 != (size_t)(width * height) || width <= 0 || height <= 0) {
        std::cerr << "Param�tres invalides - byte size=" << bytes.size() << " w*h=" << width * height << std::endl;
        return nullptr;
    }

    // Create new texture
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        std::cerr << "Erreur cr�ation de texture: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);


    // Temporary RGBA32 buffer (4bytes by pixel)
    std::vector<uint8_t> pixels_rgba(width * height * 4);

    // Fill pixels_rgba with gray 
    for (int i = 0; i < width * height; i++) {
        uint8_t r = bytes[i * 3 + 0];
        uint8_t g = bytes[i * 3 + 1];
        uint8_t b = bytes[i * 3 + 2];

        pixels_rgba[i * 4 + 0] = 255;   // A
        pixels_rgba[i * 4 + 1] = b;   // B
        pixels_rgba[i * 4 + 2] = g;   // G
        pixels_rgba[i * 4 + 3] = r; // R
    }


    // Lock texture to write in
    void* texture_pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(texture, nullptr, &texture_pixels, &pitch) < 0) {
        std::cerr << "Erreur lock texture: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(texture);
        return nullptr;
    }


    // Copy data in the texture
    // should be pitch = width * 4, but here it copy line by line for safety
    for (int y = 0; y < height; ++y) {
        memcpy(
            (uint8_t*)texture_pixels + y * pitch,
            pixels_rgba.data() + y * width * 4,
            width * 4
        );
    }

    SDL_UnlockTexture(texture);



    return texture;
}


bool Editor::OpenWiewer(SDL_Texture* texture, bool destroy_texture_at_end)
{
    // Check texture
    if (!texture)
    {
        std::cerr << "Invalid texture \n";
        return false;
    }


    // Calculate the new window ratio for the texture
    float window_ratio = (float)texture->w * 1.25 / (float)texture->h;
    SDL_SetWindowAspectRatio(window, window_ratio, window_ratio);

    // TODO fix the ratio 
	SDL_SetWindowSize(window, texture->w * 1.25, texture->h * 1.25);

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);



    // Hide cursor
    std::cout << "\x1b[?25l";


    // Window loop init
    bool running = true;
    SDL_Event event;

    CalculateResize();



    Uint64 lastFrame = SDL_GetTicksNS();
    float fps = 0.0f;
    float fps_average = 0.0f;

    // Window loop run
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                CalculateResize();
            }

            // The clic appen where the mouse is released
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
                MouseEvent(&event);
        }
        // Update fps infos
        UpdateFpsInfos(&lastFrame, &fps, &fps_average);
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 10, 0, 100, 255);
        SDL_RenderClear(renderer);

        // Draw texture
        SDL_RenderTexture(renderer, texture, NULL, image_area);
        // Draw button
        SDL_SetRenderDrawColor(renderer, 150, 255, 0, 255);
        SDL_RenderFillRect(renderer, &button);

        // Update screen
        SDL_RenderPresent(renderer);
    }



    // Clean and return
    MoveCursor(7, 0);
    if (destroy_texture_at_end)
        SDL_DestroyTexture(texture);

    return true;
}

void Editor::CalculateResize()
{
    int win_w, win_h;
    SDL_GetWindowSize(window, &win_w, &win_h);

    // Create the pixel area
    image_area->x = 0;
    image_area->y = 0;
    image_area->w = win_w * pixel_area_percentage;
    image_area->h = win_h;
    // Remaining space for the UI
    ui_area->x = image_area->w;
    ui_area->y = 0;
    ui_area->w = win_w - image_area->w;
    ui_area->h = win_h;

    // Button in the UI
    button.x = ui_area->x + ui_area->w * 0.1f;
    button.y = ui_area->y + ui_area->h * 0.2f;
    button.w = ui_area->w * 0.8;
    button.h = ui_area->h * 0.08;
}
void Editor::UpdateFpsInfos(Uint64* last_frame, float* fps, float* fps_average)
{

    // Calcul du delta temps
    Uint64 currentFrame = SDL_GetTicksNS();
    float deltaSec = (currentFrame - *last_frame) / 1'000'000'000.0f;
    *last_frame = currentFrame;

    // Calcul FPS
    if (deltaSec > 0.0f) {
        *fps = 1.0f / deltaSec;
    }
    *fps_average = 0.995 * *fps_average + 0.005 * *fps;

    // Show result for debug
    MoveCursor(2, 1);
    std::cout << "FPS : " << *fps << std::endl;
    std::cout << "FPS_: " << *fps_average << std::endl;
}


void Editor::MouseEvent(const SDL_Event* event)
{
    int x = event->button.x;
    int y = event->button.y;

    std::string rect = ""; // The name of the rect where the cursor is
    rect += IsPositionInRect(image_area, x, y) ? "image_area, ": "";
    rect += IsPositionInRect(ui_area, x, y)    ? "ui_area, ": "";
    rect += IsPositionInRect(&button, x, y)    ? "button, ": "";

    if (event->button.button == SDL_BUTTON_LEFT)
    {
        MoveCursor(5, 1);
        std::cout << "Last left click   : ( " << std::setw(4) << x << " ; " << std::setw(4) << y << " ) in " << rect << std::setw(15) << " \n";
    }
    else if (event->button.button == SDL_BUTTON_RIGHT)
    {
        MoveCursor(6, 1);
        std::cout << "Last right click  : ( " << std::setw(4) << x << " ; " << std::setw(4) << y << " ) in " << rect << std::setw(15) << " \n";
    }
    else if (event->button.button == SDL_BUTTON_MIDDLE)
    {
        MoveCursor(7, 1);
        std::cout << "Last middle click : ( " << std::setw(4) << x << " ; " << std::setw(4) << y << " ) in " << rect << std::setw(15) << " \n";
        
    }
}
bool Editor::IsPositionInRect(const SDL_FRect* rect, int x, int y)
{
    // Check X axis
    if (x > rect->x && x < (rect->x + rect->w))
        if (y > rect->y && y < (rect->y + rect->h))
            return true;

    return false;
}


void Editor::Free()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}