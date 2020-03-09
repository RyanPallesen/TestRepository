#pragma once
#include "Rigidbody.h"
class Sphere : public Rigidbody {
public:  ~Sphere();
	  virtual void OnCollide(PhysicsObject* other) {};

	  virtual void makeGizmo();

	  bool ContainsPoint(glm::vec2 point);

	  Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float elasticity = 1.0f, float linearDrag = 1.0f, float angularDrag = 1.0f);

	  void debug() {};

	  float getRadius() { return m_radius; }  glm::vec4 getColour() { return m_colour; }

	  float m_radius;  glm::vec4 m_colour;
};