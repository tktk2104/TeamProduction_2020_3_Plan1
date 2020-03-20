#pragma once

#include <vector>
#include <TktkComponentFramework/Component/ComponentBase.h>
#include <TktkComponentFramework/GameObject/GameObjectPtr.h>

class InputToRotate
	: public tktk::ComponentBase
{
public:

	InputToRotate() = default;

public:

	void start();

	void update();

private:

	std::vector<GameObjectPtr> m_debugTextObjArray;
};