#include <iostream>
#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"

#include "Fluid.h"
#include <chrono>

#include "example_demos.h"

const char* asciiShader = "$@B%8O0Xzcv'.  ";

using namespace graphics;
int fluid3_demo()
{
	graphics::Graphics graph;
	ResourceManager resourceManager;
	std::string msg = "";

	float elapsedTime = 0;
	float totalTime = 0.0001;


	const size_t W = 16, H = 16, D = 16;
	float dt = 0.1f;
	float diff = 0.00001f;
	float visc = 0.00001f;


	fluidSim::Fluid3 fluid(W, H, D, dt, diff, visc);

	while (true)
	{

		Sleep(1);
		const auto start{ std::chrono::steady_clock::now() };

		graph.updateInput();

		graph.clearBuffer();



		// Continuously inject fluid density + upward force at the bottom-center
		fluid.addDensity(W / 2, H - 2, D / 2, 200.0f);
		fluid.addVelocity(W / 2, H - 2, D / 2, rand()%10-5.f, rand() % 10 - 5.f, rand() % 10 - 5.f);

		// Step solver forward
		fluid.step();

		// Print center cross-section
		float maxD = 0, minD = INFINITY;
		Point point;
		for (size_t i{};i<W;i++)
			for (size_t j{};j < H;j++)
				for (size_t k{};k < D;k++)
				{
					float densit = fluid.getDensity(i, j, k);
					
					if (maxD < densit)
						maxD = densit;
					if (minD > densit)
						minD = densit;

					unsigned short at = densit > maxD/2.f ? rand()%15 + 1 : Colors::BLACK;
					point.setPosition(vec3d(i, j, k)/10.f);
					point.setCharacter(asciiShader[int(densit*14/(maxD+1.f))]);
					point.setColor(at);
					graph.drawPoint(point);
				}
		msg = "MAX: " + std::to_string(maxD) + "    |    MIN: " + std::to_string(minD);
		graph.printGui(msg, vec2i(10, 3), Colors::B_WHITE);

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

	return 0;
}
