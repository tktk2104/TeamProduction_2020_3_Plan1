#pragma once

#include <array>
#include <forward_list>
#include <TktkMath/Color.h>
#include <TktkMath/Vector3.h>
#include <TktkComponentFramework/Component/ComponentBase.h>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>
#include "../../../Ids/MaterialId.h"

constexpr int countId(MaterialId first, MaterialId last)
{
	return static_cast<int>(last) - static_cast<int>(first) + 1;
}

class ElementSphereManagerScript
	: public tktk::ComponentBase
{
public:

	ElementSphereManagerScript() = default;

public:

	void start();

	void update();

public:

	void createElementSphere(const Vector3& position, const Color& color);

private:

	static constexpr unsigned int m_firstElementMaterialId{ static_cast<unsigned int>(MaterialId::Sphere_1) };

	static constexpr unsigned int m_lastElementMaterialId{ static_cast<unsigned int>(MaterialId::Sphere_10) };

	static constexpr unsigned int m_useMatArraySize{ m_lastElementMaterialId - m_firstElementMaterialId + 1U };

private:

	std::array<bool, m_useMatArraySize> m_useMatArray;

	std::forward_list<GameObjectPtr> m_elementSphereObjectList;
};