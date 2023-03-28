#pragma once


#include <map>
#include <string>
#include <Component.h>
#include <Scripting/Variable.h>
#include <Scripting/Node.h>
#include "Scripting/ScriptManager.h"

namespace ECS {


	//Los scripts comparten nodos
	class Script : public Component
	{

	public:

		static Script* currentScript;

		void Initialise(std::string path);


		void start() override;
		void update(float dt) override;
		void onCollisionEnter(Entity* entity) override;

	private:

		Scripting::ScriptManager::ScriptNodes nodes;

		std::map<std::string, Scripting::Variable> attributes;

		void Iteration(Scripting::Node* beginNode);
	};
}