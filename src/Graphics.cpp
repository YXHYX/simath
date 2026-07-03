#include "Graphics.h"

using namespace graphics;

void Graphics::initialize()
{
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	this->gBufferSize.X = this->width;
	this->gBufferSize.Y = this->height;

	this->gBufferOffset.X = 0;
	this->gBufferOffset.Y = 0;
	
	this->gBufferRegion.Top = 0;
	this->gBufferRegion.Left = 0;
	this->gBufferRegion.Right = this->gBufferRegion.Left + this->gBufferSize.X;
	this->gBufferRegion.Bottom = this->gBufferRegion.Top + this->gBufferSize.Y;

	this->gBuffer = new CHAR_INFO[this->gBufferSize.X * this->gBufferSize.Y];
	this->zBuffer = new double[this->gBufferSize.X * this->gBufferSize.Y];


	this->guiSize.X = this->gwidth;
	this->guiSize.Y = this->gheight;
	//test with 1 buffer but diff offsets???
	// or use just one buffer and draw all shit there
	this->guiOffset.X = 0;
	this->guiOffset.Y = 0;

	this->guiRegion.Top = this->height;
	this->guiRegion.Left = 0;
	this->guiRegion.Right = this->guiRegion.Left + this->guiSize.X;
	this->guiRegion.Bottom = this->guiRegion.Top + this->guiSize.Y;

	this->guiBuffer = new CHAR_INFO[this->guiSize.X * this->guiSize.Y];


	this->debugSize.X = this->dwidth;
	this->debugSize.Y = this->dheight;

	this->debugOffset.X = 0;
	this->debugOffset.Y = 0;

	this->debugRegion.Top = 0;
	this->debugRegion.Left = this->width;
	this->debugRegion.Right = this->debugRegion.Left + this->debugSize.X;
	this->debugRegion.Bottom = this->debugRegion.Top + this->debugSize.Y;

	this->debugBuffer = new CHAR_INFO[this->debugSize.X * this->debugSize.Y];
}

