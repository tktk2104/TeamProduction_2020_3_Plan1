#include "ElementSphereManager.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>
#include "../../Ids/GameObjectTag.h"
#include "Script/ElementSphereManagerScript.h"

GameObjectPtr ElementSphereManager::create()
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	gameObject->addTag(static_cast<int>(GameObjectTag::ElementSphereManager));

	gameObject->createComponent<ElementSphereManagerScript>();

	return gameObject;
}
