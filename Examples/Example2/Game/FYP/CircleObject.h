#pragma once
#include "GameObject.h"
#include "BasicTypes.h"

class CircleObject : public GameObject
{
public:
	CircleObject(Point2D position, Point2D velocity, float radius);
	void Render(Renderer& r) override;
	void Update(float deltaTime) override;
	Point2D getTopLeft() override;
	void SetBounds(Rect r);
private:
	float m_radius;
	Point2D m_velocity;
	Rect m_bounds;
	Point2D m_position;
	Colour m_colour;
};
