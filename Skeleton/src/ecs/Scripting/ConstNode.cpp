#include "ConstNode.h"
#include "ScriptManager.h"
#include "Script.h"

Scripting::ConstNode::ConstNode(int idx, Scripting::Variable val): Scripting::OutputNode(idx), value(val)
{
	originalValue = output = value;
}

void Scripting::ConstNode::Operate(Node*& next, int iterationIdx)
{
	next = nullptr;

	if (name.size() == 0) return;

	ECS::Script* script = ScriptManager::instance()->GetCurrentScript();

	output = script->GetSerialisedValue(name);

	if (output.type == Scripting::Variable::Type::Null)
		output = originalValue;
}

void Scripting::ConstNode::SetName(cstring name)
{
	this->name = name;
}
