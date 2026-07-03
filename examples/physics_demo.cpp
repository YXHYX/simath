#include <iostream>
#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include "fizix/PhysicsWorld.h"

#include "Physics.h"
#include <chrono>

#include "example_demos.h"

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

using namespace graphics;
void physics_demo()
{
	graphics::Graphics graph;
	ResourceManager resourceManager;
	
	std::string msg = "";

	resourceManager.addObject<Cube>("cube1",vec3d(0,1,0));

	resourceManager.loadTexture("teto", "./test1.png", 0);
	
	resourceManager.bindTexture("teto", "cube1");

	resourceManager.loadSound("teto", "./sounds/helloworld.wav");
	float elapsedTime = 0;
	float totalTime = 0.0001;

	resourceManager.getSound("teto")->setPitch(0.8);
	resourceManager.getSound("teto")->playSound();

	while (true)
	{
		
		Sleep(1);
		const auto start{ std::chrono::steady_clock::now() };

		graph.updateInput();

		resourceManager.getSound("teto")->setSourcePosition(vec3d(20 * cos(10*totalTime), 0, 20 * sin(10 * totalTime)));
		graph.clearBuffer();

		msg = std::to_string(graph.getCamera()->getPosition().x) + " " + std::to_string(graph.getCamera()->getPosition().y)
			+ " " + std::to_string(graph.getCamera()->getPosition().z) + "  TIME : " + std::to_string(elapsedTime);
		graph.printGui(msg, vec2i(10, 3), Colors::B_WHITE);

		graph.drawObject(resourceManager.getObject("cube1"), ' ', 0);


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
}
