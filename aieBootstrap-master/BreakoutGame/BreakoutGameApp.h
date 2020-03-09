#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

class BreakoutGameApp : public aie::Application {
public:

	BreakoutGameApp();

	void BallHitBrick(PhysicsObject* self, PhysicsObject* brick);

	virtual ~BreakoutGameApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	int gameScore;
	int gameLives = 5;

	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;
	PhysicsScene* m_physicsScene;
};