/*
* TODO
* Fix clipping done
* make z buffer depth testing done
* mess around with the view matrix done
* quaternions done
* transformations done
* 
* texturing
* shading
* light etc
* 
* try celshading 
*/



#pragma once

#include <windows.h>
#include "camera.h"
#include "Texture.h"
#include <vector>

//normal graphics buffer
#define HEIGHT 100
#define WIDTH  400
#define OFFSET_X 0
#define OFFSET_Y 0

//gui buffer
#define GHEIGHT 30
#define GWIDTH 400
//debu buffer
#define DHEIGHT 100
#define DWIDTH 30

#define GRAPHICS_DEBUG

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
		void setTexture(Texture const&  t)
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
	struct Object 
	{
		//set of all vertices 
		std::vector<vec3f> vertices;
		std::vector<vec2f> texcoords;
		std::shared_ptr<Texture> texture;
		//define each triangle to draw
		std::vector<unsigned int> indices;
		//define the color of each triangle
		std::vector<unsigned int> colors;
		mat4x4d model;
	};
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

	//fix up
	enum Styles 
	{
		//LINE AT TOP
		LTOP = 0x0200
	};

	// https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix.html

	class Graphics
	{
	private:
		//dummy variable
		float dt;
	

		//
		int height, width, offset_x, offset_y;
		int gheight, gwidth, goff_x = 0, goff_y = HEIGHT;
		int dheight, dwidth, doff_x = WIDTH, doff_y = 0;
		
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

		//vector to store and sort triangles depending on depth, visibility etc...
		std::vector<Triangle> triangles;
		
		Camera camera;
		void computeDepth();
		bool pointInTriangle(vec2f p, Triangle t);
		int safeIndex(int i, int j, int w = -1, int h = -1);
	public:
		Graphics(int h = HEIGHT, int w = WIDTH, int ox = OFFSET_X, int oy = OFFSET_Y, int gw = GWIDTH, int gh = GHEIGHT, int dw = DWIDTH, int dh = DHEIGHT);
		~Graphics();

		CHAR_INFO* getBuffer();
		COORD getBufferSize();
		COORD getBufferOffset();

		//DRAWING FUNCTIONS
		
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
		
		void drawObject(Object& obj, char c, unsigned short attr);
		void drawContour(int thickness, char c, unsigned short attr);
		void updateInput();
		//Write the buffer onto the console and display it
		void render();
	};

}