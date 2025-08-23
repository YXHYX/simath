/*
* TODO
* Fix clipping done
* make z buffer depth testing done
* mess around with the view matrix done
* quaternions done
* transformations done
* 
* texturing semi-done? add dithering??
* 
* shading 
* light etc
* 
* cool math shit:
*	hopf's fibration
* 
* try celshading 
*/



#pragma once
#include <windows.h>
#include "camera.h"
#include "shapes/Object.h"
#include "shapes/Line.h"
#include "shapes/Point.h"
#include "shapes/Cube.h"
#include <vector>

//normal graphics buffer
#define HEIGHT 100
#define WIDTH  400

//gui buffer
#define GHEIGHT 30
#define GWIDTH 400
//debu buffer
#define DHEIGHT 130
#define DWIDTH 70

//#define GRAPHICS_DEBUG

using namespace mathT;

namespace graphics
{
	struct Triangle
	{
		vec3f vertices[3];
		vec2f texcoords[3];

		std::shared_ptr<Texture> texture;
		bool visible = true;

		Triangle(vec3i v1, vec3i v2, vec3i v3)
		{
			vertices[0] = v1;
			vertices[1] = v2;
			vertices[2] = v3;
		}
		Triangle(vec3f v1, vec3f v2, vec3f v3)
		{
			vertices[0] = v1;
			vertices[1] = v2;
			vertices[2] = v3;
		}
		~Triangle()
		{
			int i = this->texture.use_count();
		}
		void setTexture(Texture const& t)
		{
			this->texture = std::make_shared<Texture>(&t);
		}
		void setTexCoords(vec2f x, vec2f y, vec2f z)
		{
			texcoords[0] = x;
			texcoords[1] = y;
			texcoords[2] = z;
		}

		unsigned short attr = 14;
		char Char = '.';
	};

	//fix up
	enum Styles 
	{
		//LINE AT TOP
		LTOP = 0x0200
	};
	class Graphics
	{
	private:
		//dummy variable
		float dt;
	

		// buffer parameter data, g for gui, d for debug
		int height, width;
		int gheight, gwidth;
		int dheight, dwidth;
		
		HANDLE hConsole;
		
		//Get the console's handle and set up the graphics settings
		void initialize();

		//BUFFER DATA
		CHAR_INFO * gBuffer;
		float* zBuffer;
		COORD gBufferSize;
		COORD gBufferOffset;
		SMALL_RECT gBufferRegion;

		CHAR_INFO* guiBuffer;
		COORD guiSize;
		COORD guiOffset;
		SMALL_RECT guiRegion;

		CHAR_INFO* debugBuffer;
		COORD debugSize;
		COORD debugOffset;
		SMALL_RECT debugRegion;

		//vector to store and sort triangles, lines, points depending on depth, visibility etc...
		std::vector<Triangle> triangles;
		std::vector<Line> lines;
		std::vector<Point> points;
		
		Camera camera;
		void computeDepth();
		bool pointInTriangle(vec2f p, Triangle t);
		int safeIndex(int i, int j, int w = -1, int h = -1);
	public:
		Graphics(int h = HEIGHT, int w = WIDTH, int gw = GWIDTH, int gh = GHEIGHT, int dw = DWIDTH, int dh = DHEIGHT);
		~Graphics();

		CHAR_INFO* getBuffer();
		COORD getBufferSize();
		COORD getBufferOffset();

		//DRAWING FUNCTIONS FOR 2D PURPOSES
		
		//clear the buffer with the desired character and attributes (leave blank for blank canvas)
		void clearBuffer(char c = ' ', unsigned short attr = Colors::BLACK);
		//Draw rectangle from (x,y) to point (x+w, y+h) with desired character and attributes
		void drawRect(int x, int y, int w, int h, char c, unsigned short attr);
		//Draw circle with center (x,y) and radius r with desired character and attributes
		void drawCircle(int x, int y, int r, char c, unsigned short attr);

		//Draw a triangle
		void drawTriangle(vec2i p1, vec2i p2, vec2i p3, char c, unsigned short attr);

		//Draw a line with specified attributes
		void drawLine(int x1, int y1, int x2, int y2, char c, unsigned short attr);


		//Fill rectangle from (x,y) to point (x+w, y+h) with desired character and attributes
		void fillRect(int x, int y, int w, int h, char c, unsigned short attr);
		//Fill circle with center (x,y) and radius r with desired character and attributes
		void fillCircle(int x, int y, int r, char c, unsigned short attr);
		//Fill a triangle
		void fillTriangle(vec2i p1, vec2i p2, vec2i p3, char c, unsigned short attr);
		//Set a pixel on the buffer
		void setPixel(int x, int y, char c, unsigned int attr);
		//Draw a contour around buffers
		void drawContour(int thickness, char c, unsigned short attr);
		/**********************/

		// drawing functions for 3D

		vec2u getDepth(float z, unsigned short attr);
		void drawObject(Object& obj, char c, unsigned short attr);
		void drawLine(Line l);
		void drawPoint(Point p);

		// gui/debug functions

		void printGui();

		void printDebug();

		void updateInput();
		//Write the buffer onto the console 
		void render();
		// display the buffer content
		void display();
	};

}