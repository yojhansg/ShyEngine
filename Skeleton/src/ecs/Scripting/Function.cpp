#include "Function.h"
#include "ScriptManager.h"

Scripting::Function::Function(int idx, std::string const& method) : OutputNode(idx), methodName(method), next(nullptr)
{

}

void Scripting::Function::Operate(Node*& nextNode, int iterationIdx)
{
	nextNode = next;

	for (auto& in : input) {

		if (in->iterationIdx != iterationIdx)
			in->Cicle(iterationIdx);
	}

	std::vector<Scripting::Variable> inputVariable;

	for (auto in : input) {

		inputVariable.push_back(in->output);
	}

	output = ScriptManager::CallFunction(methodName, inputVariable);
}


void Scripting::Function::SetNextNode(Scripting::Node* nextNode)
{
	this->next = nextNode;
}

void Scripting::Function::SetInput(std::vector<OutputNode*> input)
{
	this->input = input;
}
