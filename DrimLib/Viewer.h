#pragma once
#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <stdint.h>
#include <stdbool.h>

class Viewer {
public:
    bool Init(std::string title, int w, int h);
    bool Init(std::string title);
    bool Init(int w, int h);
    bool Init();

    SDL_Texture* GetPngTexture(std::string path);
    SDL_Texture* GetByteTexture_Gray(const std::vector<uint8_t>& bytes, int width, int height);
    SDL_Texture* GetByteTexture_RGB(const std::vector<uint8_t>& bytes, int width, int height);
    

    bool OpenWiewer(SDL_Texture* texture, bool destroy_texture_at_end = false);
    void Free();

    SDL_Renderer* renderer;
    SDL_Window* window;

private:
    void CalculateResize();
    void UpdateFpsInfos(Uint64* last_frame, float* fps, float* fps_average);

    void MouseEvent(const SDL_Event* event);

    // return true if the (x,y) position is situed in the rect)
    bool IsPositionInRect(const SDL_FRect* rect, int x, int y);

    void MoveCursor(int row, int col) {
        std::cout << "\x1b[" << row << ";" << col << "H";
    }

    // UI properties
    float pixel_area_percentage = 0.9;

    SDL_FRect* image_area;
    SDL_FRect* ui_area;
};