#include "Script.h"
#include "ScriptManager.h"


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
	Iteration(nodes.onCollision);
}

void ECS::Script::onCollisionExit(Entity* entity)
{
	Iteration(nodes.onCollisionExit);
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

std::string ECS::Script::GetName()
{
	return name;
}

Scripting::Variable ECS::Script::Get(std::string const& name)
{
	if (!attributes.contains(name))
	{
		printWarning("Trying to access non existing attribute. Returning Empty", "Script");
		return Scripting::Variable::Null();
	}
	return attributes[name];
}

void ECS::Script::Set(std::string const& name, Scripting::Variable variable)
{
	attributes[name] = variable;
}

void ECS::Script::Initialise(std::string path)
{
	name = path;
	nodes = Scripting::ScriptManager::LoadScript(path);
}

void ECS::Script::Iteration(Scripting::Node* beginNode)
{
	if (beginNode == nullptr)
		return;

	Scripting::ScriptManager::instance()->NewIteration(this, beginNode);
}



