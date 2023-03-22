#include "ConstNode.h"

Scripting::ConstNode::ConstNode(int idx, Scripting::Variable val): Scripting::OutputNode(idx), value(val)
{
}

void Scripting::ConstNode::Operate(Node*& next)
{
	next = nullptr;


	output = value;
}
