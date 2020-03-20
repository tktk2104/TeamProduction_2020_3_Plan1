#include "Enemy.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>

#include <TktkAppend3DComponent/Transform3DMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Mesh/MeshDrawerMaker.h>

#include "../../Ids/MeshId.h"
#include "../../Ids/MaterialId.h"
#include "../../Ids/CameraId.h"

GameObjectPtr Enemy::create(const Vector3 & position)
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform3DMaker::makeStart(gameObject)
		.position(position)
		.create();

	tktk::MeshDrawerMaker::makeStart(gameObject)
		.drawPriority(0.0f)
		.cameraId(CameraId::PlayerCamera)
		.meshId(MeshId::Enemy_1)
		.skeltonId(-10)
		.materialIdArray(MaterialId::Enemy_1)
		.rasterizerStateId(1)
		.create();

	return gameObject;
}
