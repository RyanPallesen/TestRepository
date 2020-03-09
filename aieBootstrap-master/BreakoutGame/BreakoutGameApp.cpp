#include "BreakoutGameApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <math.h>

#include <Application.h>

using namespace glm;
using namespace aie;

BreakoutGameApp::BreakoutGameApp()
{
}
BreakoutGameApp::~BreakoutGameApp()
{
}

void BreakoutGameApp::BallHitBrick(PhysicsObject* self, PhysicsObject* brick)
{
	gameScore += 1;
	m_physicsScene->removeActor(brick);
}


Box* paddle = new Box(vec2(0, 0), vec2(0, 0), 256, vec2(5, 2), vec4(0.25, 0.25, 1, 1), 0, 1.1f, 0.0f, 0.0f);
std::vector<Sphere*> balls;

bool BreakoutGameApp::startup() {
	srand(time(NULL));

	Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new Renderer2D();
	m_physicsScene = new PhysicsScene(0.01f, vec2(0, 0.01f));

	m_physicsScene->addActor(paddle);


	paddle->setKinematic(true);
	paddle->tags.push_back("Paddle");


	m_physicsScene->addActor(new Plane(vec2(0, 1), 64));
	m_physicsScene->addActor(new Plane(vec2(1, 0), 100));
	m_physicsScene->addActor(new Plane(vec2(1, 0), -100));


	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new Font("../bin/font/consolas.ttf", 32);

	return true;
}

void BreakoutGameApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void BreakoutGameApp::update(float deltaTime) {

	// input example
	Input* input = Input::getInstance();

	static int timesCompleted;
	static vec2 paddlePosition = vec2(0, -40);

	if (m_physicsScene->getActorCount() <= 4 + timesCompleted)
	{

		Sphere* newBall = new Sphere(vec2(0, -30), vec2(25, 25), 1, 1, vec4(0.2f, 0, 0.2f, 1), 1.0f, 0.0f, 0.0f);
		balls.push_back(newBall);
		m_physicsScene->addActor(newBall);


		for (Sphere* ball : balls)
		{
			ball->m_position = paddlePosition + vec2(0, 10);
			if (ball->getVelocity().y < 1)
			{
				ball->m_velocity.y *= -1;
			}
		}

		int rows = 2 + timesCompleted;
		int columns = 4 + timesCompleted + timesCompleted;

		vec2 boxExtents(min(50 / ((rows - 1) * 2), 12), min(50 / ((rows - 1) * 2), 6));

		//going from -50 to +50 X
		//going from 0 to + 50 Y

		vec2 startPos(-100, 0);

		gameLives += 3;
		gameLives *= 1.1f;

		//draw the grid of blocks
		vec2 pos = startPos;
		vec4 colour = vec4(0, 0, 0, 1);

		m_physicsScene->ReserveSpace(columns * rows);

		for (int y = 0; y < rows; y++)
		{
			colour += vec4((float)rand() / (RAND_MAX * rows), (float)rand() / (RAND_MAX * rows), (float)rand() / (RAND_MAX * rows), 0);

			for (int x = 0; x < columns; x++)
			{
				Box* box = new Box(pos, vec2(0, 0), 256, boxExtents, colour, rand() % 360, 1.0f, 2.0f, 2.0f);
				box->tags.push_back("Breakable");
				box->setKinematic(true);
				m_physicsScene->addActor(box);

				pos.x += (200 / (columns - 1));

			}
			pos.x = startPos.x;
			pos.y += (50 / (rows - 1));

		}

		timesCompleted++;

	}
	Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();


	if (input->isKeyDown(INPUT_KEY_A))
	{
		paddlePosition.x -= 1 + (timesCompleted / 10.0f);
	}

	if (input->isKeyDown(INPUT_KEY_D))
	{
		paddlePosition.x += 1 + (timesCompleted / 10.0f);
	}

	if (input->isKeyDown(INPUT_KEY_Q))
	{
		paddle->m_rotation += 0.01f;
	}
	if (input->isKeyDown(INPUT_KEY_E))
	{
		paddle->m_rotation -= 0.01f;

	}
	for (Sphere* ball : balls) {
		if (ball->getPosition().y < -50 && gameLives > 0) { gameLives -= 1; ball->m_velocity.y *= -1; }
	}

	//Draw paddle
	paddle->setPosition(paddlePosition);

	// exit the application
	if (input->isKeyDown(INPUT_KEY_ESCAPE))
		quit();
}



void BreakoutGameApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	Gizmos::draw2D(ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	// output some text, uses the last used colour
	//char score[32];
	//sprintf_s(score, 32, "Score: %i", gameScore);
	//m_2dRenderer->drawText(m_font, score, 0, 720 - 32);

	char lives[32];
	sprintf_s(lives, 32, "Lives: %i", gameLives);
	m_2dRenderer->drawText(m_font, lives, 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();
}