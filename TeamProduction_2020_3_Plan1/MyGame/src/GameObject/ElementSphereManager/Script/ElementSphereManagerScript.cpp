#include "ElementSphereManagerScript.h"

#include <stdexcept>
#include <limits>
#include <cmath>
#include <TktkMath/Random.h>
#include <TktkDirectX11Wrapper/Input/Keyboard/Keyboard.h>
#include <TktkDirectX11Wrapper/Graphics/Window/Window.h>
#include <TktkDirectX11Wrapper/Graphics/Camera/Camera.h>

#include <TktkAppend3DComponent/Transform3D.h>
#include <TktkAppendDirectX11Component/Graphics/Line2D/Line2DDrawer.h>
#include "../../ElementSphere/Script/ElementColorChanger.h"

#include "../../ElementSphere/ElementSphere.h"
#include "../../ElementShapeGuide/ElementShapeGuide.h"
#include "../../ElementShape/ElementShape.h"
#include "../../../Ids/CameraId.h"

constexpr unsigned int ElementSphereManagerScript::m_firstElementMaterialId;
constexpr unsigned int ElementSphereManagerScript::m_lastElementMaterialId;
constexpr unsigned int ElementSphereManagerScript::m_useMatArraySize;

void ElementSphereManagerScript::start()
{
	for (unsigned int i = 0; i < m_useMatArraySize; i++)
	{
		Color sphereColor;

		switch (Random::getRandI(1, 3))
		{
		case 1: sphereColor = Color::red;	break;
		case 2: sphereColor = Color::green; break;
		case 3: sphereColor = Color::blue;	break;
		}

		createElementSphere(
			Vector3(
				Random::getRandF(-3.0f, 3.0f),
				Random::getRandF(-3.0f, 3.0f),
				Random::getRandF(-3.0f, 3.0f)
			),
			sphereColor
		);
	}

	m_elementShapeGuide = ElementShapeGuide::create();
}

