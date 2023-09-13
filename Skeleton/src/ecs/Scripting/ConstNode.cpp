#include "ConstNode.h"
#include "ScriptManager.h"
#include "Script.h"
#include "ReferencesManager.h"
#include "Entity.h"
#include "Scene.h"
#include "SceneManager.h"

Scripting::ConstNode::ConstNode(int idx, Scripting::Variable val) : Scripting::OutputNode(idx)
{
	originalValue = output = val;
}

void Scripting::ConstNode::Operate(Node*& next, int& iterationIdx)
{
	next = nullptr;

	if (name.size() == 0) return;

	ECS::Script* script = ScriptManager::instance()->GetCurrentScript();

	output = script->GetSerialisedValue(name);


	auto currentScene = ECS::SceneManager::instance()->getCurrentScene();

	if (originalValue.type == Variable::Type::Entity)
	{
		//TODO: hacer algo con el id

		int id = output.value.entityId;
		if (currentScene->GetReferencesManager()->IsEntityValid(id)) {

			output.value.entity = currentScene->GetReferencesManager()->GetEntity(id);
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
