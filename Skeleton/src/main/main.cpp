#include <memory>
#include <iostream>
#include "Engine.h"


//#define DEBUG_SCRIPT


#ifdef DEBUG_SCRIPT
#include "Scripting/Script.h"
#endif // DEBUG_SCRIPT
#include "Scripting/ScriptFunctionality.h"

int main(int argc, char* args[]) {

	std::cout << Scripting::ScriptFunctionality::instance()->String_Begining("0123456789", 2) << std::endl;

	return 0;
#ifdef DEBUG_SCRIPT

	ECS::Script script;

	script.Initialise("prueba.json");

	std::cout << "=== Init ===" << std::endl;
	script.start();
	std::cout << "=== Udate ===" << std::endl;

	return 0;*/

#endif // DEBUG_SCRIPT



	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;

	engine.init();

	engine.update();

	engine.close();

	std::cout << "This was My Engine!\n";

	return 0;
}
