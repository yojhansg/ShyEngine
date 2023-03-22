#include "Fork.h"
#include "OutputNode.h"

Scripting::Fork::Fork(int idx, ForkType type): Scripting::Node(idx), type(type)
{
	A = B = condition = nullptr;
}

void Scripting::Fork::SetFork(Node* A, Node* B)
{
	this->A = A;
	this->B = B;
}

void Scripting::Fork::SetCondition(OutputNode* node)
{
	condition = node;
}

void Scripting::Fork::Operate(Node*& next)
{
	//Default -> if

	//TODO: resto de forks

	if (condition->output.value.Bool)
		next = A;
	else next = B;
}
