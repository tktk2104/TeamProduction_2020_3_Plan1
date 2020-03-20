#include "TitleScene.h"

#include "../../GameObject/Player/Player.h"
#include "../../GameObject/Enemy/Enemy.h"
#include "../../GameObject/ElementSphereManager/ElementSphereManager.h"

#include "../../DebugObject/DebugCamera/DebugCamera.h"

void TitleScene::sceneStart()
{
	Player::create(Vector3::zero);
	Enemy::create(Vector3::zero);
	ElementSphereManager::create();

	//DebugCamera::create(Vector3(0.0f, 0.0f, -10.0f));
}

void TitleScene::sceneEnd()
{
}
