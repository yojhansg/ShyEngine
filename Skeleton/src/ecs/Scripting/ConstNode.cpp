#include "ConstNode.h"

Scripting::ConstNode::ConstNode(int idx, Scripting::Variable val): Scripting::OutputNode(idx), value(val)
{
	output = value;
}

void Scripting::ConstNode::Operate(Node*& next)
{
	next = nullptr;
}
