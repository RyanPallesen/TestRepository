#include "PhysicsProjectProjectilesApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <math.h>

#include <Application.h>

using namespace glm;
using namespace aie;
PhysicsProjectProjectilesApp::PhysicsProjectProjectilesApp() {

}

PhysicsProjectProjectilesApp::~PhysicsProjectProjectilesApp() {

}
// Convert screen space coords to world space coords
glm::vec2 PhysicsProjectProjectilesApp::ToWorldSpace(int screenX, int screenY)
{
	// Get the window dimensions
	glm::vec2 windowDimensions = glm::vec2(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f);
	static float aspectRatio = windowDimensions.x / windowDimensions.y;

	// Convert screen position to numbers in range -1 to 1
	glm::vec2 normalisedScreenPos = (glm::vec2((float)screenX, (float)screenY) / windowDimensions) - glm::vec2(1.0f, 1.0f);

	// Convert screen position to world space
	glm::vec2 worldPos = glm::vec2(normalisedScreenPos.x * 100.0f, normalisedScreenPos.y * 100.0f / aspectRatio);

	return worldPos;
}

void PhysicsProjectProjectilesApp::setupContinuousDemo(glm::vec2 startPos, glm::vec2 velocity, glm::vec2 gravity, float mass)
{
	int maxT = 16;
	float t = 0;
	float tStep = 0.1f;
	float radius = 0.5f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	while (t <= maxT)
	{
		radius -= (radius / (maxT / tStep));
		// calculate the x, y position of the projectile at time t

		float x = startPos.x + (velocity.x * t) + ((gravity.x) * t * t * 0.5f);;
		float y = startPos.y + (velocity.y * t) + ((gravity.y) * t * t * 0.5f);

		aie::Gizmos::add2DCircle(vec2(x, y), radius, segments, colour);
		t += tStep;
	}
}
 Box* paddle = new Box(vec2(0, 0), vec2(0, 0), 15, vec2(5, 2), vec4(0.25, 0.25, 1, 1), 0);
bool PhysicsProjectProjectilesApp::startup() {
	srand(time(NULL));

	Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new Renderer2D();
	m_physicsScene = new PhysicsScene(0.01f, vec2(0, -25));

	m_physicsScene->addActor(paddle);
	paddle->setKinematic(true);

	for (int i = -25; i < 25; i++)
		m_physicsScene->addActor(new Plane(vec2(i*5, 15),-35 - abs(i*5)));
	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new Font("../bin/font/consolas.ttf", 32);

	return true;
}

void PhysicsProjectProjectilesApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsProjectProjectilesApp::update(float deltaTime) {

	// input example
	Input* input = Input::getInstance();



	Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	static bool isCreating;
	static vec2 createdPosition = vec2(input->getMouseX(), input->getMouseY());

	if (input->wasMouseButtonPressed(0) && isCreating == false)
	{
		isCreating = true;
		createdPosition = ToWorldSpace(input->getMouseX(), input->getMouseY());
	}

	if (isCreating)
	{
		Gizmos::add2DCircle(createdPosition, 1, 32, vec4(1, 1, 1, 1));

		setupContinuousDemo(createdPosition, createdPosition - ToWorldSpace(input->getMouseX(), input->getMouseY()), m_physicsScene->getGravity(), 1);

		if (!input->isMouseButtonDown(0))
		{
			isCreating = false;
			m_physicsScene->addActor(new Sphere(createdPosition, createdPosition - ToWorldSpace(input->getMouseX(), input->getMouseY()), 1, 1, vec4(1, 1, 1, 1),0.9f,0.0f));

		}
	}

	if (input->isMouseButtonDown(1))
	{
		for each (PhysicsObject * var in m_physicsScene->getActors())
		{
			if (dynamic_cast<Rigidbody*>(var) != nullptr)
			{
				if (dynamic_cast<Rigidbody*>(var)->ContainsPoint(ToWorldSpace(input->getMouseX(), input->getMouseY())))
				{
					m_physicsScene->removeActor(var);
				}
			}
		}
	}



	static const vec4 colours[] =
	{

	vec4(1,0,0,1),
	vec4(0,1,0,1),
	vec4(0,0,1,1),
	vec4(0.8f,0,0.5f,1),
	vec4(0,1,1,1),

	};

	static const int rows = 5;
	static const int columns = 10;
	static const int hSpace = 1;
	static const int vSpace = 1;

	static const vec2 scrExtents(100, 50);
	static const vec2 boxExtents(7, 3);
	static const vec2 startPos(-(columns >> 1)* ((boxExtents.x * 2) + vSpace) + boxExtents.x + (vSpace / 2.0f), scrExtents.y - ((boxExtents.y * 2) + hSpace));


	//draw the grid of blocks
	vec2 pos;
	for (int y = 0; y < rows; y++)
	{

		pos = vec2(startPos.x, startPos.y - (y * ((boxExtents.y * 2) + hSpace)));
		for (int x = 0; x < columns; x++)
		{
			//Gizmos::add2DAABBFilled(pos, boxExtents, colours[y]);

			pos.x += (boxExtents.x * 2) + vSpace;
		}
	}

	//Draw ball



	static vec2 paddlePosition = vec2(0, 0);

	if (input->isKeyDown(INPUT_KEY_A))
	{
		paddlePosition.x--;
	}

	if (input->isKeyDown(INPUT_KEY_D))
	{
		paddlePosition.x++;
	}

	if (input->isKeyDown(INPUT_KEY_S))
	{
		paddlePosition.y--;
	}

	if (input->isKeyDown(INPUT_KEY_W))
	{
		paddlePosition.y++;
	}

	if (input->isKeyDown(INPUT_KEY_Q))
	{
		paddle->m_rotation++;
	}

	if (input->isKeyDown(INPUT_KEY_E))
	{
		paddle->m_rotation--;
	}

	//Draw paddle
	paddle->setPosition(paddlePosition);

	// exit the application
	if (input->isKeyDown(INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsProjectProjectilesApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	Gizmos::draw2D(ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}