//calculate depth buffer?
void graphics::Graphics::computeDepth()
{

	//for triangles
	for (auto e : triangles)
	{
		// project the triangle
		
		// calculate the depth of the pixel
		
		// update it if its less than the one in the depth buffer

		// set the attributes in the corresponding pixel in the g buffer

		//remake this slow stuff, optimize it more and never look at it again
		vec4d pp1(e.vertices[0].x, e.vertices[0].y, e.vertices[0].z, 1),
			pp2(e.vertices[1].x, e.vertices[1].y, e.vertices[1].z, 1),
			pp3(e.vertices[2].x, e.vertices[2].y, e.vertices[2].z, 1);

		pp1 = (*camera.getView()) * pp1;
		pp2 = (*camera.getView()) * pp2;
		pp3 = (*camera.getView()) * pp3;
		if (pp1.w != 1)
			pp1 /= pp1.w;
		if (pp2.w != 1)
			pp2 /= pp2.w;
		if (pp3.w != 1)
			pp3 /= pp3.w;
		
		//needed for inverse interpolating and finding the z on a given point inside the triangle (using barycentric weights)
		double z1 = pp1.z, z2 =pp2.z, z3 = pp3.z;

		//if any are behind the screen dont bother rendering
		//if (z1 <= 0 && z2 <= 0 && z3 <= 0)
			//continue;

		//project on the screen plane
		pp1 = *camera.getProjection() * pp1;
		pp2 = *camera.getProjection() * pp2;
		pp3 = *camera.getProjection() * pp3;
		
		if (pp1.w != 1 && pp1.w != 0)
			pp1 /= pp1.w;
		if (pp2.w != 1 && pp2.w != 0)
			pp2 /= pp2.w;
		if (pp3.w != 1 && pp3.w != 0)
			pp3 /= pp3.w;


		//recentering the projected point to raster space
		pp1.x += h_width;
		pp1.y = pp1.y / 2 + q_height;

		pp2.x += h_width;
		pp2.y = pp2.y / 2 + q_height;

		pp3.x += h_width;
		pp3.y = pp3.y / 2 + q_height;
		//define a bounding box to do calculations more fast

		//clipping happens here
		vec3f pmax,pmin;
		pmax.x = min(max(max(pp1.x, pp2.x), pp3.x), width);
		pmax.y = min(max(max(pp1.y, pp2.y), pp3.y), height);

		pmin.x = max(min(min(pp1.x, pp2.x), pp3.x), 0);
		pmin.y = max(min(min(pp1.y, pp2.y), pp3.y), 0);

		double denominator = ((pp2.y - pp3.y) * (pp1.x - pp3.x) + (pp3.x - pp2.x) * (pp1.y - pp3.y));
		if (denominator == 0)
			continue;
		for (int i = pmin.x; i <= pmax.x; i++)
		{
			for (int j = pmin.y; j <= pmax.y; j++)
			{
				//if point in triangle
				//calculate the barycentric weights of the pixel
				double a = ((pp2.y - pp3.y) * (i - pp3.x) + (pp3.x - pp2.x) * (j - pp3.y)) / denominator;
				double b = ((pp3.y - pp1.y) * (i - pp3.x) + (pp1.x - pp3.x) * (j - pp3.y)) / denominator;
				double c = 1 - a - b;

				// Check if all barycentric coordinates
				// are non-negative
				if (a >= 0 && b >= 0 && c >= 0)
				{
					//retrieve the z coordinate of the point of the projected point thru interpolation?
					float w1 = a / z1, w2 = b / z2, w3 = c / z3;
					float z = 1 / (w1 + w2 + w3);
					if (z < 0)
						continue;
					//make sure the index is not exceeding the limited space
					int K = safeIndex(i, j);
					if (K != -1)
					{	//if its the closest triangle to the screen, render it
						if (this->zBuffer[K] >= z)
						{
							this->zBuffer[K] = z;
							this->gBuffer[K].Char.UnicodeChar = e.Char;
							this->gBuffer[K].Attributes = e.attr;

							//map the texture
							if (e.texture.get())
							{
								vec2f uv = (e.texcoords[0] * w1 + e.texcoords[1] * w2 + e.texcoords[2] * w3) * z;
								this->gBuffer[K].Char.UnicodeChar = this->getDepth(z, e.attr).x;// e.Char;
								this->gBuffer[K].Attributes = e.texture->getPixel(uv.x, uv.y);
							}
						}
					}
				}
			}
		}
	}
	//for lines
	for(auto e: this->lines)
	{
		vec4d p1(-e.getP1().x, -e.getP1().y, -e.getP1().z, 1);
		vec4d p2(-e.getP2().x, -e.getP2().y, -e.getP2().z, 1);
		p1 = *camera.getView() * p1;
		p2 = *camera.getView() * p2;

		if (p1.w != 1)
			p1 /= p1.w;
		if (p2.w != 1)
			p2 /= p2.w;

		float z1 = p1.z, z2 = p2.z;
		if (z1 < 0 || z2 < 0)
			continue;


		p1 = *camera.getProjection() * p1;
		p2 = *camera.getProjection() * p2;
		
		if (p1.w != 1)
			p1 /= p1.w;
		if (p2.w != 1)
			p2 /= p2.w;

		p1.x += h_width;
		p1.y = p1.y / 2 + q_height;
		
		p2.x += h_width;
		p2.y = p2.y / 2 + q_height;

		vec2i maxp(min(max(p1.x,p2.x),width), min(max(p1.y, p2.y),height)), minp(max(min(p1.x, p2.x),0), max(min(p1.y, p2.y),0));
		if (minp.x >= width || minp.y >= height || maxp.x < 0 || maxp.y < 0)
			continue;

		//instead of a bounding box like triangles, better to parametrically slide through the line by a parameter lambda
		int numPoints = norm<float>(maxp-minp);
		
		for (int r = 0; r <= numPoints; r++)
		{
			float lambda = float(r) / numPoints;
			vec4d point = p1 * lambda + p2 * (1 - lambda);

			float z = 1/((1-lambda)/z2 + lambda/z1);
			if ((point.x < 0 || point.x > width) || (point.y < 0 || point.y > height) || z < 0)
				continue;

			int K = safeIndex(point.x, point.y);
			if (K > 0)
				if (this->zBuffer[K] > z)
				{
					//filter the depth
					this->gBuffer[K].Attributes = e.getColor();// this->getDepth(z, e.getColor()).y;
					this->gBuffer[K].Char.AsciiChar = e.getCharacter();//this->getDepth(z, e.getColor()).x;
				}
		}
	}


	//for points
	for (auto e : this->points)
	{
		vec4d point(e.getPosition().x, e.getPosition().y, e.getPosition().z, 1);
		point = *camera.getView() * point;
		if (point.w != 1)
			point /= point.w;

		float z = point.z;
		if (z < 0)
			continue;
		point = *camera.getProjection() * point;
		if (point.w != 1)
			point /= point.w;

		point.x += h_width;
		point.y = point.y / 2 + q_height;
		
		if (point.x >= width || point.y >= height || point.x < 0 || point.y < 0)
			continue;
		int K = safeIndex(int(point.x), int(point.y));
		if(K> 0)
			if (this->zBuffer[K] > z)
			{
				//filter the depth
				vec2u charCol= this->getDepth(z, e.getColor());
				this->gBuffer[K].Attributes = charCol.y;
				this->gBuffer[K].Char.AsciiChar = charCol.x;//e.getCharacter();
			}
	}
}

