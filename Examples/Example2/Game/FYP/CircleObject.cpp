#include "stdafx.h"
#include "CircleObject.h"

CircleObject::CircleObject(Point2D position, Point2D velocity, float radius) : m_velocity(velocity), m_radius(radius)
{
}

void CircleObject::Render(Renderer & r)
{
	r.drawCircle(m_position, m_radius, m_colour);
}

void CircleObject::Update(float deltaTime)
{
	m_position = m_position + (m_velocity * deltaTime);
	if (m_position.x > (m_bounds.size.w + m_bounds.pos.x) - m_radius)
	{
		m_position.x = (m_bounds.size.w + m_bounds.pos.x) - m_radius;
		m_velocity.x = -m_velocity.x;
	}
	if (m_position.y > (m_bounds.size.h + m_bounds.pos.y) - m_radius)
	{
		m_position.y = (m_bounds.size.h + m_bounds.pos.y) - m_radius;
		m_velocity.y = -m_velocity.y;
	}
	if (m_position.x < m_bounds.pos.x + m_radius)
	{
		m_position.x = m_bounds.pos.x + m_radius;
		m_velocity.x = -m_velocity.x;
	}

	if (m_position.y < m_bounds.pos.y + m_radius)
	{			   
		m_position.y = m_bounds.pos.y + m_radius;
		m_velocity.y = -m_velocity.y;
	}
}



Point2D CircleObject::getTopLeft()
{
	Point2D topLeft = m_position - Point2D(m_radius, m_radius);
	return topLeft;
}

void CircleObject::SetBounds(Rect r)
{
	m_bounds = r;
}
