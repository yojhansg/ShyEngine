#include <Editor.h>

#include "CheckML.h"

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

	Editor* editor = Editor::getInstance();

	//editor->Probando();
	//editor->Example();

	if (!editor->Init()) {
		editor->Close();
		return -1;
	}

	editor->Loop();

	editor->Close();

	return 0;

}