bool graphics::Graphics::pointInTriangle(vec2f p, Triangle t)
{
	double denominator = ((t.vertices[1].y - t.vertices[2].y) * (t.vertices[0].x - t.vertices[2].x) + (t.vertices[2].x - t.vertices[1].x) * (t.vertices[0].y - t.vertices[2].y));
	double a = ((t.vertices[1].y - t.vertices[2].y) * (p.x - t.vertices[2].x) + (t.vertices[2].x - t.vertices[1].x) * (p.y - t.vertices[2].y)) / denominator;
	double b = ((t.vertices[2].y - t.vertices[0].y) * (p.x - t.vertices[2].x) + (t.vertices[0].x - t.vertices[2].x) * (p.y - t.vertices[2].y)) / denominator;
	double c = 1 - a - b;

	// Check if all barycentric coordinates
	// are non-negative
	if (a >= 0 && b >= 0 && c >= 0)
		return true;
	return false;
}

int graphics::Graphics::safeIndex(int i, int j, int w, int h)
{
	if (w ==-1 && h == -1)
		w = this->width, h = height;
	if (j * w + i < w * h)
		return j * w + i;
	else
		return -1;
}

void graphics::Graphics::debugCamera()
{
	this->printDebug("POSITION", vec2i(2, 1), Colors::B_WHITE);
	this->printDebug("X: "+std::to_string(this->camera.getPosition().x), vec2i(2, 2), Colors::WHITE);
	this->printDebug("Y: " + std::to_string(this->camera.getPosition().y), vec2i(2, 3), Colors::WHITE);
	this->printDebug("Z: " + std::to_string(this->camera.getPosition().z), vec2i(2, 4), Colors::WHITE);

	this->printDebug("DIRECTION", vec2i(2, 5), Colors::B_WHITE);
	this->printDebug("X: " + std::to_string(this->camera.getDirection().x), vec2i(2, 6), Colors::WHITE);
	this->printDebug("Y: " + std::to_string(this->camera.getDirection().y), vec2i(2, 7), Colors::WHITE);
	this->printDebug("Z: " + std::to_string(this->camera.getDirection().z), vec2i(2, 8), Colors::WHITE);


	this->printDebug("PARAMETERS", vec2i(2, 10), Colors::B_WHITE);
	this->printDebug("NEAR: " + std::to_string(this->camera.getnear()), vec2i(2, 11), Colors::WHITE);
	this->printDebug("FAR: " + std::to_string(this->camera.getfar()), vec2i(2, 12), Colors::WHITE);
	this->printDebug("FOV: " + std::to_string(this->camera.getFOV()), vec2i(2, 13), Colors::WHITE);

}

Graphics::Graphics(int h, int w, int gw, int gh, int dw, int dh)
	: height(h), width(w), gwidth(gw), gheight(gh), dwidth(dw), dheight(dh)
{
	this->initialize();
	//stupid optimization
	this->q_height = height / 4.0;
	this->h_width = width / 2.0;

	
}

Graphics::~Graphics()
{
	delete[] this->gBuffer;
	delete[] this->zBuffer;
	delete[] this->guiBuffer;
	delete[] this->debugBuffer;
}

CHAR_INFO* Graphics::getBuffer()
{
	return this->gBuffer;
}

COORD Graphics::getBufferSize()
{
	return this->gBufferSize;
}

