#include <memory>
#include <iostream>
#include "Engine.h"

#include "Scripting/ScriptManager.h"
#include "Scripting/Function.h"




int main(int argc, char* args[]) {

	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Scripting::ScriptManager::LoadScript("Parallax.json");
	//return 0;


	Engine engine;

	engine.init();

	engine.update();

	engine.close();

	std::cout << "This was My Engine!\n";

	return 0;
}
