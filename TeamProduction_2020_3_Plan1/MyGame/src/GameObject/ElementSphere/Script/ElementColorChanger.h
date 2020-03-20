#pragma once

#include <TktkMath/Color.h>
#include <TktkComponentFramework/Component/ComponentBase.h>
#include "../../../Ids/MaterialId.h"

class ElementColorChanger
	: public tktk::ComponentBase
{
public:

	ElementColorChanger(
		const Color& color,
		MaterialId id
	);

public:

	void start();

	void update();

public:

	const Color& getColor() const;

private:

	Color m_color;
	MaterialId m_id;
};