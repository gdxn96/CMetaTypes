#pragma once

#include "GameObject.h"
#include "BasicTypes.h"
#include "EventListener.h"
#include "../../../MetaSystem/MetaSystem.h"

/**define a box that spins aronf the centre*/
class SpinningBox : public RuntimeEditable<SpinningBox>, public GameObject, public EventListener
{
private:
	Rect rect;
	Colour col;

	float angVel; //control rate of spin
public:
	SpinningBox(Rect r) :rect(r) { jsonInit(); angVel = .0f; LINK_TO_JSON(SpinningBox);
	};
	SpinningBox();

	META_DATA(SpinningBox);

	void Render(Renderer & r);

	void Update(float deltaTime);

	void onEvent(EventListener::Event);
};