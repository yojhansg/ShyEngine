#include <memory>
#include <iostream>
#include "Engine.h"

//#include "Scripting/Script.h"

int main(int argc, char* args[]) {



	//ECS::Script script;

	//script.Initialise("prueba.json");

	//std::cout << "=== Init ===" << std::endl;
	//script.start();
	//std::cout << "=== Udate ===" << std::endl;

	//return 0;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;

	engine.init();

	engine.update();

	engine.close();

	std::cout << "This was My Engine!\n";

	return 0;
}
