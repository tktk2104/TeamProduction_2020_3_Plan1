#include "InputToMove.h"

#include <cmath>
#include <TktkComponentFramework/Time/Time.h>
#include <TktkDirectX11Wrapper/Input/Keyboard/Keyboard.h>

#include <TktkAppend3DComponent/Transform3D.h>

void InputToMove::update()
{
	auto transform = getComponent<tktk::Transform3D>();

	if (tktk::Keyboard::isPush(tktk::KEYBORD_UP))
	{
		transform->addLocalPosition(Vector3(0.0f, 0.0f,  10.0f) * tktk::Time::deltaTime());
	}

	auto curPosition = transform->getLocalPosition();

	if (curPosition.z > -5.0f)
	{
		transform->setLocalPosition(Vector3(curPosition.x, curPosition.x,  -5.0f));
	}
	else if (curPosition.z > -10.0f)
	{
		transform->addLocalPosition(Vector3(0.0f, 0.0f, -5.0f) * tktk::Time::deltaTime());
	}
	else if (curPosition.z < -10.0f)
	{
		transform->setLocalPosition(Vector3(curPosition.x, curPosition.x, -10.0f));
	}
}