COORD Graphics::getBufferOffset()
{
	return this->gBufferOffset;
}

Camera* graphics::Graphics::getCamera()
{
	return &this->camera;
	
}

void graphics::Graphics::clearBuffer(char c, unsigned short attr)
{
	//clear objects
	this->triangles.clear();
	this->lines.clear();
	this->points.clear();


	bool randomizeChar = c == 0;
	for (int i = 0; i < this->gBufferSize.X; i++)
	{
		for (int j = 0; j < this->gBufferSize.Y; j++)
		{
			if (randomizeChar)
				c = rand() % 93 + 33;
			this->gBuffer[i * this->gBufferSize.Y + j].Char.UnicodeChar = CHAR(c);
			this->gBuffer[i * this->gBufferSize.Y + j].Attributes = attr;
		}
	}

	for (int i = 0; i < this->guiSize.X; i++)
	{
		for (int j = 0; j < this->guiSize.Y; j++)
		{
			if (randomizeChar)
				c = rand() % 93 + 33;
			this->guiBuffer[i * this->guiSize.Y + j].Char.UnicodeChar = CHAR(c);
			this->guiBuffer[i * this->guiSize.Y + j].Attributes = attr;
		}
	}
	for (int i = 0; i < this->debugSize.X; i++)
	{
		for (int j = 0; j < this->debugSize.Y; j++)
		{
			if (randomizeChar)
				c = rand() % 93 + 33;
			this->debugBuffer[i * this->debugSize.Y + j].Char.UnicodeChar = CHAR(c);
			this->debugBuffer[i * this->debugSize.Y + j].Attributes = attr;
		}
	}

	for (int i = 0; i < this->gBufferSize.X; i++)
		for (int j = 0; j < this->gBufferSize.Y; j++)
			this->zBuffer[i * this->gBufferSize.Y + j]= INFINITY;
}

void graphics::Graphics::drawRect(int x, int y, int w, int h, char c, unsigned short attr)
{
	bool randomizeChar = c == 0;
	if (x < 0 || y < 0 || x+w > this->width || y + h> this->height)
		return;
	
	if (w < 0 || h < 0 || x > this->width || y > this->height)
		return;

	for (int i = x; i < w+x; i++)
	{
		if (randomizeChar)
			c = rand() % 93 + 33;
		this->gBuffer[y * this->gBufferSize.X + i].Char.UnicodeChar = CHAR(c);
		this->gBuffer[y * this->gBufferSize.X + i].Attributes = attr;
		if (randomizeChar)
			c = rand() % 93 + 33;
		this->gBuffer[(y+h-1) * this->gBufferSize.X + i].Char.UnicodeChar = CHAR(c);
		this->gBuffer[(y+h-1) * this->gBufferSize.X + i].Attributes = attr;
	}
	for (int i = y; i < h + y; i++)
	{
		if (randomizeChar)
			c = rand() % 93 + 33;
		this->gBuffer[i * this->gBufferSize.X + x].Char.UnicodeChar = CHAR(c);
		this->gBuffer[i * this->gBufferSize.X + x].Attributes = attr;
		if (randomizeChar)
			c = rand() % 93 + 33;
		this->gBuffer[i * this->gBufferSize.X + x + w-1].Char.UnicodeChar = CHAR(c);
		this->gBuffer[i * this->gBufferSize.X + x + w-1].Attributes = attr;
	}
}

void graphics::Graphics::drawCircle(int x, int y, int r, char c, unsigned short attr)
{
	bool randomizeChar = c == 0;
	if (x - r < 0 || x + r > this->height || y - r < 0 || y + r > this->width)
		return;

	for (int j = y-r; j <= y+r; j++)
	{
		for (int i = x-r; i <= x+r; i++)
		{
			//multiply the x component by 1.5 because the character takes 2 * 1 pixels in the console
			float d = pow((i -x)*1.5, 2) + pow((j-y), 2);
			if(d <= r*r && d>= r*r -4)
			{
				if (randomizeChar)
					c = rand() % 93 + 33;
				this->gBuffer[i * this->gBufferSize.X + j].Char.UnicodeChar = CHAR(c);
				this->gBuffer[i * this->gBufferSize.X + j].Attributes = attr;
			}
		}
	}

}

