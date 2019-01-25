#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <bitset>

#include "01_hello_SDL.hpp"
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
#define WINDOW_WIDTH 600

ostream& operator<<(ostream& os, Pixel& obj) {
    return os << "Rendering pixel with" << obj.color << endl;
}

Pixel::Pixel(uint8_t low_nibble, uint8_t high_nibble) {
    uint8_t pixel_color = (low_nibble << 1)  | high_nibble;
    std::bitset<8> pixel (pixel_color); 
    // cout << "pixel bits: " << pixel << endl;    

    switch (pixel_color)
    {
        case 0x0:
            color = { 0,0,0 };
            break;

        case 0x01:
            color = { 255, 0 , 0};
        break;

        case 0x2:
            color = { 255, 255, 0};
        break;

        case 0x3:
            color = { 0,255,0 };
        break;

        default:
            color = { 0,144, 244 };
            break;
    }
}

int main( int argc, char** argv )
{
    bool quit = false;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    // SDL_Window * window = SDL_CreateWindow("SDL2 Grayscale", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    // SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    // SDL_PumpEvents();
    // SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // for (int i = 0; i < WINDOW_WIDTH; ++i)
    //     SDL_RenderDrawPoint(renderer, i, i);

    readTileROM(renderer);
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

void readTileROM(SDL_Renderer *renderer) {
    // std::ifstream ifd("pacmam.5e", std::ifstream::in);
    std::ifstream ifd;
    ifd.open("../pacman/pacman.5e", ios::binary | ios::in | ios::ate);

    if (!ifd) {
        std::cerr << "Could not open the file!" << std::endl;
        throw exception();
    }

    ifd.seekg(0, ios::end);
    int size = ifd.tellg();
    cout << "file size:" << size << endl;
    vector<char> buffer;
    buffer.resize(size);
    ifd.seekg(0, ios::beg);
    ifd.read(buffer.data(), size); //Fill buffer with the ROM data
    //dumphex(buffer); // Print out buffer's content
    drawPixels(buffer, renderer);
}


void drawPixels(const std::vector<char> & buffer, SDL_Renderer *renderer) {

    // Since screen is made up of 224 x 288 pixels (rotated)
    // and each tile is a 8x8 pixel 4-color square, we could then split the ROM contents
    // into a 28 x 36 tileset.
    // Also, given that each tile is a 8x8 pixel and each pixel has 2-bit color depth, then each
    // tile takes 8x8 pixels --> 64 pixels at 2bits each = 128bit --> 16bytes per tile
    // |--------
    // | 4 | 4 |
    // |--------
    // | 4 | 4 |
    // |--------
    int offset_x = 0;
    int offset_y = 0;

    for(int row = 0; row < 16; row++)
    {
        for(int column = 0; column < 16; column++)
        {        
            auto array_offset = 16 * (column + row * 16) ;
            std::vector<char> tile (std::next(std::begin(buffer),0 + array_offset),
                                    std::next(std::begin(buffer),16 + array_offset));

            offset_x = column * 8;
            offset_y = row * 8;

            // cout << "tile size" << tile.size() << endl;

            for(int t = 0; t < 8; t++) // Eight columns
            {
                std::bitset<8> tile_bits (tile[t * 2]); 
                cout << tile_bits << " ";
                std::bitset<8> tile_bits_1 (tile[(t * 2)+1]); 
                cout << tile_bits_1 << endl;

                // cout << "column: " << t << endl;
                //We need four bytes per 4 pixels , because each pixel has a 8-bit color depth
                // thus having 8 bit planes for 4 pixels
                int off = 8;
                //Get lowest four bits, each bit corresponding to a different pixel, plane 0
                uint8_t low_nibble = tile[t + off] & 0x0F;
                //Get hightest four bits, each bit corresponding to a different pixel, plane 1
                uint8_t high_nibble = (tile[t + off] >> 4) & 0x0F;
               
                // std::bitset<8> low_nibbl (low_nibble);
                // cout << "low nibble " << low_nibbl << " ";
                // std::bitset<8> high_nibbl (high_nibble); 
                // cout << "high niblle " << high_nibbl << endl;

                Pixel first_pixel = Pixel(low_nibble & 0x01, high_nibble & 0x01);
                Pixel second_pixel = Pixel((low_nibble & 0x02) >> 1, (high_nibble & 0x02) >> 1);
                Pixel third_pixel = Pixel((low_nibble & 0x04) >> 2, (high_nibble & 0x04) >> 2);
                Pixel fourth_pixel = Pixel( (low_nibble & 0x08) >> 3, (high_nibble & 0x08) >> 3);

                Pixel pixels[] = { first_pixel, 
                                    second_pixel, 
                                    third_pixel, 
                                    fourth_pixel,
                                   };

                for(int c=0; c < 4; c++) {
                    // std::cout << pixels[c] << std::endl;
                    SDL_SetRenderDrawColor(renderer, pixels[c].color.red, pixels[c].color.green, pixels[c].color.blue , 255);
                    SDL_RenderDrawPoint(renderer, offset_x + t, offset_y + c + 4);
                }
            }

             for(int t = 0; t < 8; t++) // Eight columns
            {
                
                 //Get lowest four bits, each bit corresponding to a different pixel, plane 0
                uint8_t low_nibble = tile[t] & 0x0F;
                //Get hightest four bits, each bit corresponding to a different pixel, plane 1
                uint8_t high_nibble = (tile[t] >> 4) & 0x0F;

                // std::bitset<8> low_nibbl (low_nibble);
                // cout << "low nibble " << low_nibbl << " ";
                // std::bitset<8> high_nibbl (high_nibble); 
                // cout << "high niblle " << high_nibbl << endl;

                Pixel first_pixel = Pixel(low_nibble & 0x01, high_nibble & 0x01);
                Pixel second_pixel = Pixel((low_nibble & 0x02) >> 1, (high_nibble & 0x02) >> 1);
                Pixel third_pixel = Pixel((low_nibble & 0x04) >> 2, (high_nibble & 0x04) >> 2);
                Pixel fourth_pixel = Pixel( (low_nibble & 0x08) >> 3, (high_nibble & 0x08) >> 3);

                Pixel pixels[] = { first_pixel, 
                                    second_pixel, 
                                    third_pixel, 
                                    fourth_pixel,

                                 };

                for(int c=0; c < 4; c++) {
                    // std::cout << pixels[c] << std::endl;
                    SDL_SetRenderDrawColor(renderer, pixels[c].color.red, pixels[c].color.green, pixels[c].color.blue , 255);
                    SDL_RenderDrawPoint(renderer, offset_x + t, offset_y + c);
                }
            }
        }

    }

    SDL_RenderPresent(renderer);

}

void dumphex(const vector<char>& v)
{
    const int N = 16;
    const char hex[] = "0123456789ABCDEF";
    char buf[N*4+5+2];
    for (int i = 0; i < v.size(); ++i)
    {
        int n = i % N;
        if (n == 0)
        {
            if (i)
                puts(buf);
            memset(buf, 0x20, sizeof(buf));
            buf[sizeof(buf) - 2] = '\n';
            buf[sizeof(buf) - 1] = '\0';
        }
        unsigned char c = (unsigned char)v[i];
        buf[n*3+0] = hex[c / 16];
        buf[n*3+1] = hex[c % 16];
        buf[3*N+5+n] = (c>=' ' && c<='~') ? c : '.';
    }
    puts(buf);
}
