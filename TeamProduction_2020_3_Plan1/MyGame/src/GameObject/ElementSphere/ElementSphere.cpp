#include "ElementSphere.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>

#include <TktkAppend3DComponent/Transform3DMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Mesh/MeshDrawerMaker.h>
#include "Script/ElementColorChanger.h"

#include "../../Ids/CameraId.h"
#include "../../Ids/MeshId.h"

GameObjectPtr ElementSphere::create(const Vector3 & position, const Color & color, MaterialId id)
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform3DMaker::makeStart(gameObject).position(position).scaleRate(Vector3(0.1f)).create();

	tktk::MeshDrawerMaker::makeStart(gameObject)
		.drawPriority(0.0f)
		.cameraId(CameraId::PlayerCamera)
		.meshId(tktk::SystemMeshId::Sphere)
		.skeltonId(-10)
		.materialIdArray(id)
		.rasterizerStateId(tktk::SystemRasterizerStateId::Basic)
		.create();

	gameObject->createComponent<ElementColorChanger>(color, id);

	return gameObject;
}
