#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
    if(index > 15 || index < 0)
    {
        int balls;
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

Texture::Texture(std::string p): path(p)
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
#ifdef TEXTURE_DITHERING
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            int ind = (i*height + j) * channels;
            mathT::vec3f mappedPixel(FIXED_COLORS[ colorMap(data[ind], data[ind + 1], data[ind + 2]) ]);
            mathT::vec3f err(data[ind] - mappedPixel.x, data[ind + 1] - mappedPixel.y, data[ind + 2] - mappedPixel.z);
            

            this->data[ind] = mappedPixel.x,
                this->data[ind + 1] = mappedPixel.y,
                this->data[ind + 2] = mappedPixel.z;
            if (i < width-1)
            {
                ind = ((i+1) * height + j ) * channels;

                this->data[ind] += err.x * 7 / 16;
                this->data[ind + 1] += err.y * 7 / 16;
                this->data[ind + 2] += err.z * 7 / 16;
            }
            if (j < height - 1) {

                ind = (i * height + j+1) * channels;

                this->data[ind] += err.x * 5 / 16;
                this->data[ind + 1] += err.y * 5 / 16;
                this->data[ind + 2] += err.z * 5 / 16;
                if( i >0){
                    ind = ((i-1) * height + j+1) * channels;

                    this->data[ind] += err.x * 3 / 16;
                    this->data[ind + 1] += err.y * 3 / 16;
                    this->data[ind + 2] += err.z * 3 / 16;
                }
                if (i < width - 1) {
                    ind = ((i+1) * height + j+1) * channels;

                    this->data[ind] += err.x / 16;
                    this->data[ind + 1] += err.y / 16;
                    this->data[ind + 2] += err.z / 16;
                }
            }     
        }
#endif
    
    
    //save cel shaded texture
    int w2 = width, h2 = height;
    unsigned char* data2 = new unsigned char[w2 * h2 * channels];

    for (int i = 0; i < w2; i++)
        for (int j = 0; j < h2; j++)
            for(int k = 0; k < channels; k++)
                data2[(j * w2 + i) * channels + k] = data[(j * w2 + i) * channels + k];
    stbi_write_png("test1.png", w2, h2, channels, data2, w2 * channels);

    //remap the texture;
    this->reducedData = new unsigned short[width * height];
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            int ind = j * width + i;
            this->reducedData[ind] = colorMap(data[ind * channels], data[ind * channels + 1], data[ind * channels + 2]) << 4;
        }

    //dithering effect
    
    

    
    /*
    */
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
