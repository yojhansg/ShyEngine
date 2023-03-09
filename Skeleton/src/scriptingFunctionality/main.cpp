
#include <iostream>
#include <string>
#include "GenerateScriptMethod.h"
#include <filesystem>

int main() {
	
	GenerateScriptMethod script(GenerateScriptMethod::GetDefaultRoot(), GenerateScriptMethod::GetDefaultOutput());

	script.AskForRoot();
	script.AskForOutput();
	script.CreateOutputFolder();
	script.Begin();

	return 0;
}