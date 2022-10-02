#include <ImGUIManager.h>

int main(int, char**)
{
	ImGUIManager* imGUIManager = new ImGUIManager();

	imGUIManager->init();
	imGUIManager->loop();
}
