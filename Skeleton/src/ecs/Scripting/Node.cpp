#include "Node.h"

Scripting::Node::Node(int idx) : nodeIdx(idx)
{
}

Scripting::Node::~Node()
{
}

void Scripting::Node::Cicle()
{
	Scripting::Node* current = this;

	Node* next = nullptr;
	current->Operate(next);

	if (next)
		next->Cicle();
}
