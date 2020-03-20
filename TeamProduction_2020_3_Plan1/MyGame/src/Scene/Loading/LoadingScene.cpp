#include "LoadingScene.h"

#include <TktkComponentFramework/Scene/SceneManager.h>

#include "EnemyMeshLoader/EnemyMeshLoader.h"
#include "SphereMatCreater/SphereMatCreater.h"

#include "../Title/TitleScene.h"

#include <TktkDirectX11Wrapper/Graphics/RasterizerState/RasterizerState.h>

void LoadingScene::sceneStart()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = TRUE;
	tktk::RasterizerState::create(1, desc);

	EnemyMeshLoader::load();

	SphereMatCreater::create();

	tktk::SceneManager::enableScene<TitleScene>();
	tktk::SceneManager::disableScene<LoadingScene>();
}

void LoadingScene::sceneEnd()
{
}
