#include "example_demos.h"

#include <iostream>
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include "fizix/PhysicsWorld.h"
#include "fizix_control/PIDController.h"
#include <chrono>


using namespace graphics;
void benchmark_demo()
{
	graphics::Graphics graph;
	ResourceManager resourceManager;
	PhysicsWorld world;

	std::string msg = "";

	const int maxObjects = 100;
	//create objects
	for(int i = 0; i < maxObjects; i++)
		resourceManager.addObject<Cube>("o" + std::to_string(i), vec3d(i, i, i));

	float elapsedTime = 0.00001;
	float totalTime = 0.0001;

	while (true)
	{


		//fps lock
		//Sleep(1);
		//timer for deltaTime
		const auto start{ std::chrono::steady_clock::now() };

		graph.updateInput();
		graph.clearBuffer();

		//DEBUG STUFF
		msg = "  TIME : " + std::to_string(elapsedTime / 10);
		graph.printGui(msg, vec2i(10, 3), Colors::B_WHITE);

		//set positions

		//draw objects
		for (float i = 0; i < maxObjects; i++){
			resourceManager.getObject("o" + std::to_string(int(i)))->transform.setRotation(vec3f(totalTime)+vec3f(i/maxObjects, -i / maxObjects, i / maxObjects));
			resourceManager.getObject("o" + std::to_string(int(i)))->transform.setPosition(
				vec3f(30,30,30) + vec3f(10 * cos(totalTime+6*i / maxObjects), 10 * sin(totalTime - 6 * i / maxObjects), -10 * cos(totalTime + 6 * i / maxObjects)));
		}

		//draw objects
		for (int i = 0; i < maxObjects; i++)
			graph.drawObject(resourceManager.getObject("o" + std::to_string(i)), ' ', 0);

		//render everything
		graph.render();
		// then display
		graph.display();

		//get DT and elapsed time
		const auto finish{ std::chrono::steady_clock::now() };
		const std::chrono::duration<double> elapsed_seconds{ finish - start };
		elapsedTime = elapsed_seconds.count();
		totalTime += elapsedTime;

		//quit
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;
	}
}
