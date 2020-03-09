#pragma once
#include <glm\ext.hpp>
#include <vector>

enum ShapeType { PLANE = 0, SPHERE, BOX, SHAPE_COUNT };


class PhysicsObject {
protected:
	PhysicsObject() {};
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}


public:

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};
	ShapeType getShapeID() { return m_shapeID; };
	ShapeType m_shapeID;
};

