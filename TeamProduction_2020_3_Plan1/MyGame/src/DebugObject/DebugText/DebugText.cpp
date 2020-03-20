#include "DebugText.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>

#include <TktkAppend2DComponent/Transform2DMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Text/TextDrawerMaker.h>

GameObjectPtr DebugText::create(const Vector2 & position, const std::string& initText)
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform2DMaker::makeStart(gameObject)
		.position(position)
		.create();

	tktk::TextDrawerMaker::makeStart(gameObject)
		.initText(initText)
		.create();

	return gameObject;
}
