#include "ElementSphereManagerScript.h"

#include <stdexcept>
#include <limits>
#include <cmath>
#include <TktkMath/Random.h>
#include <TktkDirectX11Wrapper/Input/Keyboard/Keyboard.h>
#include <TktkDirectX11Wrapper/Graphics/Window/Window.h>
#include <TktkDirectX11Wrapper/Graphics/Camera/Camera.h>
#include <TktkAppend3DComponent/Transform3D.h>
#include "../../ElementSphere/ElementSphere.h"
#include "../../ElementShape/ElementShape.h"
#include "../../../Ids/CameraId.h"

constexpr unsigned int ElementSphereManagerScript::m_firstElementMaterialId;
constexpr unsigned int ElementSphereManagerScript::m_lastElementMaterialId;
constexpr unsigned int ElementSphereManagerScript::m_useMatArraySize;

void ElementSphereManagerScript::start()
{
	for (unsigned int i = 0; i < m_useMatArraySize; i++)
	{
		createElementSphere(
			Vector3(
				Random::getRandF(-3.0f, 3.0f),
				Random::getRandF(-3.0f, 3.0f),
				Random::getRandF(-3.0f, 3.0f)
			),
			Color::green
		);
	}
}

void ElementSphereManagerScript::update()
{
	m_elementSphereObjectList.remove_if(
		[](GameObjectPtr& node) { return node.isNull(); }
	);

	if (tktk::Keyboard::isTrigger(tktk::KEYBORD_SPACE))
	{
		// �g�p���Ă���J�����̏��
		auto cameraDataPtr = tktk::Camera::getDataPtr(CameraId::PlayerCamera);

		// �X�N���[����ԏ�̓_�̔z��
		std::vector<Vector2> elementSpherePointArray;

		// X���W���ő�A�ŏ��̍��W�̃C���f�b�N�X
		unsigned int maxXPosIndex = 0U;
		unsigned int minXPosIndex = 0U;

		// Y���W���ő�A�ŏ��̍��W�̃C���f�b�N�X
		unsigned int maxYPosIndex = 0U;
		unsigned int minYPosIndex = 0U;

		// ���݂̃C���f�b�N�X
		unsigned int curIndex = 0U;

		for (const auto& node : m_elementSphereObjectList)
		{
			auto transform = node->getComponent<tktk::Transform3D>();
			const auto& screenSize = tktk::Window::getWindowSize();

			// �X�N���[�����W�����߂�
			Vector3 result = transform->getWorldPosition() * (*cameraDataPtr->getViewMatrixPtr()) * (*cameraDataPtr->getProjectionMatrixPtr());
			result /= result.z;
			result = result * Matrix4::createScreen(0.0f, 0.0f, screenSize.x, screenSize.y);

			// ���������_����ʊO�ɏo�Ă����ꍇ�A�������Ȃ�
			if (result.x < 0.0f || result.x > screenSize.x || result.y < 0.0f || result.y > screenSize.y) continue;

			// �X�N���[����ԏ�̓_�̔z���ǉ�����
			elementSpherePointArray.push_back(Vector2(result.x, result.y));

			// ���W���ꂼ��̐����̍ő�A�ŏ��̍��W�̃C���f�b�N�X���X�V����
			if (elementSpherePointArray.at(maxXPosIndex).x < result.x) maxXPosIndex = curIndex;
			if (elementSpherePointArray.at(minXPosIndex).x > result.x) minXPosIndex = curIndex;
			if (elementSpherePointArray.at(maxYPosIndex).y < result.y) maxYPosIndex = curIndex;
			if (elementSpherePointArray.at(minYPosIndex).y > result.y) minYPosIndex = curIndex;

			curIndex++;
		}

		// �ő�A�ŏ��ł͂Ȃ����_�̃C���f�b�N�X�̃��X�g
		std::forward_list<unsigned int> notMaxOrMinIndexList;

		// ���݂̒��_��������̐����̍ő�A�ŏ��l�Ŗ�����΁u�ő�A�ŏ��ł͂Ȃ����_�̃C���f�b�N�X�̃��X�g�v�ɒǉ�����
		for (unsigned int i = 0; i < elementSpherePointArray.size(); i++)
		{
			if (maxXPosIndex != i || minXPosIndex != i || maxYPosIndex != i || minYPosIndex != i)
			{
				notMaxOrMinIndexList.push_front(i);
			}
		}


		// ���_���P�����������牽�����Ȃ�
		if (curIndex == 0U) return;
		

		// �쐬����}�`�̒��_�̔z��
		std::vector<Vector2> elementShapeVertexArray;

		// ���O�ɒǉ��������_�����ϐ�
		Vector2 previousAddPos = elementSpherePointArray.at(minXPosIndex);

		// �u�����W���ŏ��̍��W�v���ŏ��̒��_�Ƃ��Ĕz��ɒǉ�����
		elementShapeVertexArray.push_back(previousAddPos);

		// �u�����W���ŏ��̍��W�v���u�����W���ŏ��̍��W�v�Ɠ����łȂ����
		if (minYPosIndex != minXPosIndex)
		{
			addElementShapeVertexArray(
				&elementShapeVertexArray,
				&notMaxOrMinIndexList,
				previousAddPos,
				elementSpherePointArray.at(minYPosIndex),
				elementSpherePointArray
			);
//******************************
			//
			//// �O�ɒǉ��������_
			//Vector2 firstPos = elementSpherePointArray.at(minXPosIndex);
			//
			//// ���ɒǉ����钸�_
			//Vector2 nextPos = elementSpherePointArray.at(minYPosIndex);
			//
			//// �ufirstPos->nextPos�v�̃x�N�g��
			//Vector2 line = nextPos - firstPos;
			//
			//// firstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�̃C���f�b�N�X�̃��X�g
			//std::forward_list<unsigned int> insertVertexIndexList;
			//
			//// �쐬����}�`�̒��_�ɂȂ��Ă��Ȃ����_�̃C���f�b�N�X������
			//for (auto node : notMaxOrMinIndexList)
			//{
			//	// �ufirstPos->���񒆂̒��_�v�̃x�N�g��
			//	Vector2 firstToPoint = elementSpherePointArray.at(node) - firstPos;
			//
			//	// firstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�Ȃ�ufirstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�̃��X�g�v�ɒǉ�����
			//	if (Vector2::cross(firstToPoint, line) < 0.0f) insertVertexIndexList.push_front(node);
			//}
			//
			//// �ufirstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�v�����݂�����
			//if (std::begin(insertVertexIndexList) != std::end(insertVertexIndexList))
			//{
			//	// �ł��ufirstPos->nextPos�v�̐������牓�����_�̃C���f�b�N�X
			//	unsigned int mostFarVertexIndex = 0U;
			//
			//	// �ł������ufirstPos->nextPos�v����umostFarVertexIndex�v�̒��_�̋���
			//	float mostFarDist = std::numeric_limits<float>::min();
			//
			//	// firstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�̃C���f�b�N�X������
			//	for (auto node : insertVertexIndexList)
			//	{
			//		float tempDist = 0.0f;
			//
			//		Vector2 chackVertex = elementSpherePointArray.at(node);
			//
			//		float a = nextPos.x - firstPos.x;
			//		float b = nextPos.y - firstPos.y;
			//
			//		float a2 = a * a;
			//		float b2 = b * b;
			//
			//		// �����̒����̂Q��̒l�H
			//		float r2 = a2 + b2;
			//
			//		float tt = -(a * (firstPos.x - chackVertex.x) + b * (firstPos.y - chackVertex.y));
			//
			//		if (tt < 0)
			//		{
			//			tempDist = std::sqrtf((firstPos.x - chackVertex.x) * (firstPos.x - chackVertex.x) + (firstPos.y - chackVertex.y) * (firstPos.y - chackVertex.y));
			//		}
			//		else if (tt > r2)
			//		{
			//			tempDist = std::sqrtf((nextPos.x - chackVertex.x) * (nextPos.x - chackVertex.x) + (nextPos.y - chackVertex.y) * (nextPos.y - chackVertex.y));
			//		}
			//		else
			//		{
			//			float f1 = a * (firstPos.y - chackVertex.y) - b * (firstPos.x - chackVertex.x);
			//			tempDist = std::sqrtf((f1 * f1) / r2);
			//		}
			//
			//		if (mostFarDist < tempDist)
			//		{
			//			mostFarVertexIndex = node;
			//			mostFarDist = tempDist;
			//		}
			//	}
			//
			//	// �ufirstPos->nextPos�v�̐�������ł��������W�����̒��_�Ƃ��Ĕz��ɒǉ�����
			//	elementShapeVertexArray.push_back(elementSpherePointArray.at(mostFarVertexIndex));
			//
			//	// �ǉ��������_�̃C���f�b�N�X���unotMaxOrMinIndexList�v����폜����
			//	notMaxOrMinIndexList.remove(mostFarVertexIndex);
			//
			//
			//}
//******************************
			// �u�����W���ŏ��̍��W�v�����̒��_�Ƃ��Ĕz��ɒǉ�����

			// �u�����W���ŏ��̍��W�v���ŏ��̒��_�Ƃ��Ĕz��ɒǉ�����
			previousAddPos = elementSpherePointArray.at(minYPosIndex);
			elementShapeVertexArray.push_back(previousAddPos);
		}

		if (maxXPosIndex != minXPosIndex && maxXPosIndex != minYPosIndex)
		{
			addElementShapeVertexArray(
				&elementShapeVertexArray,
				&notMaxOrMinIndexList,
				previousAddPos,
				elementSpherePointArray.at(maxXPosIndex),
				elementSpherePointArray
			);

			previousAddPos = elementSpherePointArray.at(maxXPosIndex);
			elementShapeVertexArray.push_back(previousAddPos);
		}

		if (maxYPosIndex != minXPosIndex && maxYPosIndex != minYPosIndex && maxYPosIndex != maxXPosIndex)
		{
			addElementShapeVertexArray(
				&elementShapeVertexArray,
				&notMaxOrMinIndexList,
				previousAddPos,
				elementSpherePointArray.at(maxYPosIndex),
				elementSpherePointArray
			);

			previousAddPos = elementSpherePointArray.at(maxYPosIndex);
			elementShapeVertexArray.push_back(previousAddPos);
		}

		addElementShapeVertexArray(
			&elementShapeVertexArray,
			&notMaxOrMinIndexList,
			previousAddPos,
			elementSpherePointArray.at(minXPosIndex),
			elementSpherePointArray
		);

		elementShapeVertexArray.push_back(elementSpherePointArray.at(minXPosIndex));

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
	std::vector<Vector2>* elementShapeVertexArrayPtr,
	std::forward_list<unsigned int>* notMaxOrMinIndexListPtr,
	const Vector2 & previousPos,
	const Vector2 & nextPos,
	const std::vector<Vector2>& elementSpherePointArray
)
{
	// �ufirstPos->nextPos�v�̃x�N�g��
	Vector2 line = nextPos - previousPos;

	// firstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�̃C���f�b�N�X�̃��X�g
	std::forward_list<unsigned int> insertVertexIndexList;

	// �쐬����}�`�̒��_�ɂȂ��Ă��Ȃ����_�̃C���f�b�N�X������
	for (auto node : *notMaxOrMinIndexListPtr)
	{
		// �ufirstPos->���񒆂̒��_�v�̃x�N�g��
		Vector2 firstToPoint = elementSpherePointArray.at(node) - previousPos;

		// firstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�Ȃ�ufirstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�̃��X�g�v�ɒǉ�����
		if (Vector2::cross(firstToPoint, line) > 0.0f) insertVertexIndexList.push_front(node);
		// �����]
	}

	// �ufirstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�v�����݂�����
	if (std::begin(insertVertexIndexList) != std::end(insertVertexIndexList))
	{
		// �ł��ufirstPos->nextPos�v�̐������牓�����_�̃C���f�b�N�X
		unsigned int mostFarVertexIndex = 0U;

		// �ł������ufirstPos->nextPos�v����umostFarVertexIndex�v�̒��_�̋���
		float mostFarDist = std::numeric_limits<float>::min();

		// firstPos��nextPos�̊Ԃɓ��鎑�i�̂��钸�_�̃C���f�b�N�X������
		for (auto node : insertVertexIndexList)
		{
			float tempDist = 0.0f;

			Vector2 chackVertex = elementSpherePointArray.at(node);

			float a = nextPos.x - previousPos.x;
			float b = nextPos.y - previousPos.y;

			float a2 = a * a;
			float b2 = b * b;

			// �����̒����̂Q��̒l�H
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

		// �ufirstPos->nextPos�v�̐�������ł��������W�����̒��_�Ƃ��Ĕz��ɒǉ�����
		elementShapeVertexArrayPtr->push_back(elementSpherePointArray.at(mostFarVertexIndex));

		// �ǉ��������_�̃C���f�b�N�X���unotMaxOrMinIndexList�v����폜����
		notMaxOrMinIndexListPtr->remove(mostFarVertexIndex);

		// �V�������_�ōċN����
		addElementShapeVertexArray(
			elementShapeVertexArrayPtr,
			notMaxOrMinIndexListPtr,
			elementSpherePointArray.at(mostFarVertexIndex),
			nextPos,
			elementSpherePointArray
		);
	}
}
