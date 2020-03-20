#pragma once

#include <string>
#include <TktkMath/Vector2.h>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>

struct DebugText
{
	static GameObjectPtr create(const Vector2& position, const std::string& initText);
};