#include <ImGUIManager.h>
#include "imgui_files/imgui.h"
#include "Window.h"
#include "Scene.h"
#include "GameObject.h"
#include "MenuBar.h"
#include "Hierarchy.h"
#include "FileExplorer.h"
#include "Components.h"

int main(int, char**)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	imGUIManager->loop();

	delete imGUIManager;
}
