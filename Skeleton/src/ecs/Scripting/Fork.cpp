#include "Fork.h"
#include "OutputNode.h"

Scripting::Fork::Fork(int idx, ForkType type) : Scripting::Node(idx), type(type)
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


//TODO: no se comprueba si tiene condition

void Scripting::Fork::Operate(Node*& next, int iterationIdx)
{
	condition->Cicle(iterationIdx);

	switch (type)
	{
	case Scripting::Fork::ForkType::While:

		while (condition->output.value.Bool) {

			A->Cicle(iterationIdx);


			condition->Cicle(iterationIdx);
		}
		next = B;

		break;
	case Scripting::Fork::ForkType::For:


		for (int i = 0; i < condition->output.value.Float; i++) {

			A->Cicle(iterationIdx);
		}
		next = B;

		break;
	default:

		if (condition->output.value.Bool)
			next = A;
		else next = B;

		break;
	}
}

void Scripting::Fork::ForceReevaluation()
{
	if (condition)
		condition->ForceReevaluation();
	if (A)
		A->ForceReevaluation();
	if (B)
		B->ForceReevaluation();
}

