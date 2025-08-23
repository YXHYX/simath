#pragma once

#include <tools.h>
//works with 2d

//bound X (-2.00, 0.47))
// bound Y (-1.12, 1.12)
class Mandelbrot
{
private:
	mathT::vec2f center;
	mathT::vec2f size;
	mathT::vec2i gridSize;
	int maxIterations;
public:
	Mandelbrot(int w, int h, int maxIt = 200)
	:	gridSize(w,h), maxIterations(maxIt)
	{
		center.x = 0;
		center.y = 0;
		size.x = 2;
		size.y = 1;

	}
	~Mandelbrot()
	{

	}
	void updateInput(float dt)
	{
		if (GetAsyncKeyState(0x57) & 0x8000) //w
			center.y -=  5*size.y / float(gridSize.y);
		if (GetAsyncKeyState(0x53) & 0x8000) //s
			center.y += 5 * size.y / float(gridSize.y);
		if (GetAsyncKeyState(0x41) & 0x8000) //A
			center.x -= 10 * size.x / float(gridSize.x);
		if (GetAsyncKeyState(0x44) & 0x8000) //D
			center.x += 10 * size.x / float(gridSize.x);

		//0x46
		if (GetAsyncKeyState(0x46) & 0x8000) //F
			this->zoom(1.01);
		if (GetAsyncKeyState(0x47) & 0x8000) //G
			this->zoom(0.99);

		//dont touch, 
		//this->maxIterations = -100 *log(size.x + size.y);

	}
	void setCenter(mathT::vec2f c)
	{
		this->center = c;
	}
	void zoom(float factor) 
	{
		if(factor != 0.f)
			this->size /= factor;
	}
	unsigned int getPixel(int i, int j)
	{
		float x0 = center.x-size.x/2 + (float(i)*size.x/float(gridSize.x)), y0 = center.y - size.y / 2 + (float(j) * size.y / float(gridSize.y));
		float x = 0, y= 0;
		int iteration = 0;
		while(x*x + y*y < 4 && iteration < this->maxIterations)
		{
			float tmp = x * x - y * y + x0;
			y = 2 * x * y + y0;
			x = tmp;
			iteration++;
		}
		
		//int col = iteration%16;
		int col = int(2*16*float(maxIterations - iteration) / float(maxIterations)) %16;
		return col << 4;
	}

};

