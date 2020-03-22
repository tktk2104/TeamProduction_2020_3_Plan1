#include "Player.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>

#include <TktkAppend3DComponent/Transform3DMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Camera/SimpleCameraMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Light/PointLightMaker.h>
#include "Scripts/InputToRotate.h"
#include "Scripts/InputToMove.h"

#include "../../Ids/CameraId.h"
#include "../../Ids/LightId.h"

GameObjectPtr Player::create(const Vector3 & rotateCenterPos)
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform3DMaker::makeStart(gameObject).position(rotateCenterPos).create();

	gameObject->createComponent<InputToRotate>();

	gameObject->addChild(createPlayerCamera());

	return gameObject;
}

GameObjectPtr Player::createPlayerCamera()
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform3DMaker::makeStart(gameObject).position(Vector3(0.0f, 0.0f, -10.0f)).create();

	tktk::SimpleCameraMaker::makeStart(gameObject)
		.cameraId(CameraId::PlayerCamera)
		.cameraFov(45.0f)
		.cameraNear(0.1f)
		.cameraFar(1000.0f)
		.create();

	tktk::PointLightMaker::makeStart(gameObject)
		.lightId(LightId::basic)
		.create();

	gameObject->createComponent<InputToMove>();

	return gameObject;
}
