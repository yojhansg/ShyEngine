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

void Scripting::Fork::Operate(Node*& next, int& iterationIdx)
{
	condition->Cycle(iterationIdx);

	switch (type)
	{
	case Scripting::Fork::ForkType::While:

		if (A != nullptr) {

			while (condition->output.value.Bool) {


				A->Cycle(++iterationIdx);


				condition->Cycle(++iterationIdx);
			}
		}
		next = B;

		break;
	case Scripting::Fork::ForkType::For:

		if (A != nullptr) {
			for (int i = 0; i < condition->output.value.Float; i++) {

				A->Cycle(++iterationIdx);
			}
		}
		next = B;

		break;
	default:


		//If

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

