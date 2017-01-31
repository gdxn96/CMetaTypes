#include "stdafx.h"

#include "SpinningBox.h"

DEFINE_META(SpinningBox)
{
	ADD_RE_MEMBER(angVel);
	ADD_RE_MEMBER(col);
	ADD_RE_MEMBER(rect);
	LINK_TO_JSON(SpinningBox);
}

SpinningBox::SpinningBox()
{
	jsonInit();
}


void SpinningBox::Render(Renderer& r) {
	r.drawRect(rect, col);
}


void SpinningBox::Update(float deltaTime) {

	float angle = angVel*deltaTime;
	Point2D p = rect.pos;//current position

						 //apply a rotation about the origin transform
	rect.pos.x = (float)cos(angle)*p.x - (float)sin(angle)*p.y;
	rect.pos.y = (float)sin(angle)*p.x + (float)cos(angle)*p.y;

}

void SpinningBox::onEvent(EventListener::Event evt)
{
}