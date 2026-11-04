#include "example_demos.h"

#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include "../src/fizix/PhysicsWorld.h"
#include <chrono>

using namespace graphics;

int rocketphysics_demo()
{
	graphics::Graphics graph;
	ResourceManager resourceManager;
	PhysicsWorld world;

	float elapsedTime = 0;
	float totalTime = 0.0001;
	srand(time(NULL));
	//rocket model
	std::shared_ptr<Model> m = std::make_shared<Model>("./res/model/rocket.obj");
	std::shared_ptr<Square> plane = std::make_shared<Square>(vec3d(0, -10, 0), vec3d(0, 0, 0), vec3d(20, 20, 1));
	
	m->transform.setRotation(vec3d(2*MPI, 0, 0));
	plane->transform.setRotation(vec3d(HPI, 0, 0));

	//model the rocket as a 2 point constraint with 1 being able to move 

	world.addMaterialPoint(m, 3.f);
	world.addMaterialPoint(nullptr, 2.f);

	world.addPositionConstraint(world.getMaterialPoints()->at(0), world.getMaterialPoints()->at(1), 5);

	world.getMaterialPoints()->at(0)->setPosition(vec3d(0, 0, 0));
	world.getMaterialPoints()->at(1)->setPosition(vec3d(0, 5, 0));

	float thurstForce = 20.f;
	float thrustAngle = 0.f;

	while (true)
	{

		Sleep(1);
		const auto start{ std::chrono::steady_clock::now() };

		world.resetForces();
		// handle input
		graph.updateInput();

		//more thrust
		if (GetAsyncKeyState(VK_UP) & 0x8000)
			thurstForce += 10*elapsedTime;
		//less thrust
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			thurstForce -= 10 * elapsedTime;
	
		if (thurstForce > 100.f)
			thurstForce = 100.f;
		if (thurstForce < 0.f)
			thurstForce = 0.f;

		//thrust angle
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			thrustAngle -= 10*elapsedTime;

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			thrustAngle += 10 * elapsedTime;
		

		if (thrustAngle > 90.f)
			thrustAngle = 90.f;
		if (thrustAngle < -90.f)
			thrustAngle = -90.f;
		
		
		vec3d orientation = world.getMaterialPoints()->at(1)->getPosition() - world.getMaterialPoints()->at(0)->getPosition();

		m->transform.setRotation(transformation::get_euler_from_up(orientation)+vec3d(HPI,0,0));

		mat4x4d rotmat = identity4x4<double>();
		transformation::rotate(rotmat,vec3d(degToRad(thrustAngle), 0, 0));
		vec4d modifiedOr(orientation.x, orientation.y, orientation.z, 1);
		modifiedOr = rotmat * modifiedOr;
		orientation.x = modifiedOr.x;
		orientation.y = modifiedOr.y;
		orientation.z = modifiedOr.z;
		orientation = normalize(orientation)*thurstForce;
		orientation += vec3d(0.1*sin(totalTime), 0, 0); //wind
		world.getMaterialPoints()->at(0)->applyForce(orientation);
		world.update(elapsedTime);
		



		graph.clearBuffer();


		//line
		Line rocketLine(world.getMaterialPoints()->at(0)->getPosition(), world.getMaterialPoints()->at(1)->getPosition(), Colors::B_CYAN, 'X');
		
		graph.drawObject(m, ' ', 0);
		graph.drawLine(rocketLine);
		graph.drawObject(plane, 'X', Colors::B_GREEN);
		graph.render();

		//misc graphs
		graph.printDebug("ACCELERATION: " + std::to_string(norm(world.getMaterialPoints()->at(0)->getAcceleration())), vec2i(2, 15), Colors::YELLOW);

		graph.printDebug("VELOCITY: " + std::to_string(norm(world.getMaterialPoints()->at(0)->getVelocity())), vec2i(2, 17), Colors::GREEN);

		graph.printDebug("POSITION: ", vec2i(2, 19), Colors::WHITE);
		graph.printDebug("X: " + std::to_string(world.getMaterialPoints()->at(0)->getVelocity().x), vec2i(3, 20), Colors::WHITE);
		graph.printDebug("Y: " + std::to_string(world.getMaterialPoints()->at(0)->getVelocity().y), vec2i(3, 21), Colors::WHITE);
		graph.printDebug("Z: " + std::to_string(world.getMaterialPoints()->at(0)->getVelocity().z), vec2i(3, 22), Colors::WHITE);

		graph.printDebug("THRUST: " + std::to_string(thurstForce), vec2i(2, 25), Colors::LIGHT_CYAN);
		graph.printDebug("ANGLE: " + std::to_string(thrustAngle), vec2i(2, 26), Colors::LIGHT_CYAN);


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
