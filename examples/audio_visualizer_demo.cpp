#include "example_demos.h"

#include <iostream>
#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include <chrono>

/*
* TODO:
*	- Z DEPTH buffer			done
*	- clipping space			done
*   - orthographic projetion
*   - shading					SOMEWHAT DONE
*   - proper dithering			ordered bayer 4x4 and floyd steinberg
*   - lighting					somewhat made?
*	- physics
*		- better solvers						RK4 and euler for now
*		- consistency checks (energy)			later
*		- constraints
*			- angular							later
*			- linear							done
*		- springs								done
*		- forcefields							done
*	- graphics
*		- line rendering		done but gotta fix and optimize
*		- z fighting and viewport clipping
*		- more dithering!
*		- LIGHTING!!!!
*	- resource manager
*		- texture manager
*		- models( if need for any)
*		- sounds manager
*/
vec3d sinc(vec3d p)
{
	vec3d vec;
	double rho = sqrt(p.x * p.x + p.z * p.z);
	vec = p;
	if (rho != 0)
		vec.y = 10 * sin(rho) / rho;
	return vec;
}
using namespace graphics;
void audio_visualizer_demo(std::string audioPath)
{
	graphics::Graphics graph;
	ResourceManager resourceManager;


	std::string msg = "";

	float elapsedTime = 0;
	float totalTime = 0.0001;


	while (true)
	{

		Sleep(1);
		const auto start{ std::chrono::steady_clock::now() };

		graph.updateInput();

		graph.clearBuffer();

		//resourceManager.getObject("sqr")->transform.setRotation(vec3d(0, totalTime * 3, 0));

		for (int i = 0; i <= 50; i++)
			for (int j = 0; j <= 50; j++)
				graph.drawPoint(Point(sinc(vec3d(i - 25, 0, j - 25) / (10*sin(totalTime)+20)), (int(norm(vec3d(i - 25, 0, j - 25))) % 8 + 8) << 4, ' '));


		graph.render();

		graph.display();

		const auto finish{ std::chrono::steady_clock::now() };
		const std::chrono::duration<double> elapsed_seconds{ finish - start };
		elapsedTime = elapsed_seconds.count();
		totalTime += elapsedTime;

		//quit
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;
	}
	//return 0;
}
