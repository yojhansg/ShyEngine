#include <ImGUIManager.h>
#include "imgui_files/imgui.h"
#include "Window.h"
#include "Scene.h"
#include "Image.h"
#include "MenuBar.h"


int main(int, char**)
{

	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	//MENU BAR
	PEditor::MenuBar* menuBar = new PEditor::MenuBar();
	imGUIManager->addWindow(menuBar);



	//SCENE
	Scene* scene = new Scene();


	imGUIManager->loop();
}
