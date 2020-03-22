#pragma once

#include <vector>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>
#include "VertexColor2DPolygonVertexData.h"

struct ElementShape
{
	static GameObjectPtr create(const std::vector<VertexColor2DPolygonVertexData>& lineVertexArray);
};