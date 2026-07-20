#include "example_demos.h"

#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include <chrono>

using namespace graphics;

int model_demo()
{
	graphics::Graphics graph;
	ResourceManager resourceManager;

	float elapsedTime = 0;
	float totalTime = 0.0001;

	std::shared_ptr<Model> m = std::make_shared<Model>("./res/model/sphere/sphere.obj");
	while (true)
	{

		Sleep(1);
		const auto start{ std::chrono::steady_clock::now() };

		graph.updateInput();

		graph.clearBuffer();

		graph.drawObject(m,' ', 0);

		graph.render();

		graph.display();

		const auto finish{ std::chrono::steady_clock::now() };
		const std::chrono::duration<double> elapsed_seconds{ finish - start };
		elapsedTime = elapsed_seconds.count();
		totalTime += elapsedTime;

		//quit
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			break;
	}
	return 0;
}
