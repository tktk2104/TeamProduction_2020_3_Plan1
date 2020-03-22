#pragma once

#include <TktkMath/Color.h>
#include <TktkMath/Vector2.h>

struct VertexColor2DPolygonConstantBufferData
{
	float worldMatrix[12];
	Vector2 screenSize;
	float pad2[2];
	Color blendRate;
};