#include "ConstNode.h"
#include "ScriptManager.h"
#include "Script.h"
#include "ReferencesManager.h"

Scripting::ConstNode::ConstNode(int idx, Scripting::Variable val) : Scripting::OutputNode(idx)
{
	originalValue = output = val;
}

void Scripting::ConstNode::Operate(Node*& next, int iterationIdx)
{
	next = nullptr;

	if (name.size() == 0) return;

	ECS::Script* script = ScriptManager::instance()->GetCurrentScript();

	output = script->GetSerialisedValue(name);



	if (originalValue.type == Variable::Type::Entity)
	{
		//TODO: hacer algo con el id

		int id = output.value.entityId;
		if (ECS::ReferencesManager::instance()->IsEntityValid(id)) {


			output.value.entity = ECS::ReferencesManager::instance()->GetEntity(id);
		}
		else {
			output.value.entity = nullptr;
		}
	}

	if (output.type == Scripting::Variable::Type::Null)
		output = originalValue;
}

void Scripting::ConstNode::SetName(cstring name)
{
	this->name = name;
}
