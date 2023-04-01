#include "Function.h"
#include "ScriptManager.h"

Scripting::Function::Function(int idx, std::string const& method): OutputNode(idx), methodName(method), next(nullptr)
{

}

void Scripting::Function::Operate(Node*& nextNode)
{
	nextNode = next;

	for (auto& in : input) {
		in->Cicle();
	}

	std::vector<Scripting::Variable> inputVariable;

	for (auto in : input) {

		inputVariable.push_back(in->output);
	}

	output = ScriptManager::CallFunction(methodName, inputVariable);

	//Default -> suma
	//TODO: resto de operaciones
	//TODO: script manager tiene que tener el mapa generado por function manager

	//TODO: ser un puto loco, hacer que no haya indice de operacion y que todo vaya por metodos

	//output = input[0]->output + input[1]->output;

	//std::cout << "output: " << output.value.Int << std::endl;
}

void Scripting::Function::SetNextNode(Scripting::Node* nextNode)
{
	this->next = nextNode;
}

void Scripting::Function::SetInput(std::vector<OutputNode*> input)
{
	this->input = input;
}
