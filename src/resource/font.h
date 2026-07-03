#pragma once

#include <fstream>
#include <iterator>
#include <vector>
#include <filesystem>
#include <string>
#include <unordered_map>

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
};

class Font
{
private:
	//path to the .tdf font
	std::filesystem::path path;
	
	//font info
	Font_Info f_info;
	//map any character to its glyph
	std::unordered_map<char, std::vector<std::vector<char>>> glyphs;


public:
	Font(std::filesystem::path path);
	~Font();

	Font_Info* getInfo();
	std::vector<std::vector<char>>* getGlyph(char c);
};

