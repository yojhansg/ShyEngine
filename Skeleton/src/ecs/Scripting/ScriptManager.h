#pragma once

#include <Singleton.h>
#include <map>
#include <vector>
#include <string>
#include <Scripting/Variable.h>

typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);

class FunctionManager;

namespace Scripting {

	class Node;
	class Fork;
	class Function;


	class ScriptManager : public Utilities::Singleton<ScriptManager> {

		friend Singleton<ScriptManager>;

	public:

		struct ScriptNodes {

			Node* start;
			Node* update;
			Node* onCollisionEnter;
		};


		static ScriptNodes LoadScript(std::string const& path);

		static Variable CallFunction(std::string const& func, std::vector<Variable> const& params);

		~ScriptManager();

	private:

		ScriptManager();

		std::vector<Node*> allNodes;
		std::map<std::string, ScriptNodes> scripts;

		std::map<std::string, CallableFunction> functionMap;


		struct NodeInfo {
			Fork* fork;
			int A, B, condition;

			int next;
			Function* function;
			std::vector<int> input;
		};

	};

}