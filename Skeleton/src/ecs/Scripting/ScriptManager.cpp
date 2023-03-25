#include "ScriptManager.h"
#include "ECSUtilities/FunctionManager.h"
#include "json.hpp"
#include <fstream>
#include "Node.h"
#include "Function.h"
#include "ConstNode.h"
#include "Fork.h"

using namespace nlohmann;

using jsonarray = std::vector<json>;

#include <iostream>

Scripting::ScriptManager::ScriptManager()
{
	FunctionManager::CreateFunctionMap(functionMap);
}

Scripting::ScriptManager::~ScriptManager()
{

}



Scripting::ScriptManager::ScriptNodes Scripting::ScriptManager::LoadScript(std::string const& path)
{
	ScriptManager* manager = instance();

	if (manager->scripts.contains(path)) {

		return manager->scripts[path];
	}

	std::ifstream fileStream("Scripts/" + path);

	if (!fileStream.good())
	{
		std::cout << "Error leyendo el archivo" << std::endl;
		return ScriptNodes();
	}
	json file = json::parse(fileStream);


	int nodeCount = file["nodeCount"].get<int>();
	std::vector<Node*> allScriptNodes(nodeCount);

	std::vector<NodeInfo> functionInfo;
	std::vector<NodeInfo> forkConnections;

	if (file.contains("functions")) {

		jsonarray functions = file["functions"].get<jsonarray>();

		for (auto& node : functions) {

			int nodeIdx = node["index"].get<int>();
			int operation = node["operator"].get<int>();

			std::string methodCall = "";
			if (operation == 0) {
				methodCall = node["methodCall"].get<std::string>();
			}

			NodeInfo info;
			int next = -1;
			if (node.contains("next")) {

				next = node["next"].get<int>();
			}
			info.next = next;

			if (node.contains("input")) {

				info.input = node["input"].get<std::vector<int>>();
			}

			Function* function = new Function(nodeIdx, operation, methodCall);
			allScriptNodes[nodeIdx] = function;

			info.function = function;
			functionInfo.push_back(info);
		}
	}

	if (file.contains("constValues")) {

		jsonarray constValues = file["constValues"].get<jsonarray>();

		for (auto& constValue : constValues) {

			int nodeIdx = constValue["index"].get<int>();

			Variable value;

			std::string type = constValue["type"].get<std::string>();

			if (type == "float") {

				float v = constValue["float"].get<float>();
				value = v;
			}
			else if (type == "int") {

				int v = constValue["int"].get<int>();
				value = v;
			}

			ConstNode* constValueNode = new ConstNode(nodeIdx, value);
			allScriptNodes[nodeIdx] = constValueNode;
		}
	}


	if (file.contains("forks")) {

		jsonarray forks = file["forks"].get<jsonarray>();

		for (auto& forkNode : forks) {

			int nodeIdx = forkNode["index"].get<int>();
			int forkType = forkNode["type"].get<int>();

			int A = forkNode["A"].get<int>();
			int B = forkNode["B"].get<int>();

			int condition = forkNode["condition"].get<int>();

			forks.push_back({ nodeIdx, A, B });

			Fork* forkNode = new Fork(nodeIdx, (Fork::ForkType)forkType);
			allScriptNodes[nodeIdx] = forkNode;

			NodeInfo info;
			info.fork = forkNode;
			info.A = A;
			info.B = B;
			info.condition = condition;
			forkConnections.push_back(info);
		}
	}

	for (auto& info : functionInfo) {

		if (info.next >= 0)
			info.function->SetNextNode(allScriptNodes[info.next]);

		std::vector<OutputNode*> input;

		for (int i : info.input) {
			OutputNode* out = static_cast<OutputNode*>(allScriptNodes[i]);
			input.push_back(out);
		}

		info.function->SetInput(input);
	}

	for (auto& fork : forkConnections) {

		fork.fork->SetFork(allScriptNodes[fork.A], allScriptNodes[fork.B]);
		fork.fork->SetCondition(static_cast<OutputNode*>(allScriptNodes[fork.condition]));
		//static_cast<Fork*>(allScriptNodes[fork[0]])->SetFork(allScriptNodes[fork[1]], allScriptNodes[fork[2]]);
	}


	for (Node* node : allScriptNodes) {

		manager->allNodes.push_back(node);
	}


	ScriptNodes scriptNodeInfo = { nullptr, nullptr };

	if (file.contains("start")) {
		int startIdx = file["start"].get<int>();
		Node* startNode = allScriptNodes[startIdx];
		scriptNodeInfo.start = startNode;
	}

	if (file.contains("update")) {
		int updateIdx = file["update"].get<int>();
		Node* updateNode = allScriptNodes[updateIdx];
		scriptNodeInfo.update = updateNode;
	}



	fileStream.close();
	manager->scripts[path] = scriptNodeInfo;
	return scriptNodeInfo;
}

Scripting::Variable Scripting::ScriptManager::CallFunction(std::string const& func, std::vector<Scripting::Variable> const& vec)
{
	ScriptManager* manager = instance();

	return manager->functionMap[func](vec);
}
