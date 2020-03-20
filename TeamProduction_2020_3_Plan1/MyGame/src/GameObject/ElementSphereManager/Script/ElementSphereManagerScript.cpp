#include "ElementSphereManagerScript.h"

#include <stdexcept>
#include <TktkMath/Random.h>
#include <TktkDirectX11Wrapper/Input/Keyboard/Keyboard.h>
#include "../../ElementSphere/ElementSphere.h"

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

	if (tktk::Keyboard::isPush(tktk::KEYBORD_SPACE))
	{

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
