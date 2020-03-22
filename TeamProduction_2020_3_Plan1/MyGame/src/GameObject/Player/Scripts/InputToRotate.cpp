#include "InputToRotate.h"

#include <TktkComponentFramework/Time/Time.h>
#include <TktkDirectX11Wrapper/Input/Keyboard/Keyboard.h>

#include <TktkAppend3DComponent/Transform3D.h>


void InputToRotate::update()
{
	Vector3 rotete = Vector3::zero;

	if (tktk::Keyboard::isPush(tktk::KEYBORD_A))	rotete.y =  100.0f;
	if (tktk::Keyboard::isPush(tktk::KEYBORD_D))	rotete.y = -100.0f;
	if (tktk::Keyboard::isPush(tktk::KEYBORD_W))	rotete.x =  100.0f;
	if (tktk::Keyboard::isPush(tktk::KEYBORD_S))	rotete.x = -100.0f;

	getComponent<tktk::Transform3D>()->addLocalEulerAngles(rotete * tktk::Time::deltaTime());
}
