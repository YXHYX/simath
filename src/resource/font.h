#pragma once

#include <fstream>
#include <iterator>
#include <vector>
#include <filesystem>
#include <string>
#include <unordered_map>
#include "tools.h"
//todo:
// make a font loader to load .TDF fonts
// add support for color shading
// resizing fonts?

struct Font_Info {
	char magic_constant[4];
	char font_name[20];
	char font_type;
	char space_count;
	int font_length;
	mathT::vec2u maxSize= mathT::vec2u(0,0);
	mathT::vec2u minSize= mathT::vec2u(5000,5000);
};

class Font
{
private:
	//path to the .tdf font
	std::filesystem::path path;
	
	//font info
	Font_Info f_info;
	//map any character to its glyph
	std::unordered_map<char, std::vector<std::vector<std::pair<char,unsigned short>>>> glyphs;


public:
	Font(std::filesystem::path path);
	~Font();

	Font_Info* getInfo();
	std::vector<std::vector<std::pair<char, unsigned short>>>* getGlyph(char c);
};

