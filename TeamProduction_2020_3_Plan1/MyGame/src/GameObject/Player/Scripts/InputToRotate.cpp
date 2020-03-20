#include "InputToRotate.h"

#include <cmath>
#include <TktkMath/MathHelper.h>
#include <TktkComponentFramework/Time/Time.h>
#include <TktkDirectX11Wrapper/Input/Keyboard/Keyboard.h>

#include <TktkAppend3DComponent/Transform3D.h>

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkAppendDirectX11Component/Graphics/Text/TextDrawer.h>
#include "../../../DebugObject/DebugText/DebugText.h"

void InputToRotate::start()
{
	m_debugTextObjArray.push_back(DebugText::create(Vector2(20.0f,  20.0f), "rote_x:---"));
	m_debugTextObjArray.push_back(DebugText::create(Vector2(20.0f,  80.0f), "rote_y:---"));
	m_debugTextObjArray.push_back(DebugText::create(Vector2(20.0f, 140.0f), "rote_z:---"));
}

void InputToRotate::update()
{
	auto transform = getComponent<tktk::Transform3D>();

	Vector3 rotete = Vector3::zero;

	if (tktk::Keyboard::isPush(tktk::KEYBORD_LEFT))		rotete.y = 1.0f;
	if (tktk::Keyboard::isPush(tktk::KEYBORD_RIGHT))	rotete.y = -1.0f;
	if (tktk::Keyboard::isPush(tktk::KEYBORD_UP))		rotete.x = 1.0f;
	if (tktk::Keyboard::isPush(tktk::KEYBORD_DOWN))		rotete.x = -1.0f;

	transform->addLocalEulerAngles(rotete);

	Vector3 curEuler = transform->calculateLocalEulerAngles();
	m_debugTextObjArray.at(0)->getComponent<tktk::TextDrawer>()->setText("rote_x:" + std::to_string(curEuler.x));
	m_debugTextObjArray.at(1)->getComponent<tktk::TextDrawer>()->setText("rote_y:" + std::to_string(curEuler.y));
	m_debugTextObjArray.at(2)->getComponent<tktk::TextDrawer>()->setText("rote_z:" + std::to_string(curEuler.z));
}