void graphics::Graphics::drawTriangle(vec2i p1, vec2i p2, vec2i p3, char c, unsigned short attr)
{
	this->drawLine(p1.x, p1.y, p2.x, p2.y, c, attr);
	
	this->drawLine(p2.x, p2.y, p3.x, p3.y, c, attr);

	this->drawLine(p3.x, p3.y, p1.x, p1.y, c, attr);
}

void graphics::Graphics::drawLine(int x1, int y1, int x2, int y2, char c, unsigned short attr)
{
	bool randomizeChar = c == 0;
	if (x1 < 0 || x1 > this->width
		|| x2 < 0 || x2 > this->width
		|| y1 < 0 || y1 > this->height
		|| y2 < 0 || y2 > this->height)
		return;
	//vertical line
	if (x2 == x1)
	{
		for (int i = min(y1, y2); i <= max(y1, y2); i++)
		{
			if (randomizeChar)
				c = rand() % 93 + 33;
			gBuffer[i * this->gBufferSize.X + x1].Char.UnicodeChar = c;
			gBuffer[i * this->gBufferSize.X + x1].Attributes = attr;
		}
	}
	//it has a slope
	else
	{
		//slope
		double m = double(y2 - y1) / double(x2 - x1);
		int prevIndex = 0;
		for(int i = min(x1,x2); i < max(x1,x2); i++)
		{
			
			int j = min(int(m * (i - x1) + y1), int(m * (i + 1 - x1) + y1));
			int l = max(int(m * (i - x1) + y1), int(m * (i + 1 - x1) + y1));
			for (int k = j; k <= l; k++)
			{
				int index = k * this->gBufferSize.X + i;

				if (randomizeChar)
					c = rand() % 93 + 33;
				gBuffer[index].Char.UnicodeChar = c;
				gBuffer[index].Attributes = attr;
			}
		}
	}
}

void graphics::Graphics::fillRect(int x, int y, int w, int h, char c, unsigned short attr)
{
	bool randomizeChar = c == 0;
	if (x < 0 || y < 0 || x + w > this->width || y + h> this->height)
		return;

	if (w < 0 || h < 0 || x > this->width || y > this->height)
		return;

	for (int i = x; i < w + x; i++)
	{
		for (int j = y; j < h + y; j++)
		{
			if (randomizeChar)
				c = rand() % 93 + 33;
			this->gBuffer[j * this->gBufferSize.X + i].Char.UnicodeChar = CHAR(c);
			this->gBuffer[j * this->gBufferSize.X + i].Attributes = attr;
		}
	}
}

void graphics::Graphics::fillCircle(int x, int y, int r, char c, unsigned short attr)
{
	bool randomizeChar = c == 0;
	if (x - r < 0 || x + r > this->height || y - r < 0 || y + r > this->width)
		return;

	for (int j = y - r; j <= y + r; j++)
	{
		for (int i = x - r; i <= x + r; i++)
		{
			//multiply the x component by 1.5 because the character takes 2 * 1 pixels in the console
			float d = pow((i - x) * 1.5, 2) + pow((j - y), 2);
			if (d <= r * r)
			{
				if (randomizeChar)
					c = rand() % 93 + 33;
				this->gBuffer[i * this->gBufferSize.X + j].Char.UnicodeChar = CHAR(c);
				this->gBuffer[i * this->gBufferSize.X + j].Attributes = attr;
			}
		}
	}

}

//fill the triangles based on the container triangles
void graphics::Graphics::fillTriangle(vec2i p1, vec2i p2, vec2i p3, char c, unsigned short attr)
{
	bool randomizeChar = c == 0;
	
	//create bounding box
	int minX = max(min(min(p1.x,p2.x),p3.x), 0), minY = max(min(min(p1.y,p2.y),p3.y), 0);
	int maxX = min(max(max(p1.x, p2.x), p3.x), width), maxY = min(max(max(p1.y, p2.y), p3.y), height);
	
	double det1 = p2.x*p1.y - p2.y*p1.x, det2 = p3.x * p2.y - p3.y * p2.x, det3 = p1.x * p3.y - p1.y * p3.x;

	for (double i = minY; i <= maxY; i++)
	{
		for (double j = minX; j <= maxX; j++)
		{
			//check if pixel is inside triangle
			double t1 = j * (p2.y - p1.y) - i * (p2.x - p1.x) + det1;
			double t2 = j * (p3.y - p2.y) - i * (p3.x - p2.x) + det2;
			double t3 = j * (p1.y - p3.y) - i * (p1.x - p3.x) + det3;
			
			if ((t1 >= 0 && t2 >= 0 && t3 >= 0) || (t1 <= 0 && t2 <= 0 && t3 <= 0))
			{
				if (randomizeChar)
					c = rand() % 93 + 33;
				this->gBuffer[int(i) * this->gBufferSize.X + int(j)].Char.UnicodeChar = CHAR(c);
				this->gBuffer[int(i) * this->gBufferSize.X + int(j)].Attributes = attr;
			}
		}
	}
}

