#include "ElementShape.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>

#include <TktkAppend2DComponent/Transform2DMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Line2D/Line2DDrawerMaker.h>

GameObjectPtr ElementShape::create(const std::vector<Vector2>& lineVertexArray)
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform2DMaker::makeStart(gameObject).create();

	tktk::Line2DDrawerMaker::makeStart(gameObject)
		.drawPriority(100.0f)
		.lineVertexArray(lineVertexArray)
		.lineColor(Color::white)
		.create();

	return gameObject;
}
