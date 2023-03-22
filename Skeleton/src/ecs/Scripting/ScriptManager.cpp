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


Scripting::ScriptManager::ScriptManager()
{
	FunctionManager::CreateFunctionMap(functionMap);
}

Scripting::ScriptManager::~ScriptManager()
{

}



Scripting::ScriptManager::ScriptNodes Scripting::ScriptManager::LoadScript(std::string const& path)
{
	std::ifstream fileStream("Scripts/" + path);
	json file = json::parse(path);

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
			if (node.contains("next")) {

				int next = node["next"].get<int>();
				info.next = next;
			}

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

			forks.push_back({nodeIdx, A, B});

			Fork* forkNode = new Fork(nodeIdx, (Fork::ForkType) forkType);
			allScriptNodes[nodeIdx] = forkNode;

			NodeInfo info = { forkNode, A, B };
			forkConnections.push_back(info);
		}
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

	//TODO: mover esto un poco mas arriba
	for (auto& info : functionInfo) {

		info.function->SetNextNode(allScriptNodes[info.next]);

	}

	//for (auto& next : nextNodes) {

	//	//static_cast<Function*>(allScriptNodes[next[0]])->SetNextNode(allScriptNodes[next[1]]);
	//}

	//for (auto& fork : forkConnections) {

	//	//static_cast<Fork*>(allScriptNodes[fork[0]])->SetFork(allScriptNodes[fork[1]], allScriptNodes[fork[2]]);
	//}


	for (Node* node : allScriptNodes) {

		allNodes.push_back(node);
	}

	fileStream.close();
	scripts[path] = scriptNodeInfo;
	return scriptNodeInfo;
}
