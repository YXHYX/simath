#include <iostream>
#include "tools.h"
#include "Graphics.h"
#include "example_demos.h"
#include "../examples/Mandelbrot.h"

using namespace graphics;

int mandelbrot_demo()
{
	graphics::Graphics graph;

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
		for (int i = 0; i < graph.getBufferSize().X; i++)
		{
			for (int j = 0; j < graph.getBufferSize().Y; j++)
			{
				graph.setPixel(i, j, 'x', mb.getPixel(i, j));
			}
		}

		graph.display();
	}

	return 0;
}
