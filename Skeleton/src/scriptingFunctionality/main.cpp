
#include <iostream>
#include <string>
#include "Method2Function.h"

int main() {

	Method2Function script(
		Method2Function::GetDefaultRoot()
	);

	script.AskForRoot();
	script.AskForOutput();
	script.CreateOutputFolder();
	script.Begin();
	script.CreateFunctionManagerHeader();
	script.CreateFunctionManagerContent();

	return 0;
}