#pragma once

#include <vector>
#include <TktkMath/Vector2.h>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>

struct ElementShape
{
	static GameObjectPtr create(const std::vector<Vector2>& lineVertexArray);
};