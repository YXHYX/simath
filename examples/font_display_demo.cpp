#include "example_demos.h"

#include <iostream>
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include <chrono>


using namespace graphics;
int font_display_demo()
{
	graphics::Graphics graph;
	ResourceManager resourceManager;

	std::string msg = "";

	const int maxObjects = 100;
	//create objects
	for (int i = 0; i < maxObjects; i++)
		resourceManager.addObject<Cube>("o" + std::to_string(i), vec3d(i, i, i));

	float elapsedTime = 0.00001;
	float totalTime = 0.0001;
	float factor = 2;
	std::vector<std::shared_ptr<Font>> fonts;
	
	std::string path = "res/fonts/";
	for (const auto& entry : std::filesystem::directory_iterator(path))
		fonts.push_back(std::make_shared<Font>(entry.path().string()));


	while (true)
	{
		//timer for deltaTime
		const auto start{ std::chrono::steady_clock::now() };

		graph.updateInput();
		graph.clearBuffer();

		//DEBUG STUFF
		msg = "  TIME : " + std::to_string(elapsedTime / 10);
		int num = 6;
		graph.printGui(msg, vec2i(10, 3), Colors::B_WHITE);
		
		graph.drawText("ABCDEFGHI " + std::to_string(int(totalTime)), vec2i(20, 5), fonts.at(int(totalTime) % fonts.size()), (int(12)) % 15);

		graph.drawText("JKLMNOPQR " + std::to_string(int(totalTime + 1)), vec2i(20, 20), fonts.at(int(totalTime + 1) % fonts.size()), (int(12)) % 15);

		graph.drawText("STUVWXYZ " + std::to_string(int(totalTime + 2)), vec2i(20, 35), fonts.at(int(totalTime + 2) % fonts.size()), (int(12)) % 15);

		graph.drawText("ABCDEFGHI " + std::to_string(int(totalTime + 3)), vec2i(20, 50), fonts.at(int(totalTime + 3) % fonts.size()), (int(12)) % 15);
		graph.drawText("JKLMNOPQR " + std::to_string(int(totalTime + 4)), vec2i(20, 65), fonts.at(int(totalTime + 4) % fonts.size()), (int(12)) % 15);
		graph.drawText("STUVWXYZ " + std::to_string(int(totalTime + 5)), vec2i(20, 80), fonts.at(int(totalTime + 5) % fonts.size()), (int(12)) % 15);
		
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
