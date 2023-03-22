#include "Function.h"
#include <iostream>

Scripting::Function::Function(int idx, int operation, std::string const& method): OutputNode(idx), operation(operation), methodName(method), next(nullptr)
{

}

void Scripting::Function::Operate(Node*& nextNode)
{
	nextNode = next;

	for (auto& in : input) {
		in->Cicle();
	}

	//Default -> suma
	//TODO: resto de operaciones
	//TODO: script manager tiene que tener el mapa generado por function manager




	output = input[0]->output + input[1]->output;

	std::cout << "output: " << output.value.Int << std::endl;
}

void Scripting::Function::SetNextNode(Scripting::Node* nextNode)
{
	this->next = nextNode;
}

void Scripting::Function::SetInput(std::vector<OutputNode*> input)
{
	this->input = input;
}
