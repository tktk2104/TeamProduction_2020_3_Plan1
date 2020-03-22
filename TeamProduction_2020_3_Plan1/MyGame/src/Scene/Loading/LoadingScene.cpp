#include "LoadingScene.h"

#include <TktkComponentFramework/Scene/SceneManager.h>

#include "EnemyMeshLoader/EnemyMeshLoader.h"
#include "SphereMatCreater/SphereMatCreater.h"

#include "../Title/TitleScene.h"

#include <TktkDirectX11Wrapper/Graphics/RasterizerState/RasterizerState.h>
#include <TktkDirectX11Wrapper/Graphics/ConstantBuffer/ConstantBuffer.h>
#include <TktkDirectX11Wrapper/Graphics/VertexShader/VertexShader.h>
#include <TktkDirectX11Wrapper/Graphics/PixelShader/PixelShader.h>
#include "../../GameObject/ElementShape/VertexColor2DPolygonConstantBufferData.h"
#include "../../Ids/ConstantBufferId.h"
#include "../../Ids/VertexShaderId.h"
#include "../../Ids/PixelShaderId.h"


void LoadingScene::sceneStart()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = TRUE;
	tktk::RasterizerState::create(1, desc);

	tktk::ConstantBuffer::create(
		ConstantBufferId::VertexColor2DPolygon,
		new VertexColor2DPolygonConstantBufferData()
	);

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = std::vector<D3D11_INPUT_ELEMENT_DESC>({
			{ "POSITION",		0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		});
	tktk::VertexShader::load(
		VertexShaderId::VertexColor2DPolygon,
		"res/shader/VertexColor2DPolygonVertexShader.cso",
		layout,
		ConstantBufferId::VertexColor2DPolygon
	);

	tktk::PixelShader::load(
		PixelShaderId::VertexColor2DPolygon,
		"res/shader/VertexColor2DPolygonPixelShader.cso",
		ConstantBufferId::VertexColor2DPolygon
	);

	EnemyMeshLoader::load();

	SphereMatCreater::create();

	tktk::SceneManager::enableScene<TitleScene>();
	tktk::SceneManager::disableScene<LoadingScene>();
}

void LoadingScene::sceneEnd()
{
}
