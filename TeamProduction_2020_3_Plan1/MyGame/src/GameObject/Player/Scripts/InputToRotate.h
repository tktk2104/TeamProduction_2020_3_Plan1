#pragma once

#include <TktkComponentFramework/Component/ComponentBase.h>

class InputToRotate
	: public tktk::ComponentBase
{
public:

	InputToRotate() = default;

public:

	void update();
};