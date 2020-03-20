#pragma once

#include <TktkMath/Vector3.h>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>

struct Player
{
	static GameObjectPtr create(const Vector3& rotateCenterPos);

private:

	static GameObjectPtr createPlayerCamera();
};