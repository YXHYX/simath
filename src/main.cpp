#include <iostream>
#include "tools.h"
#include "Graphics.h"


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
*	- graphics
*		- line rendering		its time to go beast mode
*/

using namespace graphics;
int mainly()
{
	graphics::Graphics graph;
	Cube o;
	Square squarty;
	o.loadTexture("./textures/test.png");
	squarty.loadTexture("./textures/teto.jpg");
	
	std::string msg = "";
	float dt = 0;
	while (true)
	{

		Sleep(1);
		graph.updateInput();
		
		graph.clearBuffer();

		dt += 0.001; //we sleep by 1 millisec
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			break;
		//o.transform.setScale(vec3f(3, 3, 3));
		o.transform.setPosition(vec3f(3*sin(dt), 0, 3 * cos(dt)));

		//o.transform.setPosition(vec3f(-1, 0, -1));
		graph.drawObject(o, ' ', Colors::LIGHT_BLUE);

		squarty.transform.setRotation(vec3f(0, MPI, 0));
		squarty.transform.setScale(vec3f(100, 10, 100));
		squarty.transform.setPosition(vec3f(0, 10, 0));
		
		Line l1(o.transform.getPosition(), vec3d(0,0,0), Colors::LIGHT_BLUE,'X');
		msg = std::to_string(graph.getCamera()->getPosition().x) + " " + std::to_string(graph.getCamera()->getPosition().y)
			+ " " + std::to_string(graph.getCamera()->getPosition().z);
		
		//graph.drawObject(squarty, ' ', Colors::LIGHT_BLUE);
		graph.drawLine(l1);

		graph.render();
		
		graph.printGui(msg, vec2i(10,3), Colors::B_WHITE);
		
		graph.display();
	}
	return 0;
}
