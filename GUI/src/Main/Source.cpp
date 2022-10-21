#include <ImGUIManager.h>
#include "imgui_files/imgui.h"
#include "Window.h"

int main(int, char**)
{

	ImGUIManager* imGUIManager = new ImGUIManager();

	imGUIManager->init();

	/*PEditor::Window* window = new PEditor::Window("VENTANA TEST1", None);
	window->setSize(ImVec2(50, 50));

	imGUIManager->addWindow(window);*/

	PEditor::Window* window = new PEditor::Window("VENTANA TEST2", NoMove);
	window->setSize(ImVec2(1080, 720));

	imGUIManager->addWindow(window);

	/*window = new PEditor::Window("VENTANA TEST3", None);
	window->setSize(ImVec2(50, 50));

	imGUIManager->addWindow(window);*/


	imGUIManager->loop();
}