void ElementSphereManagerScript::update()
{
	m_elementSphereObjectList.remove_if(
		[](GameObjectPtr& node) { return node.isNull(); }
	);

	// 使用しているカメラの情報
	auto cameraDataPtr = tktk::Camera::getDataPtr(CameraId::PlayerCamera);

	// スクリーン空間上の点の配列
	std::vector<VertexColor2DPolygonVertexData> elementSpherePointArray;

	// X座標が最大、最小の座標のインデックス
	unsigned int maxXPosIndex = 0U;
	unsigned int minXPosIndex = 0U;

	// Y座標が最大、最小の座標のインデックス
	unsigned int maxYPosIndex = 0U;
	unsigned int minYPosIndex = 0U;

	// 現在のインデックス
	unsigned int curIndex = 0U;

	for (const auto& node : m_elementSphereObjectList)
	{
		auto transform = node->getComponent<tktk::Transform3D>();
		const auto& screenSize = tktk::Window::getWindowSize();

		// スクリーン座標を求める
		Vector3 result = transform->getWorldPosition() * (*cameraDataPtr->getViewMatrixPtr()) * (*cameraDataPtr->getProjectionMatrixPtr());
		result /= result.z;
		result = result * Matrix4::createScreen(0.0f, 0.0f, screenSize.x, screenSize.y);

		// もしも頂点が画面外に出ていた場合、何もしない
		if (result.x < 0.0f || result.x > screenSize.x || result.y < 0.0f || result.y > screenSize.y) continue;

		// スクリーン空間上の点の配列を追加する
		VertexColor2DPolygonVertexData tempData;
		tempData.position = Vector2(result.x, result.y);
		tempData.vertexColor = node->getComponent<ElementColorChanger>()->getColor();

		elementSpherePointArray.push_back(tempData);

		// 座標それぞれの成分の最大、最小の座標のインデックスを更新する
		if (elementSpherePointArray.at(maxXPosIndex).position.x < result.x) maxXPosIndex = curIndex;
		if (elementSpherePointArray.at(minXPosIndex).position.x > result.x) minXPosIndex = curIndex;
		if (elementSpherePointArray.at(maxYPosIndex).position.y < result.y) maxYPosIndex = curIndex;
		if (elementSpherePointArray.at(minYPosIndex).position.y > result.y) minYPosIndex = curIndex;

		curIndex++;
	}

	// 最大、最小ではない頂点のインデックスのリスト
	std::forward_list<unsigned int> notMaxOrMinIndexList;

	// 現在の頂点がいずれの成分の最大、最小値で無ければ「最大、最小ではない頂点のインデックスのリスト」に追加する
	for (unsigned int i = 0; i < elementSpherePointArray.size(); i++)
	{
		if (maxXPosIndex != i || minXPosIndex != i || maxYPosIndex != i || minYPosIndex != i)
		{
			notMaxOrMinIndexList.push_front(i);
		}
	}

	// 頂点が１つも無かったら何もしない
	if (curIndex == 0U) return;

	// 作成する図形の頂点の配列
	std::vector<Vector2> elementShapeGuideVertexArray;
	// 作成する図形の頂点カラー込みの頂点情報の配列
	std::vector<VertexColor2DPolygonVertexData> elementShapeVertexArray;

	// 直前に追加した頂点を持つ変数
	Vector2 previousAddPos = elementSpherePointArray.at(minXPosIndex).position;

	// 「ｘ座標が最小の座標」を最初の頂点として配列に追加する
	elementShapeGuideVertexArray.push_back(previousAddPos);
	elementShapeVertexArray.push_back(elementSpherePointArray.at(minXPosIndex));

	// 「ｙ座標が最小の座標」が「ｘ座標が最小の座標」と同じでなければ
	if (minYPosIndex != minXPosIndex)
	{
		addElementShapeVertexArray(
			&elementShapeGuideVertexArray,
			&elementShapeVertexArray,
			&notMaxOrMinIndexList,
			previousAddPos,
			elementSpherePointArray.at(minYPosIndex).position,
			elementSpherePointArray
		);

		// 「ｙ座標が最小の座標」を最初の頂点として配列に追加する
		previousAddPos = elementSpherePointArray.at(minYPosIndex).position;
		elementShapeGuideVertexArray.push_back(previousAddPos);
		elementShapeVertexArray.push_back(elementSpherePointArray.at(minYPosIndex));
	}

	if (maxXPosIndex != minXPosIndex && maxXPosIndex != minYPosIndex)
	{
		addElementShapeVertexArray(
			&elementShapeGuideVertexArray,
			&elementShapeVertexArray,
			&notMaxOrMinIndexList,
			previousAddPos,
			elementSpherePointArray.at(maxXPosIndex).position,
			elementSpherePointArray
		);

		previousAddPos = elementSpherePointArray.at(maxXPosIndex).position;
		elementShapeGuideVertexArray.push_back(previousAddPos);
		elementShapeVertexArray.push_back(elementSpherePointArray.at(maxXPosIndex));
	}

	if (maxYPosIndex != minXPosIndex && maxYPosIndex != minYPosIndex && maxYPosIndex != maxXPosIndex)
	{
		addElementShapeVertexArray(
			&elementShapeGuideVertexArray,
			&elementShapeVertexArray,
			&notMaxOrMinIndexList,
			previousAddPos,
			elementSpherePointArray.at(maxYPosIndex).position,
			elementSpherePointArray
		);

		previousAddPos = elementSpherePointArray.at(maxYPosIndex).position;
		elementShapeGuideVertexArray.push_back(previousAddPos);
		elementShapeVertexArray.push_back(elementSpherePointArray.at(maxYPosIndex));
	}

	addElementShapeVertexArray(
		&elementShapeGuideVertexArray,
		&elementShapeVertexArray,
		&notMaxOrMinIndexList,
		previousAddPos,
		elementSpherePointArray.at(minXPosIndex).position,
		elementSpherePointArray
	);

	elementShapeGuideVertexArray.push_back(elementSpherePointArray.at(minXPosIndex).position);

	m_elementShapeGuide->getComponent<tktk::Line2DDrawer>()->setLineVertexArray(elementShapeGuideVertexArray);

	if (tktk::Keyboard::isTrigger(tktk::KEYBORD_SPACE) && elementShapeVertexArray.size() >= 3U)
	{
		ElementShape::create(elementShapeVertexArray);
	}
}

