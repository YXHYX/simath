#include "example_demos.h"

#include <iostream>
#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include "fizix/PhysicsWorld.h"
#include "fizix_control/PIDController.h"
#include <chrono>


using namespace graphics;
void controller_demo()
{
	graphics::Graphics graph;
	ResourceManager resourceManager;
	PhysicsWorld world;

	std::string msg = "";
	
	//create objects
	resourceManager.addObject<Cube>("cube1", vec3d(0, 0, 0));
	resourceManager.addObject<Cube>("cube2", vec3d(0, -1, 3));
	resourceManager.addObject<Cube>("cube3", vec3d(2, -3, 0));
	resourceManager.addObject<Cube>("cube4", vec3d(0, -5, 1));

	float elapsedTime = 0.00001;
	float totalTime = 0.0001;
	
	//PID controllers for desired position (0,0,0)
	fizix::controllers::PIDController pidControl1(0, 0.1, 5, 0.0001);
	fizix::controllers::PIDController pidControl2(0, 0, 0, 0.);
	fizix::controllers::PIDController pidControl3(0, 0.1, 5, 0.0001);
	
	//create material points for each 3d object
	world.addMaterialPoint(resourceManager.getObject("cube1"), 1.f);
	world.addMaterialPoint(resourceManager.getObject("cube2"), 1.f);
	world.addMaterialPoint(resourceManager.getObject("cube3"), 1.0f);
	world.addMaterialPoint(resourceManager.getObject("cube4"), 1.f);
	
	//lock the first point on the XZ plane
	(*world.getMaterialPoints())[0]->y_unlock = false;


	//add position constraints between objects
	float distance = norm(resourceManager.getObject("cube1")->transform.getPosition() - resourceManager.getObject("cube2")->transform.getPosition());

	world.addPositionConstraint((*world.getMaterialPoints())[0], (*world.getMaterialPoints())[1], distance);
	distance = norm(resourceManager.getObject("cube2")->transform.getPosition() - resourceManager.getObject("cube3")->transform.getPosition());
	world.addPositionConstraint((*world.getMaterialPoints())[1], (*world.getMaterialPoints())[2], distance);
	distance = norm(resourceManager.getObject("cube3")->transform.getPosition() - resourceManager.getObject("cube4")->transform.getPosition());
	world.addPositionConstraint((*world.getMaterialPoints())[2], (*world.getMaterialPoints())[3], distance);
	
	//set to true to allow for control or false for no control (chaotic)
	bool control = true;
	int ITERATION_NUM = 10;
	while (true)
	{
		

		//fps lock
		//Sleep(1);
		//timer for deltaTime
		const auto start{ std::chrono::steady_clock::now() };

		graph.updateInput();
		for(int j = 0; j < ITERATION_NUM; j++){
			//reset the accelerations on each iteration
			world.resetForces();
			if(control){
		
				pidControl1.step(elapsedTime/ ITERATION_NUM);
				pidControl2.step(elapsedTime/ ITERATION_NUM);
				pidControl3.step(elapsedTime/ ITERATION_NUM);

				//apply corrections
				float mass = (*world.getMaterialPoints())[0]->getMass();
				(*world.getMaterialPoints())[0]->applyForce(vec3d(pidControl1.getOutput(), pidControl2.getOutput(), pidControl3.getOutput())* mass);
			}

			world.update(elapsedTime/ ITERATION_NUM);

			if (control) {
				//get current position to compute error
				pidControl1.setCurrent((*world.getMaterialPoints())[0]->getAcceleration().x);
				pidControl2.setCurrent((*world.getMaterialPoints())[0]->getAcceleration().y);
				pidControl3.setCurrent((*world.getMaterialPoints())[0]->getAcceleration().z);
			}
		}
		graph.clearBuffer();

		//DEBUG STUFF
		vec3d objectPos = (*world.getMaterialPoints())[0]->getPosition();
		msg = std::to_string(objectPos.x) + " " + std::to_string(objectPos.y)
			+ " " + std::to_string(objectPos.z) + "  TIME : " + std::to_string(elapsedTime/10);
		graph.printGui(msg, vec2i(10, 3), Colors::B_WHITE);
		
		//draw trajectory of the locked point

		std::vector<vec3d>* traj = (*world.getMaterialPoints())[0]->getTrajectory();
		for (int i = 0; i < traj->size(); i++)
			graph.drawPoint(Point((*traj)[i], Colors::B_LIGHT_CYAN, 'X'));

		//draw objects
		//graph.drawObject(resourceManager.getObject("cube1"), ' ', 0);
		//graph.drawObject(resourceManager.getObject("cube2"), ' ', 0);
		//graph.drawObject(resourceManager.getObject("cube3"), ' ', 0);
		graph.drawObject(resourceManager.getObject("cube4"), ' ', 0);

		//visualize constraints
		Line constraintLine;

		constraintLine.setColor(Colors::B_LIGHT_PURPLE);
		constraintLine.setPosition((*world.getMaterialPoints())[1]->getPosition(), (*world.getMaterialPoints())[0]->getPosition());
		graph.drawLine(constraintLine);

		constraintLine.setColor(Colors::B_GOLD);
		constraintLine.setPosition((*world.getMaterialPoints())[1]->getPosition(), (*world.getMaterialPoints())[2]->getPosition());
		graph.drawLine(constraintLine);

		constraintLine.setColor(Colors::B_LIGHT_BLUE);
		constraintLine.setPosition((*world.getMaterialPoints())[2]->getPosition(), (*world.getMaterialPoints())[3]->getPosition());
		graph.drawLine(constraintLine);

		constraintLine.setColor(Colors::B_LIGHT_RED);
		constraintLine.setPosition(vec3d(0,0,0), (*world.getMaterialPoints())[0]->getPosition());
		graph.drawLine(constraintLine);

		/*
		//draw grid optional
		int gridSize = 1;
		constraintLine.setColor(Colors::B_LIGHT_GREY);
		//horizontal X
			for (int j = -5; j <= 5; j++)
			{
				constraintLine.setPosition(vec3d(j*gridSize, 0, 5*gridSize), vec3d(j*gridSize,0,-5*gridSize));
				graph.drawLine(constraintLine);
			}
		//horizontal other way Z
			for (int j = -5; j <= 5; j++)
			{
				constraintLine.setPosition(vec3d(5 * gridSize, 0, j * gridSize), vec3d(-5 * gridSize, 0, j * gridSize));
				graph.drawLine(constraintLine);
			}
		*/
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
