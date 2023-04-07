#include "Node.h"
#include "ScriptManager.h"

Scripting::Node::Node(int idx) : nodeIdx(idx), iterationIdx(-1)
{

}

Scripting::Node::~Node()
{
}

void Scripting::Node::ForceReevaluation()
{
}

void Scripting::Node::Cicle(int iterationIdx)
{
	this->iterationIdx = iterationIdx;

	Node* next = nullptr;
	Operate(next, iterationIdx);

	if (next)
		next->Cicle(iterationIdx);

}
