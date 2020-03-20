#include "ElementColorChanger.h"

#include <TktkDirectX11Wrapper/Graphics/Mesh/ConstantBufferData/MonoColorConstantBufferData.h>
#include <TktkDirectX11Wrapper/Graphics/Material/Material.h>
#include <TktkDirectX11Wrapper/Graphics/ConstantBuffer/Asset/SystemConstantBufferId.h>

ElementColorChanger::ElementColorChanger(const Color & color, MaterialId id)
	: m_color(color)
	, m_id(id)
{
}

void ElementColorChanger::start()
{
	tktk::Material::getDataPtr(m_id)->getParametersRef().setConstantBufferSetData(
		tktk::SystemConstantBufferId::MonoColor,
		tktk::MonoColorConstantBufferData({ m_color })
	);
}

void ElementColorChanger::update()
{
	
}

const Color & ElementColorChanger::getColor() const
{
	return m_color;
}