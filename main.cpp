#include <iostream>
#include "tools.h"
#include "Graphics.h"

/*
* TODO:
*	- Z DEPTH buffer done
*	- clipping space done
*   - orthographic projetion
*   - shading
*   - proper dithering
*   - lighting
*/

using namespace graphics;
int main()
{
	graphics::Graphics graph;
	Object o;
	float distance = 5;
	float x = -2.5, y = -2.5;
	float size = 5;

	o.vertices.clear();

	vec3f a(x, y, distance);                  
	o.vertices.push_back(a);				//0
	a = vec3f(x + size, y , distance);
	o.vertices.push_back(a);				//1
	a = vec3f(x + size, y + size, distance);
	o.vertices.push_back(a);				//2
	a = vec3f(x, y + size, distance);
	o.vertices.push_back(a);				//3
	
	distance = 0;
	a = vec3f(x, y, distance);
	o.vertices.push_back(a);				//4
	a = vec3f(x + size, y, distance);
	o.vertices.push_back(a);				//5
	a = vec3f(x + size, y + size, distance);
	o.vertices.push_back(a);				//6
	a = vec3f(x, y + size, distance);
	o.vertices.push_back(a);				//7
	

	/*
	*	 7			6
	* 3			2
	* 
	*    4			5
	* 0			1
	* 
	*/
	o.indices.clear();
	o.colors.clear();
	o.texcoords.clear();
	//front
	// 0 1 2
	// 2 0 3
	o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(1, 1));
	o.texcoords.push_back(vec2f(1, 1)); o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(0, 1));

	o.indices.push_back(0); o.indices.push_back(1); o.indices.push_back(2);
	o.indices.push_back(2); o.indices.push_back(0); o.indices.push_back(3);
	o.colors.push_back(Colors::B_GREY);
	o.colors.push_back(Colors::B_LIGHT_GREY);
	
	//right
	// 3 2 6 
	// 3 6 7
	o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(1, 1));
	o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(1, 1)); o.texcoords.push_back(vec2f(0, 1));

	o.indices.push_back(3); o.indices.push_back(2); o.indices.push_back(6);
	o.indices.push_back(3); o.indices.push_back(6); o.indices.push_back(7);
	o.colors.push_back(Colors::B_LIGHT_RED);
	o.colors.push_back(Colors::B_RED);
	
	//top
	// 1 2 5
	// 6 2 5
	o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(0, 1));
	o.texcoords.push_back(vec2f(1, 1)); o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(0, 1));

	o.indices.push_back(1); o.indices.push_back(2); o.indices.push_back(5);
	o.indices.push_back(6); o.indices.push_back(2); o.indices.push_back(5);
	o.colors.push_back(Colors::B_BLUE);
	o.colors.push_back(Colors::B_LIGHT_BLUE);

	//left
	// 0 4 1
	// 1 4 5
	o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(0, 1));
	o.texcoords.push_back(vec2f(0, 1)); o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(1, 1));

	o.indices.push_back(0); o.indices.push_back(4); o.indices.push_back(1);
	o.indices.push_back(1); o.indices.push_back(4); o.indices.push_back(5);
	o.colors.push_back(Colors::B_YELLOW);
	o.colors.push_back(Colors::B_GOLD);
	
	//back
	// 4 7 6
	// 4 6 5
	o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(0, 1));
	o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(0, 1)); o.texcoords.push_back(vec2f(1, 1));
	
	o.indices.push_back(4); o.indices.push_back(7); o.indices.push_back(6);
	o.indices.push_back(4); o.indices.push_back(6); o.indices.push_back(5);
	o.colors.push_back(Colors::B_LIME);
	o.colors.push_back(Colors::B_GREEN);
	
	//bottom
	// 0 4 3
	// 7 3 4
	o.texcoords.push_back(vec2f(0, 0)); o.texcoords.push_back(vec2f(1, 0)); o.texcoords.push_back(vec2f(0, 1));
	o.texcoords.push_back(vec2f(1, 1)); o.texcoords.push_back(vec2f(0, 1)); o.texcoords.push_back(vec2f(1, 0));
	
	o.indices.push_back(0); o.indices.push_back(4); o.indices.push_back(3);
	o.indices.push_back(7); o.indices.push_back(3); o.indices.push_back(4);
	o.colors.push_back(Colors::B_LIGHT_PURPLE);
	o.colors.push_back(Colors::B_PURPLE);
	
	o.texture = std::make_shared<Texture>(new Texture("./textures/real.png"));
	o.texture->load();
	const int count = 1;
	float dt = 0;
	while (true)
	{

		Sleep(1);
		graph.clearBuffer();

		dt += 0.001;
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			break;
		o.model = identity4x4<double>();

		float theta = dt;
		float r = 0;
		for (int i = 0; i < count; i++)
		{
			o.model = identity4x4<double>();
			
			transformation::translate(o.model, vec3d(r * cos(10*dt), 0, r*sin(10*dt)));
			//transformation::rotate(o.model, vec3d(sin(dt), cos(dt), 1), 10*dt + 4 * MPI / count * i);
			
			//r = cos(5/3 * theta)
			graph.drawObject(o, ' ', Colors::LIGHT_BLUE);
		}
		graph.render();
		graph.updateInput();
		int n = o.texture.use_count();

	}
}
