#include <ImGUIManager.h>
#include "ImGUIWindow.h"

int main(int, char**)
{
	ImGUIWindow* window = new ImGUIWindow("VENTANA TEST", NoResize | NoMove);

	ImGUIManager* imGUIManager = new ImGUIManager();

	imGUIManager->init();

	imGUIManager->addWindow(window);

	imGUIManager->loop();
}
