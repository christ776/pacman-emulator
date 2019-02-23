#include <SDL2/SDL.h>

// #include "main.hpp"
#include "parser/parser.h"
#include "renderer/sdl_renderer.h"

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
#define WINDOW_WIDTH 600


int main( int argc, char** argv )
{
    bool quit = false;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);


    vector<char> tileData = readTileROM("../pacman/pacman.5e");
    drawPixels(tileData,renderer);
    SDL_RenderPresent(renderer);

    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

