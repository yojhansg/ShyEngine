
#include <iostream>
#include <string>
#include "Method2Function.h"

int main() {
	

//publish:
//	void Move(Vector3 newPosition);
//	Vector3 GetPosition();
//}
	auto method = Method2Function::Method();

	method.returnType = "void";
	method.className = "Transform";
	method.methodName = "Move";
	method.input = { {"newPosition", "Vector3"}};

	std::cout << method.FunctionDeclaration() << std::endl << std::endl;
	std::cout << method.FunctionDefinition() << std::endl;

	method.returnType = "Vector3";
	method.className = "Transform";
	method.methodName = "GetPosition";
	method.input = { };

	std::cout << method.FunctionDeclaration() << std::endl << std::endl;
	std::cout << method.FunctionDefinition() << std::endl;


	return 0;

	Method2Function script(Method2Function::GetDefaultRoot(), Method2Function::GetDefaultOutput());

	script.AskForRoot();
	script.AskForOutput();
	script.CreateOutputFolder();
	script.Begin();

	return 0;
}