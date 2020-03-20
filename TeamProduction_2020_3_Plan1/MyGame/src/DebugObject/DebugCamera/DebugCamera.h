#pragma once

#include <TktkMath/Vector3.h>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>

struct DebugCamera
{
	static GameObjectPtr create(const Vector3& position);
};