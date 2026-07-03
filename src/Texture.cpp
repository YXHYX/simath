#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define TEXTURE_DITHERING

static mathT::mat4x4d bayerM(0, 12.0 / 16.0, 03.0 / 16.0, 15.0 / 16.0,
    08.0 / 16.0, 04.0 / 16.0, 11.0 / 16.0, 07.0 / 16.0,
    02.0 / 16.0, 14.0 / 16.0, 01.0 / 16.0, 13.0 / 16.0,
    10.0 / 16.0, 06.0 / 16.0, 09.0 / 16.0, 05.0 / 16.0);
//map texture colors to fixed colors once the texture is loaded
unsigned short Texture::colorMap(unsigned char r, unsigned char g, unsigned char b)
{
    auto distance2 = [=](mathT::vec3u a, mathT::vec3u b) {
        float red= pow(a.x - b.x, 2), green = 4 * pow(a.y - b.y, 2), blue = pow(a.z - b.z, 2);
        float meanR = (a.x+b.x)/2;
        return meanR<128 ? sqrt(2*red+green + 3*blue) : sqrt(3 * red + green + 2 * blue);
        };

    auto distance = [=](mathT::vec3u a, mathT::vec3u b) {

        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
        };
    
    float minDist = INFINITY;
    int index = 0;
    for(int i = 0; i < 16; i++)
    {
        float dist = distance2(mathT::vec3f(r,g,b), FIXED_COLORS[i]);
        if (dist < minDist)
        {
            index = i;
            minDist = dist;
        }
    }

    return index;
}
Texture::Texture(const Texture* t)
{
    this->data = t->data;
    this->reducedData = t->reducedData;
    this->width = t->width;
    this->height= t->height;
    this->channels= t->channels;

    this->path= t->path;
}

Texture::Texture(std::string p, int d): path(p), dither(d)
{
    data = nullptr;
    reducedData = nullptr;
    channels = 0;
    width = 0;
    height = 0;
}
Texture::Texture()
{
    data = nullptr;
    reducedData = nullptr;
    channels = 0;
    width = 0;
    height = 0;
}

void Texture::load()
{
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        throw"Error loading image.";
    }

    //rgb dither

    if(this->dither == 2)
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int ind = (i * height + j) * channels;
                float bayerVal = bayerM[i % 4][j % 4];
                mathT::vec3f mappedPixel(data[ind], data[ind + 1], data[ind + 2]);

                //mathT::vec3f mappedPixel(FIXED_COLORS[colorMap(data[ind], data[ind + 1], data[ind + 2])]);

                data[ind] = mappedPixel.x > bayerVal ? mappedPixel.x : 0;

                data[ind+1] = mappedPixel.y > bayerVal ? mappedPixel.y : 0;

                data[ind+2] = mappedPixel.z > bayerVal ? mappedPixel.z : 0;
            }
        }
    if (this->dither == 1)
    {
        float a = 7, b = 5, c = 3, d = 1;
        float sum = a + b + c + d;
        a /= sum;
        b /= sum;
        c /= sum;
        d /= sum;

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int ind = (i * height + j) * channels;
                mathT::vec3f mappedPixel(FIXED_COLORS[colorMap(data[ind], data[ind + 1], data[ind + 2])]);
                mathT::vec3f err(data[ind] - mappedPixel.x, data[ind + 1] - mappedPixel.y, data[ind + 2] - mappedPixel.z);

                err.x = abs(err.x);
                err.y = abs(err.y);
                err.z = abs(err.z);
                this->data[ind] = mappedPixel.x;
                this->data[ind + 1] = mappedPixel.y;
                this->data[ind + 2] = mappedPixel.z;
                if (mappedPixel.x == mappedPixel.y && mappedPixel.y == mappedPixel.z && mappedPixel.x == 0)
                    continue;
                if (i < width - 1)
                {
                    ind = ((i + 1) * height + j) * channels;

                    this->data[ind] += err.x * a;
                    this->data[ind + 1] += err.y * a;
                    this->data[ind + 2] += err.z * a;
                }
                if (j < height - 1)

                    ind = (i * height + j + 1) * channels;

                this->data[ind] += err.x * b;
                this->data[ind + 1] += err.y * b;
                this->data[ind + 2] += err.z * b;
                if (i > 0) {
                    ind = ((i - 1) * height + j + 1) * channels;

                    this->data[ind] += err.x * c;
                    this->data[ind + 1] += err.y * c;
                    this->data[ind + 2] += err.z * c;
                }
                if (i < width - 1) {
                    ind = ((i + 1) * height + j + 1) * channels;

                    this->data[ind] += err.x * d;
                    this->data[ind + 1] += err.y * d;
                    this->data[ind + 2] += err.z * d;
                }
            }
        }
    }

//save generated texture for debugging
#ifdef SAVEGENTEX  

    const auto now = std::chrono::system_clock::now();
    stbi_write_png(std::string("test" + std::to_string(now.time_since_epoch().count()) + ".png").c_str(),
        width, height, channels, data, width * channels);
#endif
    //remap the texture;
    this->reducedData = new unsigned short[width * height];
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            int ind = j * width + i;
            this->reducedData[ind] = colorMap(data[ind * channels], data[ind * channels + 1], data[ind * channels + 2]) << 4;
        }

    stbi_image_free(this->data);
}

Texture::~Texture()
{
    //this->path.clear();
    //stbi_image_free(data);
    //delete[] this->reducedData;
}

unsigned short Texture::getPixel(float x, float y)
{
    if (x > 1.f || x < 0.f || y > 1.f || y < 0.f)
        return 0;
    return this->reducedData[int(x*width)+int(y*height)*width-1];
}
