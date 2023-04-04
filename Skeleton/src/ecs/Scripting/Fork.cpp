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
	condition->Cicle();

	switch (type)
	{
	case Scripting::Fork::ForkType::While:

		while (condition->output.value.Bool) {

			A->Cicle();
			condition->Cicle();
		}
		next = B;

		break;
	case Scripting::Fork::ForkType::For:
		break;
	default:


		if (condition->output.value.Bool)
 			next = A;
		else next = B;

		break;
	}
}
