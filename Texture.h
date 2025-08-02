#pragma once

//#define TEXTURE_DITHERING

#include <string>
#include <vector>
#include "tools.h"
/*
* [{"hex":"#ff0000"},{"hex":"#00ff00"},{"hex":"#0000ff"},{"hex":"#ffff00"},{"hex":"#ff00ff"},{"hex":"#00ffff"},{"hex":"#c6c6c6"},{"hex":"#ffffff"},{"hex":"#000000"},{"hex":"#808080"},{"hex":"#7f0000"},{"hex":"#007f00"},{"hex":"#00007f"},{"hex":"#7f7f00"},{"hex":"#7f007f"},{"hex":"#007f7f"}]
*/
const mathT::vec3uc FIXED_COLORS[16] =
{
    mathT::vec3uc(0,0,0), //BLACk 0
    mathT::vec3uc(0,0,127), //BLUE 1 
    mathT::vec3uc(0,127,0), //GREEN 2
    mathT::vec3uc(0,127,127), //CYAN 3
    mathT::vec3uc(127,0,0), //RED1 4
    mathT::vec3uc(127,0,127), //PURPLE 5
    mathT::vec3uc(127,127,0), // GOLD 14
    mathT::vec3uc(192,192,192), //LIGHT GREY 7
    mathT::vec3uc(128,128,128), //GREY 8
    mathT::vec3uc(0,0,255), // L BLUE 9
    mathT::vec3uc(0,255,0), // LIME 10
    mathT::vec3uc(0,255,255), // L CYAN 11
    mathT::vec3uc(255,0,0), // L RED 12
    mathT::vec3uc(255,0,255), // L PURPLE 13
    mathT::vec3uc(255,255,0), //YELLOW 6
    mathT::vec3uc(255,255,255) // WHITE 15
};

class Texture
{
private:
    unsigned short colorMap(unsigned char r, unsigned char g, unsigned char b);
public:
    std::string path;
    Texture(const Texture* t);
    Texture(std::string p);
    Texture();
    void load();
    virtual ~Texture();

    int width, height, channels;
    unsigned char* data;
    unsigned short* reducedData;
    unsigned short getPixel(float x, float y);
};

