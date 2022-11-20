#include <ImGUIManager.h>
#include "imgui_files/imgui.h"
#include "Window.h"
#include "Scene.h"
#include "Image.h"
#include "MenuBar.h"


int main(int, char**)
{

	ImGUIManager* imGUIManager = ImGUIManager::getInstance();

	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	PEditor::MenuBar* menuBar = new PEditor::MenuBar();
	imGUIManager->addWindow(menuBar);


	PEditor::Window* sceneWindow = new PEditor::Window("Scene", NoMove | NoResize | NoCollapse);
	//1095 x 750 instead of 1080 x 720 to leave a little offset between the window and the scene image
	ImVec2 sceneSize = ImVec2(1095, 755);
	sceneWindow->setSize(sceneSize);
	sceneWindow->setPosition(ImVec2(mainWindowSize.x / 2 - sceneSize.x / 2, 20));

	Scene* scene = new Scene(sceneWindow);
	scene->addImage("test1.jpg");

	imGUIManager->addWindow(sceneWindow);

	imGUIManager->loop();
}
