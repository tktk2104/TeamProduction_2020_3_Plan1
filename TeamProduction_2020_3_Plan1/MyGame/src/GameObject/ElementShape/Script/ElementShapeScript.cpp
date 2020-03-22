#include "ElementShapeScript.h"

#include <cmath>
#include <TktkDirectX11Wrapper/Input/Keyboard/Keyboard.h>
#include "../../../DebugObject/DebugText/DebugText.h"

ElementShapeScript::ElementShapeScript(const std::vector<VertexColor2DPolygonVertexData>& lineVertexArray)
	: m_lineVertexArray(lineVertexArray)
{
}

void ElementShapeScript::start()
{
	VertexColor2DPolygonVertexData center;
	for (const auto& node : m_lineVertexArray)
	{
		center.position += node.position;
		center.vertexColor += node.vertexColor;
	}
	center.position /= m_lineVertexArray.size();
	center.vertexColor /= m_lineVertexArray.size();


	float shapeSize = 0.0f;
	float shapeRedPowerRate = 0.0f;
	float shapeGreenPowerRate = 0.0f;
	float shapeBluePowerRate = 0.0f;

	for (unsigned int i = 0; i < m_lineVertexArray.size(); i++)
	{
		const VertexColor2DPolygonVertexData& firstVertex = m_lineVertexArray.at(i);
		const VertexColor2DPolygonVertexData& secondVertex = m_lineVertexArray.at((i + 1) % (m_lineVertexArray.size() - 1));

		float a = (center.position			- firstVertex.position	).length();
		float b = (firstVertex.position		- secondVertex.position	).length();
		float c = (secondVertex.position	- center.position		).length();
		float s = (a + b + c) / 2.0;

		float triangleSize = std::sqrt(s*(s - a)*(s - b)*(s - c));

		shapeSize += triangleSize;

		shapeRedPowerRate	+= triangleSize * (center.vertexColor.r + firstVertex.vertexColor.r + secondVertex.vertexColor.r) / 3;
		shapeGreenPowerRate += triangleSize * (center.vertexColor.g + firstVertex.vertexColor.g + secondVertex.vertexColor.g) / 3;
		shapeBluePowerRate	+= triangleSize * (center.vertexColor.b + firstVertex.vertexColor.b + secondVertex.vertexColor.b) / 3;
	}

	shapeRedPowerRate	/= shapeSize;
	shapeGreenPowerRate /= shapeSize;
	shapeBluePowerRate	/= shapeSize;

	getGameObject()->addChild(DebugText::create(Vector2(30.0f,  30.0f), "vertexCount:"	+ std::to_string(m_lineVertexArray.size())));
	getGameObject()->addChild(DebugText::create(Vector2(30.0f, 100.0f), "areaSize___:"	+ std::to_string(static_cast<int>(shapeSize))));
	getGameObject()->addChild(DebugText::create(Vector2(30.0f, 170.0f), "redPower___:"	+ std::to_string(static_cast<int>(shapeRedPowerRate * 100)) + "%"));
	getGameObject()->addChild(DebugText::create(Vector2(30.0f, 240.0f), "greenPower_:"	+ std::to_string(static_cast<int>(shapeGreenPowerRate * 100)) + "%"));
	getGameObject()->addChild(DebugText::create(Vector2(30.0f, 310.0f), "bluePower__:"	+ std::to_string(static_cast<int>(shapeBluePowerRate * 100)) + "%"));
}

void ElementShapeScript::update()
{
	if (tktk::Keyboard::isTrigger(tktk::KEYBORD_SPACE) || tktk::Keyboard::isTrigger(tktk::KEYBORD_RETURN))
	{
		getGameObject()->destroy();
	}
}
