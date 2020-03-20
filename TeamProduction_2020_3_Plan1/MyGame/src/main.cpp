#include <TktkDirectX11Wrapper/DirectX11LibDefinition.h>

#include <TktkComponentFramework/ComponentFrameworkProcessor.h>
#include <TktkComponentFramework/Scene/SceneManager.h>
#include <TktkDirectX11Wrapper/Graphics/Screen/Screen.h>
#include <TktkDirectX11Wrapper/Utility/WindownsHeaderIncluder.h>
#include <TktkDirectX11Wrapper/_Policy/DirectX11Policy.h>
#include "Scene/Loading/LoadingScene.h"
#include "Scene/Title/TitleScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR pCmdLine, int nCmdShow)
{
	tktk::ComponentFrameworkProcessor::createClass<tktk::DirectX11Policy>(
		true,
		hInstance,
		nCmdShow,
		"TestWindow",
		Vector2(1920.0f, 1080.0f),
		false
		);

	tktk::SceneManager::createScene<LoadingScene>();
	tktk::SceneManager::createScene<TitleScene>();

	tktk::SceneManager::enableScene<LoadingScene>();

	tktk::Screen::setBackgroundColor(Color(0.5f, 0.5f, 0.5f, 1.0f));

	tktk::ComponentFrameworkProcessor::run();
}