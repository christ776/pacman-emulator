#include "pixel.h"
using namespace std;


std::ostream& operator<<(std::ostream& os, Pixel::Color& obj) {
  return os << obj.red << obj.green << obj.blue << std::endl;
}

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