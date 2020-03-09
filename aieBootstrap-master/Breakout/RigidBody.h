#pragma once
#include "PhysicsObject.h"
class Rigidbody : public PhysicsObject {
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag) { m_shapeID = shapeID, m_position = position, m_velocity = velocity, m_mass = mass, m_rotation = rotation; m_elasticity = elasticity, m_linearDrag = linearDrag, m_angularDrag = angularDrag; };
	~Rigidbody() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	void applyForce(glm::vec2 force, glm::vec2 pos);
	void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	virtual bool ContainsPoint(glm::vec2 point) { return false; };

	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }

	bool isKinematic()
	{
		return m_kinematic;
	}

	void setKinematic(bool var)
	{
		m_kinematic = var;
	}

	void setPosition(glm::vec2 position)
	{
		m_position = position;
	}
public:  
	  glm::vec2 m_position;
	  glm::vec2 m_velocity;
	  float m_mass;
	  float m_linearDrag = 0.0f;
	  float m_angularDrag = 1;
	  float m_rotation = 5;
	  float m_angularVelocity = 1;
	  float m_moment = 0;
	  float m_elasticity = 1.0f;
	  bool m_kinematic = false;
};