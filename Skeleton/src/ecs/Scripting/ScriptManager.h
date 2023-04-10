#pragma once

#include <Singleton.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <Scripting/Variable.h>

typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);

class FunctionManager;


namespace ECS {
	class Script;
}

namespace Scripting {

	class Node;
	class Fork;
	class Function;

	class ScriptManager : public Utilities::Singleton<ScriptManager> {

		friend Singleton<ScriptManager>;

		friend class Engine;

		const std::string extension = ".json";


	public:

		struct ScriptNodes {

			ScriptNodes();

			Node* start;
			Node* update;

			Node* onCollisionStay;
			Node* onCollisionEnter;
			Node* onCollisionExit;

			Node* onTriggerStay;
			Node* onTriggerEnter;
			Node* onTriggerExit;

			Node* onClick;
			Node* onClickBegin;
			Node* onClickHold;
			Node* onDoubleClick;
			Node* onRightClick;

			std::unordered_map<std::string, Node*> names;
		};


		static ScriptNodes LoadScript(std::string const& path);

		static Variable CallFunction(std::string const& func, std::vector<Variable> const& params);

		void SetGlobal(std::string const& name, Scripting::Variable const& val);
		Scripting::Variable GetGlobal(std::string const& name);

		void NewIteration(ECS::Script* script, Node* beginNode);

		ECS::Script* GetCurrentScript();

		~ScriptManager();
	private:

		ScriptManager();

		int nodeIteration;
		ECS::Script* currentScript;

		std::vector<Node*> allNodes;
		std::unordered_map<std::string, ScriptNodes> scripts;

		std::unordered_map<std::string, CallableFunction> functionMap;

		std::unordered_map<std::string, Scripting::Variable> globalAttributes;

		struct NodeInfo {
			Fork* fork;
			int A, B, condition;

			int next;
			Function* function;
			std::vector<int> input;
		};

	};

}