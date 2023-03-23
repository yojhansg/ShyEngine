#include <memory>
#include <iostream>
#include "Engine.h"

int main(int argc, char* args[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;

	engine.init();

	engine.update();

	engine.close();

	std::cout << "This was My Engine!\n";

	return 0;
}
