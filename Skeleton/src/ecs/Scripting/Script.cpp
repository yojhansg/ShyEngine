#include "Script.h"
#include "ScriptManager.h"

#include "CheckML.h"

void ECS::Script::start()
{
	Iteration(nodes.start);
}

void ECS::Script::update(float dt)
{
	Iteration(nodes.update);
}

void ECS::Script::onCollisionEnter(Entity* entity)
{
	//TODO: ver una forma de saber con que objeto he colisionado
	Iteration(nodes.onCollisionEnter);
}

void ECS::Script::onCollisionStay(Entity* entity)
{
	Iteration(nodes.onCollisionStay);
}

void ECS::Script::onCollisionExit(Entity* entity)
{
	Iteration(nodes.onCollisionExit);
}

void ECS::Script::onTriggerEnter(Entity* entity)
{
 	Iteration(nodes.onTriggerEnter);
}

void ECS::Script::onTriggerStay(Entity* entity)
{
	Iteration(nodes.onTriggerStay);
}

void ECS::Script::onTriggerExit(Entity* entity)
{
	Iteration(nodes.onTriggerExit);
}

void ECS::Script::onClick()
{
	Iteration(nodes.onClick);
}

void ECS::Script::onClickBegin()
{
	Iteration(nodes.onClickBegin);
}

void ECS::Script::onClickHold()
{
	Iteration(nodes.onClickHold);
}

void ECS::Script::onDoubleClick()
{
	Iteration(nodes.onDoubleClick);
}

void ECS::Script::onRightClick()
{
	Iteration(nodes.onRightClick);
}

void ECS::Script::Event(cstring name)
{
	if (nodes.names.contains(name)) {

		Iteration(nodes.names[name]);
	}
}

std::string ECS::Script::GetName()
{
	return name;
}

Scripting::Variable ECS::Script::Get(cstring name)
{
	if (!attributes.contains(name))
	{
		printWarning("Trying to access non existing attribute. Returning Empty", "Script");
		return Scripting::Variable::Null();
	}
	return attributes[name];
}

void ECS::Script::Set(cstring name, cVariable variable)
{
	attributes[name] = variable;
}

Scripting::Variable ECS::Script::GetSerialisedValue(cstring name)
{
	if (serializedValues.contains(name))
		return serializedValues[name];

	return Scripting::Variable::Null();
}

void ECS::Script::SeterialisedValue(cstring name, cVariable val)
{
	serializedValues[name] = val;
}

void ECS::Script::Initialise(cstring path)
{
	name = path;

	//TODO: Guardar un script en lugar de leerlo cada vez
	nodes = Scripting::ScriptManager::LoadScript(path);
}

void ECS::Script::Iteration(Scripting::Node* beginNode)
{
	if (beginNode == nullptr)
		return;

	Scripting::ScriptManager::instance()->NewIteration(this, beginNode);
}



