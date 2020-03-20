#include "LoadingScene.h"

#include <TktkComponentFramework/Scene/SceneManager.h>
#include "SphereMatCreater/SphereMatCreater.h"
#include "../Title/TitleScene.h"

void LoadingScene::sceneStart()
{
	SphereMatCreater::create();

	tktk::SceneManager::enableScene<TitleScene>();
	tktk::SceneManager::disableScene<LoadingScene>();
}

void LoadingScene::sceneEnd()
{
}
