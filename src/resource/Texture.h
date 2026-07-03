#pragma once

//#define TEXTURE_DITHERING

#include <string>
#include <vector>
#include <chrono>
#include <format>
#include "tools.h"
/*
* [{"hex":"#ff0000"},{"hex":"#00ff00"},{"hex":"#0000ff"},{"hex":"#ffff00"},{"hex":"#ff00ff"},{"hex":"#00ffff"},{"hex":"#c6c6c6"},{"hex":"#ffffff"},{"hex":"#000000"},{"hex":"#808080"},{"hex":"#7f0000"},{"hex":"#007f00"},{"hex":"#00007f"},{"hex":"#7f7f00"},{"hex":"#7f007f"},{"hex":"#007f7f"}]
*/

// "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "
//
namespace graphics
{
	enum Colors
	{

		BLACK = 0x0000,
		BLUE = 0x0001,
		GREEN = 0x0002,
		CYAN = 0x0003,
		RED = 0x0004,
		PURPLE = 0x0005,
		GOLD = 0x0006,
		LIHGT_GREY = 0x0007,
		//INTENSIFIED
		GREY = 0x0008,
		LIGHT_BLUE = 0x0009,
		LIME = 0x000A,
		LIGHT_CYAN = 0x000B,
		LIGHT_RED = 0x000C,
		LIGHT_PURPLE = 0x000D,
		YELLOW = 0x000E,
		WHITE = 0x000F,


		//BACKGROUND COLOR
		B_BLACK = 0x0000,
		B_BLUE = 0x0010,
		B_GREEN = 0x0020,
		B_CYAN = 0x0030,
		B_RED = 0x0040,
		B_PURPLE = 0x0050,
		B_YELLOW = 0x0060,
		B_LIGHT_GREY = 0x0070,
		//INTENSIFIED
		B_GREY = 0x0080,
		B_LIGHT_BLUE = 0x0090,
		B_LIME = 0x00A0,
		B_LIGHT_CYAN = 0x00B0,
		B_LIGHT_RED = 0x00C0,
		B_LIGHT_PURPLE = 0x00D0,
		B_GOLD = 0x00E0,
		B_WHITE = 0x00F0,
	};
}
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
	int dither = 0;
    unsigned short colorMap(unsigned char r, unsigned char g, unsigned char b);
public:
    std::string path;
	//copy constructor
    Texture(const Texture* t);
	// p path to texture, d dithering option ( 0 = nothing, 1 = floyd steinberg, 2 = bayer ordered 4x4)
    Texture(std::string p, int d = 0);
    Texture();
    void load();
    virtual ~Texture();

    int width, height, channels;
    unsigned char* data;
    unsigned short* reducedData;
    unsigned short getPixel(float x, float y);
};

