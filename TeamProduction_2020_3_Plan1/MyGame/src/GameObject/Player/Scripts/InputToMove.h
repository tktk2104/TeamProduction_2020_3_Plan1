#pragma once

#include <TktkComponentFramework/Component/ComponentBase.h>

class InputToMove
	: public tktk::ComponentBase
{
public:

	InputToMove() = default;

public:

	void update();
};