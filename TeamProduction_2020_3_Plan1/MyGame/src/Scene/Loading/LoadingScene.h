#pragma once

#include <TktkComponentFramework/Scene/SceneBase.h>

class LoadingScene
	: public tktk::SceneBase
{
public:

	void sceneStart() override;

	void sceneEnd() override;
};