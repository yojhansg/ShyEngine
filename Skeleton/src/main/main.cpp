#include <memory>
#include "Engine.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

int main(int argc, char* args[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;

	engine.init();

	engine.update();

	engine.close();
	
	return 0;
}