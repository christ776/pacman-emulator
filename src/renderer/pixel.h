#include <iostream>
#include <bitset>

/**
 * Each pixel is made of 2 bits, thus having a total of 4 values (out of  16?)
 * For instance, we can use the following color palette until we figure the real colors
 * - 00: blue
 * - 01: red
 * - 10: yellow
 * - 11: green 
 **/
struct Pixel {

    struct Color { 
      int red, green, blue;
    } color;

    Pixel(uint8_t low_nibble, uint8_t high_nibble);
};

// std::ostream& operator<<(std::ostream& os, Pixel::Color& obj) {
//   return os << obj.red << obj.green << obj.blue << std::endl;
// }
