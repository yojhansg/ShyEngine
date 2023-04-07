#pragma once


#include <map>
#include <string>
#include <Component.h>
#include <Scripting/Variable.h>
#include <Scripting/Node.h>
#include "Scripting/ScriptManager.h"

namespace ECS {


	class Script : public Component
	{

	public:


		void Initialise(std::string path);


		void start() override;
		void update(float dt) override;
		void onCollisionEnter(Entity* entity) override;
		void onCollisionStay(Entity* entity) override;
		void onCollisionExit(Entity* entity) override;

		void onClick() override;
		void onClickBegin() override;
		void onClickHold() override;
		void onDoubleClick() override;
		void onRightClick() override;

		std::string GetName();

		Scripting::Variable Get(std::string const& name);
		void Set(std::string const& name, Scripting::Variable variable);

	private:

		std::string name;

		Scripting::ScriptManager::ScriptNodes nodes;

		std::map<std::string, Scripting::Variable> attributes;

		void Iteration(Scripting::Node* beginNode);
	};
}