#pragma once

#include <vector>
#include <TktkMetaFunc/TypeCheck/isIdType.h>
#include <TktkComponentFramework/Component/ComponentBase.h>
#include <TktkAppend2DComponent/Transform2D.h>
#include "../VertexColor2DPolygonVertexData.h"

class VertexColor2DPolygonDrawer
	: public tktk::ComponentBase
{
public:

	VertexColor2DPolygonDrawer(
		float drawPriority,
		int blendStateId,
		int depthStencilStateId,
		int rasterizerStateId,
		const std::vector<VertexColor2DPolygonVertexData>& vertexArray,
		const Color& blendRate
	);

public:

	void start();
	void draw() const;

public:

	// ブレンドステートIDを再設定する（列挙型を含む整数型のidが渡された場合のみビルド可）
	template <class IdType, std::enable_if_t<is_idType_v<IdType>>* = nullptr>
	void setBlendStateId(IdType id)
	{
		setBlendStateIdImpl(static_cast<int>(id));
	}
	template <class IdType, std::enable_if_t<!is_idType_v<IdType>>* = nullptr>
	void setBlendStateId(IdType id) { static_assert(false, "BlendStateId Fraud Type"); }

	// 深度ステンシルステートIDを再設定する（列挙型を含む整数型のidが渡された場合のみビルド可）
	template <class IdType, std::enable_if_t<is_idType_v<IdType>>* = nullptr>
	void setDepthStencilStateId(IdType id)
	{
		setDepthStencilStateIdImpl(static_cast<int>(id));
	}
	template <class IdType, std::enable_if_t<!is_idType_v<IdType>>* = nullptr>
	void setDepthStencilStateId(IdType id) { static_assert(false, "DepthStencilStateId Fraud Type"); }

	// ラスタライザステートIDを再設定する（列挙型を含む整数型のidが渡された場合のみビルド可）
	template <class IdType, std::enable_if_t<is_idType_v<IdType>>* = nullptr>
	void setRasterizerStateId(IdType id)
	{
		setRasterizerStateIdImpl(static_cast<int>(id));
	}
	template <class IdType, std::enable_if_t<!is_idType_v<IdType>>* = nullptr>
	void setRasterizerStateId(IdType id) { static_assert(false, "RasterizerStateId Fraud Type"); }

	// 頂点情報を再設定する（時計回りに頂点を指定してください）
	void setVertexArray(const std::vector<VertexColor2DPolygonVertexData>& vertexArray);

	// ブレンドレートを再設定する
	void setBlendRate(const Color& blendRate);

private:

	// 各種id指定系の関数の実装
	void setBlendStateIdImpl(int id);
	void setDepthStencilStateIdImpl(int id);
	void setRasterizerStateIdImpl(int id);

private:

	int m_blendStateId{ -1 };
	int m_depthStencilStateId{ -1 };
	int m_rasterizerStateId{ -1 };
	std::vector<VertexColor2DPolygonVertexData> m_vertexArray{};
	Color m_blendRate{ 1.0f, 1.0f, 1.0f, 1.0f };

	tktk::CfpPtr<tktk::Transform2D> m_transform{  };
};