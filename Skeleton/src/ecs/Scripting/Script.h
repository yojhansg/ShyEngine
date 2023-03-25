#pragma once


#include <map>
#include <string>
#include <Component.h>
#include <Scripting/Variable.h>
#include <Scripting/Node.h>

namespace ECS {


	//Los scripts comparten nodos
	class Script : public Component
	{

	public:

		static Script* currentScript;

		void Initialise(std::string path);


		void start() override;
		void update(float dt) override;
	private:

		Scripting::Node* startNode;
		Scripting::Node* updateNode;

		std::map<std::string, Scripting::Variable> attributes;

		void Iteration(Scripting::Node* beginNode);
	};
}