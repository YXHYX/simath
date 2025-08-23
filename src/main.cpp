#include <iostream>
#include "tools.h"
#include "Graphics.h"

//#include "../examples/PerlinNoise.h"
#include "../examples/Mandelbrot.h"
/*
* TODO:
*	- Z DEPTH buffer done
*	- clipping space done
*   - orthographic projetion
*   - shading
*   - proper dithering
*   - lighting { use characters with black text color to dim colors down }
*/

using namespace graphics;
int main()
{
	graphics::Graphics graph;
	//Cube o;

	//o.loadTexture("./textures/test.png");
	//o.texture->load();


	Mandelbrot mb(graph.getBufferSize().X, graph.getBufferSize().Y);

	float dt = 0;
	while (true)
	{


		Sleep(1);
		graph.updateInput();
		
		graph.clearBuffer();

		dt += 0.01;
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			break;

		mb.updateInput(dt);
		//graph.render();
		for(int i =0; i < graph.getBufferSize().X; i++)
		{
			for (int j = 0; j < graph.getBufferSize().Y; j++)
			{
				graph.setPixel(i, j, 'x', mb.getPixel(i, j));
			}
		}
		
		graph.display();
	}
}
