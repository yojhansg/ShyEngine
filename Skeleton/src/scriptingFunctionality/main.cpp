#include <string>
#include "ECSReader.h"
#include "ClassCreator.h"


#include <iostream>
int main() {

	ECSReader reader(
		ECSReader::GetDefaultRoot()
	);

	reader.AskForRoot();
	reader.AskForOutput();
	reader.Read();

	reader.Method2Function();
	reader.ClassReflection();
	reader.ComponentFactory();
	reader.Convert2JSON();

	return 0;
}