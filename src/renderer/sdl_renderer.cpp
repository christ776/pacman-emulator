#include "sdl_renderer.h"
#include <bitset>
#include <iostream>
#include "pixel.h"

using namespace std;

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