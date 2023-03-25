#include "Script.h"
#include "ScriptManager.h"

ECS::Script* ECS::Script::currentScript = nullptr;

void ECS::Script::start()
{
	Iteration(startNode);
}

void ECS::Script::update(float dt)
{
	Iteration(updateNode);
}

void ECS::Script::Initialise(std::string path)
{
	auto scriptInfo = Scripting::ScriptManager::LoadScript(path);

	startNode = scriptInfo.start;
	updateNode = scriptInfo.update;
}

void ECS::Script::Iteration(Scripting::Node* beginNode)
{
	if (beginNode == nullptr)
		return;

	currentScript = this;
	beginNode->Cicle();
	currentScript = nullptr;
}



