#pragma once
#include "RigidBody.h"
class Box :
	public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 halfExtents, glm::vec4 colour, float rotation, float elasticity = 1.0f, float linearDrag = 1.0f, float angularDrag = 1.0f);


	float getElasticity() { return m_elasticity; }
	float getWidth() { return m_extents.x * 2; }
	float getHeight() { return m_extents.y * 2; }
	glm::vec2 getExtents() const { return m_extents; }
	glm::vec2  getLocalX() const { return m_localX; }
	glm::vec2  getLocalY() const { return m_localY; }
	float getAngularVelocity() { return m_angularVelocity; }
	float getMoment() { return m_moment; }
	glm::vec2 getCenter() const { return m_position; }

	bool checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);


private:

	glm::vec2 m_extents; // the halfedge lengths
	glm::vec4 m_colour;
	// store the local x,y axes of the box based on its angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;



	void fixedUpdate(glm::vec2 gravity, float timeStep);

	bool ContainsPoint(glm::vec2 point);

	virtual void makeGizmo();

	void debug() {};

};

