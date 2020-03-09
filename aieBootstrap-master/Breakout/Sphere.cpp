#include "Sphere.h"
#include <Gizmos.h>
#include <iostream>

void Sphere::makeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}

bool Sphere::ContainsPoint(glm::vec2 point)
{
	if (glm::distance(point, m_position) < m_radius)
	{
		return true;
	}
	return false;
}

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float elasticity, float linearDrag, float angularDrag) : Rigidbody(SPHERE, position, velocity, 0, mass, elasticity, linearDrag, angularDrag) { m_radius = radius;  m_colour = colour; m_moment = 0.5f * mass * radius * radius; }
