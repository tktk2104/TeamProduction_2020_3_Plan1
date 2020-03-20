#include "DebugCamera.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>

#include <TktkAppend3DComponent/Transform3DMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Camera/SimpleCameraMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Camera/Module/FirstPersonModuleMaker.h>
#include <TktkAppendDirectX11Component/Graphics/Light/PointLightMaker.h>

#include "../../Ids/CameraId.h"
#include "../../Ids/LightId.h"

GameObjectPtr DebugCamera::create(const Vector3 & position)
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform3DMaker::makeStart(gameObject).position(position).create();

	tktk::SimpleCameraMaker::makeStart(gameObject)
		.cameraId(CameraId::PlayerCamera)
		.cameraFov(45.0f)
		.cameraNear(0.1f)
		.cameraFar(1000.0f)
		.create();

	tktk::FirstPersonModuleMaker::makeStart(gameObject)
		.moveSpeedPerSec(16.0f)
		.rotateDegSpeedPerSec(50.0f)
		.alwaysMoveForward(false)
		.enableUpDownKey(true)
		.create();

	tktk::PointLightMaker::makeStart(gameObject)
		.lightId(LightId::basic)
		.create();

	return gameObject;
}
