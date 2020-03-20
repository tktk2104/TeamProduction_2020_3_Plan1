#pragma once

#include <TktkMath/Color.h>
#include <TktkMath/Vector3.h>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>
#include "../../Ids/MaterialId.h"

struct ElementSphere
{
	static GameObjectPtr create(const Vector3& position, const Color& color, MaterialId id);
};