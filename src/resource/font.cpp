#include "font.h"

Font::Font(std::filesystem::path filepath)
{
    if (!std::filesystem::is_regular_file(filepath))
        throw "FONT::NOT_A_FILE_ERROR: " + filepath.string();

    std::ifstream f(filepath, std::ios_base::binary);
    if (!f.is_open())
        throw "FONT::OPEN_FAILED";

    size_t length = std::filesystem::file_size(filepath);
    std::vector<char> fbytes(length);
    f.read(fbytes.data(), length);
    f.close();

    // Look for the magic number start '\x55\xAA\x00\xFF'
    int start = 0;
    bool found = false;
    while (start < length - 4)
    {
        if (fbytes[start] == '\x55' && fbytes[start + 1] == '\xAA' &&
            fbytes[start + 2] == '\x00' && fbytes[start + 3] == '\xFF') {
            found = true;
            break;
        }
        start++;
    }

    if (!found) return; // Or throw header missing error

    // Load header metadata
    for (int i = 0; i < 4; i++)
        this->f_info.magic_constant[i] = fbytes[start + i];
    for (int i = 4; i < 20; i++)
        this->f_info.font_name[i - 4] = fbytes[start + i];

    this->f_info.font_type = fbytes[start + 21];
    this->f_info.space_count = fbytes[start + 22];

    // FIX: Safely combine little-endian bytes using unsigned cast and shift by 8
    uint8_t len_low = static_cast<uint8_t>(fbytes[start + 23]);
    uint8_t len_high = static_cast<uint8_t>(fbytes[start + 24]);
    this->f_info.font_length = len_low | (len_high << 8);

    int offsets_start = start + 25;
    std::vector<unsigned short> offsets(94);

    for (int i = 0; i < 94; i++) {
        // FIX: Handle sign-extension correctly here too
        uint8_t off_low = static_cast<uint8_t>(fbytes[offsets_start + (i * 2)]);
        uint8_t off_high = static_cast<uint8_t>(fbytes[offsets_start + (i * 2) + 1]);
        offsets[i] = off_low | (off_high << 8);
    }

    int data_start = start + 213;

    for (int i = 0; i < 94; i++)
    {
        int offset = offsets[i];

        // Empty character code mapping check

        int k = data_start + offset;
        if (offset == 0xFFFF)
            continue;
        uint8_t char_width = static_cast<uint8_t>(fbytes[k]);
        uint8_t char_height = static_cast<uint8_t>(fbytes[k + 1]);

        std::vector<char> row;
        k += 2;
        while (k < length) {
            char readByte = fbytes[k];

            if (this->f_info.font_type == 2) { // Color Font
                if (!readByte) { // End of Entry
                    if (!row.empty())
                        this->glyphs[33 + i].push_back(row);
                    break;
                }
                else if (readByte == 0x0D) { // New Row
                    this->glyphs[33 + i].push_back(row);
                    row.clear();
                    k++;
                }
                else {
                    row.push_back(readByte);
                    k += 2; // Skip color attribute metadata slot byte
                }
            }
            else { // Outline or Block Font
                if (!readByte) {
                    if (!row.empty())
                        this->glyphs[33 + i].push_back(row);
                    break;
                }
                else if (readByte == 0x0D) {
                    this->glyphs[33 + i].push_back(row);
                    row.clear();
                    k++;
                }
                else {
                    row.push_back(readByte);
                    k++;
                }
            }
        }
    }

	//done with reading all the matrices for each glyph
	//jump by "font size" if there are more fonts???
	//look for new font

}

Font::~Font()
{
	//get rid of all the stuff
}

Font_Info* Font::getInfo()
{
	return &this->f_info;
}

std::vector<std::vector<char>>* Font::getGlyph(char c)
{
	if(this->glyphs.count(c))
		return &this->glyphs.at(c);
	return nullptr;
}
