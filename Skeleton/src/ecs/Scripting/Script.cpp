#include "Script.h"
#include "ScriptManager.h"

ECS::Script* ECS::Script::currentScript = nullptr;

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

std::string ECS::Script::GetName()
{
	return name;
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

	currentScript = this;
	beginNode->Cicle();
	currentScript = nullptr;
}



