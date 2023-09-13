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

		void Initialise(cstring path);

		void start() override;
		void update(float dt) override;
		void onCollisionEnter(Entity* entity) override;
		void onCollisionStay(Entity* entity) override;
		void onCollisionExit(Entity* entity) override;

		void onTriggerEnter(Entity* entity) override;
		void onTriggerStay(Entity* entity) override;
		void onTriggerExit(Entity* entity) override;

		void onClick() override;
		void onClickBegin() override;
		void onClickHold() override;
		void onDoubleClick() override;
		void onRightClick() override;

		void Event(cstring name);

		std::string GetName();

		Scripting::Variable Get(cstring name);
		void Set(cstring name, cVariable variable);

		Scripting::Variable GetSerialisedValue(cstring name);
		void SetSerialisedValue(cstring name, cVariable val);

	private:

		std::string name;

		Scripting::ScriptManager::ScriptNodes nodes;

		std::map<std::string, Scripting::Variable> serializedValues;
		std::map<std::string, Scripting::Variable> attributes;

		void Iteration(Scripting::Node* beginNode);
	};
}