#include "parser.h"
#include <vector>
#include <fstream>
#include <iostream>


vector<char> readTileROM(std::string romPath) {
    // std::ifstream ifd("pacmam.5e", std::ifstream::in);
    std::ifstream ifd;
    ifd.open(romPath, ios::binary | ios::in | ios::ate);

    if (!ifd) {
        std::cerr << "Could not open the file!" << std::endl;
        throw exception();
    }

    ifd.seekg(0, ios::end);
    int size = ifd.tellg();
    //cout << "file size:" << size << endl;

    vector<char> buffer;

    buffer.resize(size);
    ifd.seekg(0, ios::beg);
    ifd.read(buffer.data(), size); //Fill buffer with the ROM data
    return buffer;
}