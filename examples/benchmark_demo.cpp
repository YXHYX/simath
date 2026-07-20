#include "example_demos.h"

#include <iostream>
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include "fizix/PhysicsWorld.h"
#include "fizix_control/PIDController.h"
#include <chrono>


using namespace graphics;
int benchmark_demo()
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


		//set positions
		float factor = int(4 * cos(totalTime) + 5);
		float scale = 10;
		//translation
		for (float i = 0; i < factor * maxObjects; i++) {
			resourceManager.getObject("o" + std::to_string(int(i) % maxObjects))->transform.setRotation(vec3f(i / maxObjects + totalTime, -i / maxObjects + totalTime / 4, i / maxObjects + totalTime / 2));
			resourceManager.getObject("o" + std::to_string(int(i) % maxObjects))->transform.setPosition(
				vec3f(30, 30, 30) + vec3f(scale * sin(totalTime) * cos(cos(totalTime) + DPI * i / (factor * maxObjects)), scale * sin(totalTime) * sin(cos(totalTime) - DPI * i / (factor / 2 * maxObjects)), -scale * sin(totalTime) * cos(sin(totalTime) + DPI * i / maxObjects)));

			graph.drawObject(resourceManager.getObject("o" + std::to_string(int(i) % maxObjects)), ' ', 0);
		}

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

	return 0;
}
