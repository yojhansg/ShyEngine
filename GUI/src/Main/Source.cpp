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

	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	//MENU BAR
	PEditor::MenuBar* menuBar = new PEditor::MenuBar();
	imGUIManager->addWindow(menuBar);

	//SCENE
	PEditor::Scene* scene = new PEditor::Scene();
	imGUIManager->addWindow(scene);

	//HIERARCHY
	PEditor::Hierarchy* hierarchy = new PEditor::Hierarchy();
	imGUIManager->addWindow(hierarchy);

	//FILE EXPLORER
	PEditor::FileExplorer* fileExplorer = new PEditor::FileExplorer();
	imGUIManager->addWindow(fileExplorer);

	//COMPONENTS
	PEditor::Components* components = new PEditor::Components();
	imGUIManager->addWindow(components);

	imGUIManager->loop();
}
