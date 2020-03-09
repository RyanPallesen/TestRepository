#include "Box.h"
#include <Gizmos.h>
#include <iostream>

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 halfExtents, glm::vec4 colour, float rotation, float elasticity, float linearDrag, float angularDrag) : Rigidbody(BOX, position, velocity, rotation, mass, elasticity, linearDrag, angularDrag)
{
	m_colour = colour;
	m_extents = halfExtents; 
	m_moment = 1.0f / 12.0f * (mass * halfExtents.x * halfExtents.y);

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));

}


void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{//store the local axes
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));

	Rigidbody::fixedUpdate(gravity, timeStep);


}

// Check if a point lies within the OBB
bool Box::ContainsPoint(glm::vec2 point)
{
	// Convert the point to box local space
	glm::vec2 newPoint(glm::dot(point - m_position, m_localX), glm::dot(point - m_position, m_localY));

	return newPoint.x < m_extents.x && newPoint.x > -m_extents.x && newPoint.y < m_extents.y && newPoint.y > -m_extents.y;
}

void Box::makeGizmo()
{
	// if only using rotation
	//glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));
	//aie::Gizmos::add2DAABBFilled(getCenter(), m_extents, m_colour, &transform);
	// draw using local axes

	//std::cout << "p1 " << m_localY.x << " " << m_localY.y << std::endl;

	
	
		glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
		glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
		glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
		glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;

		aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
		aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
	

	//std::cout << "p1 " << p1.x << " " << p1.y <<std::endl;
	//std::cout << "p2 " << p2.x << " " << p2.y <<std::endl;
	//std::cout << "p3 " << p3.x << " " << p3.y <<std::endl;
	//std::cout << "p4 " << p4.x << " " << p4.y <<std::endl;
	//std::cout << "m_position " << m_position.x << " " << m_position.y <<std::endl;
	//std::cout << "m_extents " << m_extents.x << " " << m_extents.y <<std::endl;
	//std::cout << "m_localY " << m_localY.x << " " << m_localY.y <<std::endl;
	//std::cout << "m_localX " << m_localX.x << " " << m_localX.y <<std::endl;
	//std::cout << "m_rotation " << m_rotation <<std::endl;
	//std::cout << "m_angularVelocity " << m_angularVelocity <<std::endl;
	//std::cout << "m_angularDrag " << m_angularDrag <<std::endl;
	//std::cout << "m_linearDrag " << m_linearDrag <<std::endl;
	//std::cout << std::endl;

}

bool Box::checkBoxCorners(const Box& box, glm::vec2& contact, int& numContacts,
	float& pen, glm::vec2& edgeNormal)
{
	float minX = 0, maxX = 0, minY = 0, maxY = 0;
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;
	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			// position in worldspace
			glm::vec2 p = box.getCenter() + x * box.m_localX + y * box.m_localY;
			// position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX),
				glm::dot(p - m_position, m_localY));
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}
	if (maxX <-m_extents.x || minX >m_extents.x || maxY<-m_extents.y || minY >m_extents.y)
		return false;
	if (numLocalContacts == 0)
		return false;
	bool res = false;
	contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) /
		(float)numLocalContacts;
	numContacts++;
	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}
	return res;
}
