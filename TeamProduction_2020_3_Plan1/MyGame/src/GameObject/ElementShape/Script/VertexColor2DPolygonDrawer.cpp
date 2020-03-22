#include "VertexColor2DPolygonDrawer.h"

#include <stdexcept>
#include <array>
#include <d3d11.h>
#include <TktkDirectX11Wrapper/Graphics/Window/Window.h>
#include <TktkDirectX11Wrapper/Graphics/Screen/Screen.h>
#include <TktkDirectX11Wrapper/Graphics/ConstantBuffer/ConstantBuffer.h>
#include <TktkDirectX11Wrapper/Graphics/BlendState/BlendState.h>
#include <TktkDirectX11Wrapper/Graphics/DepthStencilState/DepthStencilState.h>
#include <TktkDirectX11Wrapper/Graphics/Mesh/Mesh.h>
#include <TktkDirectX11Wrapper/Graphics/RasterizerState/RasterizerState.h>
#include <TktkDirectX11Wrapper/Graphics/VertexShader/VertexShader.h>
#include <TktkDirectX11Wrapper/Graphics/PixelShader/PixelShader.h>
#include "../VertexColor2DPolygonConstantBufferData.h"

#include "../../../Ids/VertexShaderId.h"
#include "../../../Ids/PixelShaderId.h"
#include "../../../Ids/ConstantBufferId.h"
#include "../../../Ids/MeshId.h"

VertexColor2DPolygonDrawer::VertexColor2DPolygonDrawer(
	float drawPriority,
	int blendStateId,
	int depthStencilStateId,
	int rasterizerStateId,
	const std::vector<VertexColor2DPolygonVertexData>& vertexArray,
	const Color & blendRate
)
	: ComponentBase(drawPriority)
	, m_blendStateId(blendStateId)
	, m_depthStencilStateId(depthStencilStateId)
	, m_rasterizerStateId(rasterizerStateId)
	, m_vertexArray(vertexArray)
	, m_blendRate(blendRate)
{
}

void VertexColor2DPolygonDrawer::start()
{
	VertexColor2DPolygonVertexData center;

	for (const auto& node : m_vertexArray)
	{
		center.position += node.position;
		center.vertexColor += node.vertexColor;
	}

	center.position /= m_vertexArray.size();
	center.vertexColor /= m_vertexArray.size();

	m_vertexArray.push_back(center);

	tktk::VertexBufferInitParams vertexBufferParams;
	vertexBufferParams.stride = sizeof(VertexColor2DPolygonVertexData);
	vertexBufferParams.offset = 0U;
	vertexBufferParams.bufferSize = sizeof(VertexColor2DPolygonVertexData) * m_vertexArray.size();
	vertexBufferParams.bufferData = m_vertexArray.data();

	tktk::IndexBufferInitParams indexBufferParams;
	indexBufferParams.indices;

	for (unsigned int i = 0; i < (m_vertexArray.size() - 1); i++)
	{
		indexBufferParams.indices.push_back(m_vertexArray.size() - 1);
		indexBufferParams.indices.push_back((i + 0));
		indexBufferParams.indices.push_back((i + 1) % (m_vertexArray.size() - 1));
	}

	tktk::MaterialSlotsInitParams materialSlotsParams;
	materialSlotsParams.subsets.push_back({ 0, static_cast<int>((m_vertexArray.size() - 1) * 3) });

	tktk::Mesh::create(
		MeshId::ElementShape,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		vertexBufferParams,
		indexBufferParams,
		materialSlotsParams
	);

	m_transform = getComponent<tktk::Transform2D>();

	if (m_transform.isNull())
	{
		throw std::runtime_error("SpriteDrawer not found Transform2D");
	}
}

