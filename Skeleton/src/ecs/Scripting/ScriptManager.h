#pragma once

#include <Singleton.h>
#include <map>
#include <vector>
#include <string>
#include <Scripting/Variable.h>

typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);

class FunctionManager;


//TODO: Limpiar basura

namespace Scripting {

	class Node;
	class Fork;
	class Function;


	class ScriptManager : public Utilities::Singleton<ScriptManager> {

		friend Singleton<ScriptManager>;

		const std::string extension = ".json";

	public:

		struct ScriptNodes {

			ScriptNodes();

			Node* start;
			Node* update;

			Node* onCollision;
			Node* onCollisionEnter;
			Node* onCollisionExit;

			Node* onClick;
			Node* onClickBegin;
			Node* onClickHold;
			Node* onDoubleClick;
			Node* onRightClick;
		};


		static ScriptNodes LoadScript(std::string const& path);

		static Variable CallFunction(std::string const& func, std::vector<Variable> const& params);

		~ScriptManager();


		void SetGlobal(std::string const& name, Scripting::Variable const& val);
		Scripting::Variable GetGlobal(std::string const& name);


	private:

		ScriptManager();

		std::vector<Node*> allNodes;
		std::map<std::string, ScriptNodes> scripts;

		std::map<std::string, CallableFunction> functionMap;

		std::map<std::string, Scripting::Variable> globalAttributes;

		struct NodeInfo {
			Fork* fork;
			int A, B, condition;

			int next;
			Function* function;
			std::vector<int> input;
		};

	};

}