void graphics::Graphics::setPixel(int x, int y, char c, unsigned int attr)
{
	if (x > width || x < 0 || y > height || y < 0)
		return;
	this->gBuffer[y * width + x].Char.AsciiChar = c;
	this->gBuffer[y * width + x].Attributes = attr;
}



vec2u graphics::Graphics::getDepth(float z, unsigned short attr)
{
	char c = ' ';
	unsigned short at = attr;

	c = distanceMap.at(abs(z) < 1.f ? 0 : int(min(abs(z)/2,69)));
	return vec2u(c, at);
}

void graphics::Graphics::drawObject(Object& obj, char c, unsigned short attr)
{
	for (int i = 0; i < obj.indices.size()/3; i++)
	{
		int K = obj.indices[i * 3], L = obj.indices[i * 3 + 1], M = obj.indices[i * 3 + 2];

		// calculate model * vertex;
		vec4d pp1, pp2, pp3;
		pp1.x = obj.vertices[K].x, pp1.y = obj.vertices[K].y, pp1.z = obj.vertices[K].z, pp1.w = 1;
		pp2.x = obj.vertices[L].x, pp2.y = obj.vertices[L].y, pp2.z = obj.vertices[L].z, pp2.w = 1;
		pp3.x = obj.vertices[M].x, pp3.y = obj.vertices[M].y, pp3.z = obj.vertices[M].z, pp3.w = 1;

		pp1 = obj.transform.model * pp1;
		pp2 = obj.transform.model * pp2;
		pp3 = obj.transform.model * pp3;

		Triangle t(vec3f(pp1.x, pp1.y, pp1.z), vec3f(pp2.x, pp2.y, pp2.z), vec3f(pp3.x, pp3.y, pp3.z));
		t.setTexture(*obj.texture);
		t.setTexCoords(obj.texcoords[i * 3], obj.texcoords[i * 3 + 1], obj.texcoords[i * 3 + 2]);
		t.Char = c;
		t.attr = obj.colors[i];
		
		this->triangles.push_back(t);	
	}
}

void graphics::Graphics::drawLine(Line l)
{
	this->lines.push_back(l);
}

void graphics::Graphics::drawPoint(Point p)
{
	this->points.push_back(p);
}

