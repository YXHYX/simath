#pragma once

#include <tools.h>

class PerlinNoise
{
private:
	float* grid;
	mathT::vec2i size;
public:
	PerlinNoise()
	{
		grid = NULL;
	}
	~PerlinNoise()
	{
		if (grid != NULL)
			delete[]grid;
	}

	void generate(int h, int w, int octaves, float bias)
	{
		this->size.x = w;
		this->size.y = h;

		if (grid != NULL)
		{
			delete[] grid;
		}
		else
		{
			this->grid = new float[h * w];
			float* noise = new float[h * w];
			for(int i =0; i < w; i++)
				for (int j = 0; j < h; j++)
				{
					noise[j * w + i] = float(rand()%1024)/1024.f;
				}

			float sumScales = 0, temp = 1.f;

			sumScales += temp;
			for (int o = 0; o < octaves; o++)
			{
				temp /= bias;
				sumScales += temp;
			}
			for (int x = 0; x < w; x++)
				for (int y = 0; y < h; y++)
				{
					float val = 0.f;
					float scale = 1.f;
					
					for(int k = 0; k < octaves; k++)
					{
						int pitch = w >> k;

						int sampleX1 = int(x/pitch)*pitch, sampleY1= int(y / pitch) * pitch;
						int sampleX2 = (sampleX1+pitch)%w, sampleY2 = (sampleY1 + pitch)%w; //h
						float lambdaX = (float)(x-sampleX1)/float(pitch), lambdaY= (float)(y - sampleY1) / float(pitch);
						
						float s1 = (1-lambdaX) * noise[sampleY1 * w + sampleX1] + lambdaX * noise[sampleY1 * w + sampleX2];
						float s2 = (1-lambdaX) * noise[sampleY2 * w + sampleX1] + lambdaX * noise[sampleY2 * w + sampleX2];

						val += (lambdaY * (s2-s1) + s1)*scale;

						scale /= bias;
					}
					val /= sumScales;
					grid[w * y + x] = val;
				}
		}
	}
	float getValue(int x, int y)
	{
		if(grid != NULL)
		{
			if (x > size.x|| x < 0.f || y > size.y || y < 0.f)
				return -1;

			return this->grid[int(size.x * y + x)];
		}
		return-1;
	}
};

