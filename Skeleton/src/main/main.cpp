#include <iostream>
#include <memory>
#include <string>
#include <format>

#include "Engine.h"
#include "Save.h"
#include "ConsoleManager.h"

#include "Scripting/ScriptFunctionality.h"

void saveTimesPlayed() {

	ECS::SaveManager* save = ECS::SaveManager::instance();

	int slot = 1;

	if (!save->Exists(1)) {

		save->SetSlot("contador", Scripting::Variable::Float(0));
	}

	else {

		save->Load(1);
		save->SetSlot("contador", Scripting::Variable(save->GetSlot("contador").value.Float + 1));
	}

	Console::Output::PrintNoFormat(std::format("Has abierto el juego {} veces", save->GetSlot("contador").value.Float));

	save->Save(1);
}


/*
* This is a way to avoid opening the debug console in release mode.
*/

#ifdef _DEBUG

	int main(int argc, char* args[]) {

#else

	#include <Windows.h>

	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, int nCmdShow) {

#endif

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// saveTimesPlayed();

		Engine engine;

		bool ret = engine.init();

		if (!ret) {
			return -1;
		}

		engine.update();

		std::cout << "]::Before closing!\n";

		engine.close();

		std::cout << "]::This was My Engine!" << std::endl;

		return 0;
	}