void graphics::Graphics::drawContour(int thickness, char c, unsigned short attr)
{
	for (int i = 0; i < width; i++)
	{
		this->gBuffer[safeIndex(i, 0)].Char.AsciiChar = c;
		this->gBuffer[safeIndex(i, 0)].Attributes = attr;

		this->gBuffer[safeIndex(i, height - 1)].Char.AsciiChar = c;
		this->gBuffer[safeIndex(i, height - 1)].Attributes = attr;
	}
	for(int j = 0; j < height; j++)
	{
		this->gBuffer[safeIndex(0, j)].Char.AsciiChar = c;
		this->gBuffer[safeIndex(0, j)].Attributes = attr;

		this->gBuffer[safeIndex(width - 1, j)].Char.AsciiChar = c;
		this->gBuffer[safeIndex(width - 1, j)].Attributes = attr;
	}
	
	//GUI
	for (int i = 0; i < gwidth; i++)
	{
		this->guiBuffer[safeIndex(i, 0, this->gwidth, this->gheight)].Char.AsciiChar = c;
		this->guiBuffer[safeIndex(i, 0, this->gwidth, this->gheight)].Attributes = attr;

		this->guiBuffer[safeIndex(i, gheight - 1, this->gwidth, this->gheight)].Char.AsciiChar = c;
		this->guiBuffer[safeIndex(i, gheight - 1, this->gwidth, this->gheight)].Attributes = attr;
	}

	for (int j = 0; j < gheight; j++)
	{
		this->guiBuffer[safeIndex(0, j, this->gwidth, this->gheight)].Char.AsciiChar = c;
		this->guiBuffer[safeIndex(0, j, this->gwidth, this->gheight)].Attributes = attr;

		this->guiBuffer[safeIndex(gwidth - 1, j, this->gwidth, this->gheight)].Char.AsciiChar = c;
		this->guiBuffer[safeIndex(gwidth - 1, j, this->gwidth, this->gheight)].Attributes = attr;
	}
	
	//DEBUG
	for (int i = 0; i < dwidth; i++)
	{
		this->debugBuffer[safeIndex(i, 0, this->dwidth, this->dheight)].Char.UnicodeChar = c;
		this->debugBuffer[safeIndex(i, 0, this->dwidth, this->dheight)].Attributes = attr;

		this->debugBuffer[safeIndex(i, dheight - 1, this->dwidth, this->dheight)].Char.UnicodeChar = c;
		this->debugBuffer[safeIndex(i, dheight - 1, this->dwidth, this->dheight)].Attributes = attr;
	}

	for (int j = 0; j < dheight; j++)
	{
		this->debugBuffer[safeIndex(0, j, this->dwidth, this->dheight)].Char.UnicodeChar = c;
		this->debugBuffer[safeIndex(0, j, this->dwidth, this->dheight)].Attributes = attr;

		this->debugBuffer[safeIndex(dwidth - 1, j, this->dwidth, this->dheight)].Char.UnicodeChar = c;
		this->debugBuffer[safeIndex(dwidth - 1, j, this->dwidth, this->dheight)].Attributes = attr;
	}
}

void graphics::Graphics::printGui(std::string msg, vec2i pos, unsigned int attr)
{

	for(int i = 0; i < msg.size(); i++)
	{
		this->guiBuffer[(pos.y + (pos.x+i)/ this->guiSize.X)*this->guiSize.X + (pos.x +i)% this->guiSize.X].Char.AsciiChar = msg[i];
		this->guiBuffer[(pos.y + (pos.x + i) / this->guiSize.X) * this->guiSize.X + (pos.x + i) % this->guiSize.X].Attributes = attr;
	}
}
void graphics::Graphics::printDebug(std::string msg, vec2i pos, unsigned int attr)
{

	for (int i = 0; i < msg.size(); i++)
	{
		this->debugBuffer[(pos.y + (pos.x + i) / this->debugSize.X) * this->debugSize.X + (pos.x + i) % this->debugSize.X].Char.AsciiChar = msg[i];
		this->debugBuffer[(pos.y + (pos.x + i) / this->debugSize.X) * this->debugSize.X + (pos.x + i) % this->debugSize.X].Attributes = attr;
	}
}

void graphics::Graphics::updateInput()
{
	//translation component
	//this->camera.setPosition(vec3d(30 * cos(MPI / 2 - 30 * dt), 0, 30*sin(MPI / 2 - 30 * dt)));
	//this->camera.setPosition(vec3d(0, 0, -10));

	//rotate axis/angle
	//this->camera.rotate(vec3d(1, 0, 1), MPI / 2 + 30 * dt);
	this->camera.setFOV(60);
	this->camera.setNear(0.01);
	this->camera.setFar(10);
	camera.update();
	dt += 0.001f;
}

void Graphics::render()
{
	this->computeDepth();
	
}

void graphics::Graphics::display()
{
	this->drawContour(1, ' ', Colors::B_WHITE);
	this->debugCamera();

	if (WriteConsoleOutput(this->hConsole, this->gBuffer, this->gBufferSize, this->gBufferOffset, &this->gBufferRegion) != 0)
	{
		HRESULT err = HRESULT_FROM_WIN32(GetLastError());
	}

	//debug
	if (WriteConsoleOutput(this->hConsole, this->debugBuffer, this->debugSize, this->debugOffset, &this->debugRegion) != 0)
	{
		HRESULT err = HRESULT_FROM_WIN32(GetLastError());
	}
	//gui
	if (WriteConsoleOutput(this->hConsole, this->guiBuffer, this->guiSize, this->guiOffset, &this->guiRegion) != 0)
	{
		HRESULT err = HRESULT_FROM_WIN32(GetLastError());
	}

}
