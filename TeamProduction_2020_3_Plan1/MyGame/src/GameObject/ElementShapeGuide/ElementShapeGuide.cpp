#include "ElementShapeGuide.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>

#include <TktkAppend2DComponent/Transform2DMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Line2D/Line2DDrawerMaker.h>

GameObjectPtr ElementShapeGuide::create()
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform2DMaker::makeStart(gameObject).create();

	tktk::Line2DDrawerMaker::makeStart(gameObject)
		.drawPriority(100.0f)
		.lineColor(Color::white)
		.create();

	return gameObject;
}