void VertexColor2DPolygonDrawer::draw() const
{
	std::array<float, 4> factor = { 1.0f, 1.0f, 1.0f, 1.0f };
	tktk::Screen::getDeviceContextPtr()->OMSetBlendState(tktk::BlendState::getDataPtr(m_blendStateId)->getStatePtr(), factor.data(), 0xffffffff);
	tktk::Screen::getDeviceContextPtr()->OMSetDepthStencilState(tktk::DepthStencilState::getDataPtr(m_depthStencilStateId)->getStatePtr(), 0);

	// 定数バッファに値を詰め詰めする
	tktk::ConstantBufferData* constantBufferData = tktk::ConstantBuffer::getDataPtr(ConstantBufferId::VertexColor2DPolygon);

	VertexColor2DPolygonConstantBufferData vertexColor2DPolygonConstantBufferData;

	Matrix3 worldMat = m_transform->calculateWorldMatrix();

	for (unsigned int i = 0; i < 12; i++)
	{
		if (i % 4U == 3) continue;

		vertexColor2DPolygonConstantBufferData.worldMatrix[i] = worldMat.m[i / 4U][i % 4U];
	}

	vertexColor2DPolygonConstantBufferData.blendRate = m_blendRate;
	vertexColor2DPolygonConstantBufferData.screenSize = tktk::Window::getWindowSize();
	constantBufferData->setBufferData(std::move(vertexColor2DPolygonConstantBufferData));
	constantBufferData->updateBuffer();

	// ラスタライザステートをセット
	tktk::RasterizerState::getData(m_rasterizerStateId).setState();

	// シェーダーをセット
	tktk::VertexShader::getData(VertexShaderId::VertexColor2DPolygon).beginVertexShader();
	tktk::PixelShader::getData(PixelShaderId::VertexColor2DPolygon).beginShader();

	tktk::MeshData* meshDataPtr = tktk::Mesh::getDataPtr(MeshId::ElementShape);

	// 頂点バッファとインデックスバッファをレンダリングパイプラインに設定する
	meshDataPtr->setVertexAndIndexBuffer();

	meshDataPtr->setPrimitiveTopology();

	// ドローコール
	tktk::Screen::getDeviceContextPtr()->DrawIndexed(meshDataPtr->getSubset(0).indexBufferUseCount, meshDataPtr->getSubset(0).indexBufferStartPos, 0);
}

void VertexColor2DPolygonDrawer::setVertexArray(const std::vector<VertexColor2DPolygonVertexData>& vertexArray)
{
	m_vertexArray = vertexArray;

	tktk::VertexBufferInitParams vertexBufferParams;
	vertexBufferParams.stride = sizeof(VertexColor2DPolygonVertexData);
	vertexBufferParams.offset = 0U;
	vertexBufferParams.bufferSize = sizeof(VertexColor2DPolygonVertexData) * m_vertexArray.size();
	vertexBufferParams.bufferData = m_vertexArray.data();

	tktk::IndexBufferInitParams indexBufferParams;
	indexBufferParams.indices;

	for (unsigned int i = 0; i < (m_vertexArray.size() - 2); i++)
	{
		indexBufferParams.indices.push_back(0);
		indexBufferParams.indices.push_back(i + 1);
		indexBufferParams.indices.push_back(i + 2);
	}

	tktk::MaterialSlotsInitParams materialSlotsParams;
	materialSlotsParams.subsets.push_back({ 0, 4 });

	tktk::Mesh::create(
		MeshId::ElementShape,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		vertexBufferParams,
		indexBufferParams,
		materialSlotsParams
	);
}

void VertexColor2DPolygonDrawer::setBlendRate(const Color & blendRate)
{
	m_blendRate = blendRate;
}

void VertexColor2DPolygonDrawer::setBlendStateIdImpl(int id)
{
	m_blendStateId = id;
}

void VertexColor2DPolygonDrawer::setDepthStencilStateIdImpl(int id)
{
	m_depthStencilStateId = id;
}

void VertexColor2DPolygonDrawer::setRasterizerStateIdImpl(int id)
{
	m_rasterizerStateId = id;
}
