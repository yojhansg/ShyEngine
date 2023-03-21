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
		};


		ScriptNodes LoadScript(std::string const& path);

		~ScriptManager();

	private:

		ScriptManager();

		std::vector<Node*> allNodes;
		std::map<std::string, ScriptNodes> scripts;

		std::map<std::string, CallableFunction> functionMap;


		struct NodeInfo {
			Fork* fork;
			int A, B;

			int next;
			Function* function;
			std::vector<int> input;
		};

	};

}