void ElementSphereManagerScript::createElementSphere(const Vector3 & position, const Color & color)
{
	for (unsigned int i = 0; i < m_useMatArray.size(); i++)
	{
		if (!m_useMatArray.at(i))
		{
			m_elementSphereObjectList.push_front(ElementSphere::create(position, color, static_cast<MaterialId>(m_firstElementMaterialId + i)));
			m_useMatArray.at(i) = true;
			return;
		}
	}

	throw std::runtime_error("nou found can use ElementMaterialId");
}

void ElementSphereManagerScript::addElementShapeVertexArray(
	std::vector<Vector2>* elementShapeGuideVertexArrayPtr,
	std::vector<VertexColor2DPolygonVertexData>* elementShapeVertexArrayPtr,
	std::forward_list<unsigned int>* notMaxOrMinIndexListPtr,
	const Vector2 & previousPos,
	const Vector2 & nextPos,
	const std::vector<VertexColor2DPolygonVertexData>& elementSpherePointArray
)
{
	// 「firstPos->nextPos」のベクトル
	Vector2 line = nextPos - previousPos;

	// firstPosとnextPosの間に入る資格のある頂点のインデックスのリスト
	std::forward_list<unsigned int> insertVertexIndexList;

	// 作成する図形の頂点になっていない頂点のインデックスを巡回
	for (auto node : *notMaxOrMinIndexListPtr)
	{
		// 「firstPos->巡回中の頂点」のベクトル
		Vector2 firstToPoint = elementSpherePointArray.at(node).position - previousPos;

		// firstPosとnextPosの間に入る資格のある頂点なら「firstPosとnextPosの間に入る資格のある頂点のリスト」に追加する
		if (Vector2::cross(firstToPoint, line) > 0.0f) insertVertexIndexList.push_front(node);
		// ↑反転
	}

	// 「firstPosとnextPosの間に入る資格のある頂点」が存在したら
	if (std::begin(insertVertexIndexList) != std::end(insertVertexIndexList))
	{
		// 最も「firstPos->nextPos」の線分から遠い頂点のインデックス
		unsigned int mostFarVertexIndex = 0U;

		// 最も遠い「firstPos->nextPos」から「mostFarVertexIndex」の頂点の距離
		float mostFarDist = std::numeric_limits<float>::min();

		// firstPosとnextPosの間に入る資格のある頂点のインデックスを巡回
		for (auto node : insertVertexIndexList)
		{
			float tempDist = 0.0f;

			Vector2 chackVertex = elementSpherePointArray.at(node).position;

			float a = nextPos.x - previousPos.x;
			float b = nextPos.y - previousPos.y;

			float a2 = a * a;
			float b2 = b * b;

			// 線分の長さの２乗の値？
			float r2 = a2 + b2;

			float tt = -(a * (previousPos.x - chackVertex.x) + b * (previousPos.y - chackVertex.y));

			if (tt < 0)
			{
				tempDist = std::sqrtf((previousPos.x - chackVertex.x) * (previousPos.x - chackVertex.x) + (previousPos.y - chackVertex.y) * (previousPos.y - chackVertex.y));
			}
			else if (tt > r2)
			{
				tempDist = std::sqrtf((nextPos.x - chackVertex.x) * (nextPos.x - chackVertex.x) + (nextPos.y - chackVertex.y) * (nextPos.y - chackVertex.y));
			}
			else
			{
				float f1 = a * (previousPos.y - chackVertex.y) - b * (previousPos.x - chackVertex.x);
				tempDist = std::sqrtf((f1 * f1) / r2);
			}

			if (mostFarDist < tempDist)
			{
				mostFarVertexIndex = node;
				mostFarDist = tempDist;
			}
		}

		// 「firstPos->nextPos」の線分から最も遠い座標を次の頂点として配列に追加する
		elementShapeGuideVertexArrayPtr->push_back(elementSpherePointArray.at(mostFarVertexIndex).position);
		elementShapeVertexArrayPtr->push_back(elementSpherePointArray.at(mostFarVertexIndex));

		// 追加した頂点のインデックスを「notMaxOrMinIndexList」から削除する
		notMaxOrMinIndexListPtr->remove(mostFarVertexIndex);

		// 新しい頂点で再起する
		addElementShapeVertexArray(
			elementShapeGuideVertexArrayPtr,
			elementShapeVertexArrayPtr,
			notMaxOrMinIndexListPtr,
			elementSpherePointArray.at(mostFarVertexIndex).position,
			nextPos,
			elementSpherePointArray
		);
	}
}
