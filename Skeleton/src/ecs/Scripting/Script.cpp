#include "Script.h"


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
	//TODO: initialise script
}

void ECS::Script::Iteration(Scripting::Node* beginNode)
{
	beginNode->Cicle();
}



