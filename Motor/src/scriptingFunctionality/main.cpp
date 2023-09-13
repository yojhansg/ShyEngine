#include <string>
#include "ECSReader.h"
#include "ClassCreator.h"


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
	//reader.GenerateFunctionJSON();
	//reader.GenerateAttributeJSON();
	reader.GenerateComponentsJSON();
	reader.GenerateManagersJSON();
	return 0;
}