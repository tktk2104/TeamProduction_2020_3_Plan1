#include "ElementShape.h"

#include <TktkComponentFramework/GameObject/GameObject.h>
#include <TktkComponentFramework/GameObject/GameObjectManager.h>
#include <TktkDirectX11Wrapper/Graphics/Mesh/Mesh.h>

#include <TktkAppend2DComponent/Transform2DMaker.h>
#include "Script/ElementShapeScript.h"
#include "Script/VertexColor2DPolygonDrawer.h"

#include "../../Ids/MeshId.h"

#include <TktkDirectX11Wrapper/Graphics/BlendState/Asset/SystemBlendStateId.h>
#include <TktkDirectX11Wrapper/Graphics/DepthStencilState/Asset/SystemDepthStencilStateId.h>
#include <TktkDirectX11Wrapper/Graphics/RasterizerState/Asset/SystemRasterizerStateId.h>

GameObjectPtr ElementShape::create(const std::vector<VertexColor2DPolygonVertexData>& lineVertexArray)
{
	GameObjectPtr gameObject = tktk::GameObjectManager::createGameObject(false);

	tktk::Transform2DMaker::makeStart(gameObject).create();

	gameObject->createComponent<VertexColor2DPolygonDrawer>(
		100.0f,
		static_cast<int>(tktk::SystemBlendStateId::Alpha),
		static_cast<int>(tktk::SystemDepthStencilStateId::NotUseDepth),
		static_cast<int>(tktk::SystemRasterizerStateId::Basic),
		lineVertexArray,
		Color(1.0f, 1.0f, 1.0f, 0.8f)
		);

	gameObject->createComponent<ElementShapeScript>(lineVertexArray);

	return gameObject;
}
