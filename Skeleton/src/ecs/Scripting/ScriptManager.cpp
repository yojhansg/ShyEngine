#include "ScriptManager.h"
#include "ECSUtilities/FunctionManager.h"
#include <fstream>
#include "Node.h"
#include "Function.h"
#include "ConstNode.h"
#include "Fork.h"
#include "Script.h"
#include <ConsoleManager.h>

#include "json.hpp"
#include "ResourcesManager.h"

using namespace nlohmann;

using jsonarray = std::vector<json>;


Scripting::ScriptManager::ScriptManager()
{
	nodeIteration = 0;
	currentScript = nullptr;
	FunctionManager::CreateFunctionMap(functionMap);
}

Scripting::ScriptManager::~ScriptManager()
{
	for (auto node : allNodes) {
		delete node;
	}
	allNodes.clear();
}

void Scripting::ScriptManager::SetGlobal(std::string const& name, Scripting::Variable const& val)
{
	globalAttributes[name] = val;
}

Scripting::Variable Scripting::ScriptManager::GetGlobal(std::string const& name)
{

	if (!globalAttributes.contains(name)) {

		Console::Output::PrintError(instance()->currentScript->GetName() + ": Global attributes", 
			"Trying to access unexisting global variable <" + name + ">. Returing empty instead");

		return Scripting::Variable::Null();
	}

	return globalAttributes[name];
}

void Scripting::ScriptManager::CollectionCreate(cstring name, int size)
{
	if (collections.contains(name)) {
		Console::Output::PrintError(instance()->currentScript->GetName() + ": Collections",
			"Trying to create an already existing collection <" + name + ">. Destroying previous collection");

		CollectionDestroy(name);
	}

	collections[name] = std::vector<Scripting::Variable>(size);
}

void Scripting::ScriptManager::CollectionDestroy(cstring name)
{
	if (!collections.contains(name)) {
		Console::Output::PrintError(instance()->currentScript->GetName() + ": Collections",
			"Trying to destroy an inexisting collection <" + name + ">");
		return;
	}

	collections.erase(name);
}

void Scripting::ScriptManager::CollectionModify(cstring name, int idx, cVariable val)
{
	if (!collections.contains(name)) {
		Console::Output::PrintError(instance()->currentScript->GetName() + ": Collections",
			"Trying to destroy an inexisting collection <" + name + ">");
		return;
	}

	auto& vec = collections[name];
	int size = vec.size();

	if(idx < 0 || idx >= size) {
		Console::Output::PrintError(instance()->currentScript->GetName() + ": Collections",
			"Index <" + std::to_string(idx) + "> is unaccesible. Size of the collection: " + std::to_string(size));
		return;
	}

	collections[name][idx] = val;
}

int Scripting::ScriptManager::CollectionSize(cstring name)
{
	if (!collections.contains(name)) {
		Console::Output::PrintError(instance()->currentScript->GetName() + ": Collections",
			"Trying to destroy an inexisting collection <" + name + ">");
		return -1;
	}

	return collections[name].size();
}

Scripting::Variable Scripting::ScriptManager::CollectionPeek(cstring name, int idx)
{
	if (!collections.contains(name)) {
		Console::Output::PrintError(instance()->currentScript->GetName() + ": Collections",
			"Trying to destroy an inexisting collection <" + name + ">");
		return Scripting::Variable::Null();
	}

	auto& vec = collections[name];
	int size = vec.size();

	if (idx < 0 || idx >= size) {
		Console::Output::PrintError(instance()->currentScript->GetName() + ": Collections",
			"Index <" + std::to_string(idx) + "> is unaccesible. Size of the collection: " + std::to_string(size));

		return Scripting::Variable::Null();;
	}

	return vec[idx];
}


Scripting::ScriptManager::ScriptNodes Scripting::ScriptManager::LoadScript(std::string const& path)
{
	ScriptManager* manager = instance();

	if (manager->scripts.contains(path)) {

		return manager->scripts[path];
	}

	std::ifstream fileStream("Scripts/" + path + manager->extension);

	if (!fileStream.good())
	{
		Console::Output::PrintError("Script initialisation", "Cannot open file <" + path + ">");
		return ScriptNodes();
	}

	if (!json::accept(fileStream)) {

		Console::Output::PrintError("Script initialisation", "Trying to read an invalid script <" + path + ">");
		return ScriptNodes();
	}

	fileStream.clear();
	fileStream.seekg(0);

	json file = json::parse(fileStream);

	int nodeCount = file["nodeCount"].get<int>();
	std::vector<Node*> allScriptNodes(nodeCount);

	std::vector<NodeInfo> functionInfo;
	std::vector<NodeInfo> forkConnections;

	if (file.contains("functions")) {

		jsonarray functions = file["functions"].get<jsonarray>();

		for (auto& node : functions) {

			int nodeIdx = node["index"].get<int>();

			std::string methodCall = node["function"].get<std::string>();

			NodeInfo info;
			int next = -1;
			if (node.contains("next")) {

				next = node["next"].get<int>();
			}
			info.next = next;

			if (node.contains("input")) {

				info.input = node["input"].get<std::vector<int>>();
			}

			if (allScriptNodes[nodeIdx] != nullptr) {

				Console::Output::PrintError(path + ": Node index", "The script already contains a node with idx <" + std::to_string(nodeIdx) + ">");
				continue;
			}

			Function* function = new Function(nodeIdx, methodCall);
			allScriptNodes[nodeIdx] = function;

			info.function = function;
			functionInfo.push_back(info);
		}
	}

	if (file.contains("consts")) {

		jsonarray consts = file["consts"].get<jsonarray>();

		for (auto& constValue : consts) {

			int nodeIdx = constValue["index"].get<int>();

			Variable value;

			std::string type = constValue["type"].get<std::string>();

			if (type == "float") {

				float v = constValue["value"].get<float>();
				value = v;
			}
			else if (type == "bool") {

				bool b = constValue["value"].get<bool>();
				value = b;
			}
			else if (type == "char") {

				char c = constValue["value"].get<char>();
				value = c;
			}
			else if (type == "Vector2D") {
				std::string vec = constValue["value"].get<std::string>();
				value = (Utilities::Vector2D)vec;
			}
			else if (type == "string") {
				std::string vec = constValue["value"].get<std::string>();
				value = vec;
			}
			else if (type == "color") {
				Utilities::Color col = Utilities::Color::CreateColor(constValue["value"].get<std::string>());
				value = col;
			}
			else if (type == "Entity") {

				value.type = Variable::Type::Entity;
				value.value.entityId = constValue["value"].get<int>();
			}

			if (allScriptNodes[nodeIdx] != nullptr) {

				Console::Output::PrintError(path + ": Node index", "The script already contains a node with idx <" + std::to_string(nodeIdx) + ">");
				continue;
			}

			ConstNode* constValueNode = new ConstNode(nodeIdx, value);
			allScriptNodes[nodeIdx] = constValueNode;

			if (constValue.contains("name")) {

				constValueNode->SetName(constValue["name"].get<std::string>());
			}

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

			if (allScriptNodes[nodeIdx] != nullptr) {

				Console::Output::PrintError(path + ": Node index", "The script already contains a node with idx <" + std::to_string(nodeIdx) + ">");
				continue;
			}

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

		Node* A = fork.A >= 0 ? allScriptNodes[fork.A] : nullptr;
		Node* B = fork.B >= 0 ? allScriptNodes[fork.B] : nullptr;

		fork.fork->SetFork(A, B);
		fork.fork->SetCondition(static_cast<OutputNode*>(allScriptNodes[fork.condition]));
	}


	for (Node* node : allScriptNodes) {

		manager->allNodes.push_back(node);
	}


	ScriptNodes scriptNodeInfo;

#define SetScriptNodeValue(x) \
	if(file.contains(#x)){\
	int nodeIdx = file[#x].get<int>();\
	Node* node = allScriptNodes[nodeIdx];\
	scriptNodeInfo.x = node;}

	SetScriptNodeValue(start);
	SetScriptNodeValue(update);
	SetScriptNodeValue(onCollisionEnter);
	SetScriptNodeValue(onCollisionStay);
	SetScriptNodeValue(onCollisionExit);
	SetScriptNodeValue(onTriggerEnter);
	SetScriptNodeValue(onTriggerStay);
	SetScriptNodeValue(onTriggerExit);
	SetScriptNodeValue(onClick);
	SetScriptNodeValue(onClickBegin);
	SetScriptNodeValue(onClickHold);
	SetScriptNodeValue(onDoubleClick);
	SetScriptNodeValue(onRightClick);

#undef SetScriptNodeValue

	if (file.contains("names")) {

		for (auto& name : file["names"].items()) {

			Node* node = allScriptNodes[name.value().get<int>()];

			if (node != nullptr)
				scriptNodeInfo.names[name.key()] = node;
		}
	}

	fileStream.close();
	manager->scripts[path] = scriptNodeInfo;
	return scriptNodeInfo;
}

Scripting::Variable Scripting::ScriptManager::CallFunction(std::string const& func, std::vector<Scripting::Variable> const& vec)
{
	ScriptManager* manager = instance();

	if (!manager->functionMap.contains(func))
	{

		Console::Output::PrintError(instance()->currentScript->GetName() + ": Node error", "The node <" + func + "> does not exist");
		return Scripting::Variable::Null();
	}

	return manager->functionMap[func](vec);
}

Scripting::ScriptManager::ScriptNodes::ScriptNodes()
{
	start = update = nullptr;

	onCollisionEnter = onCollisionStay = onCollisionExit = nullptr;

	onClick = onClickBegin = onClickHold = onDoubleClick = onRightClick = nullptr;

	onTriggerStay = onTriggerEnter = onTriggerExit = nullptr;
}

void Scripting::ScriptManager::NewIteration(ECS::Script* script, Node* beginNode)
{
	currentScript = script;
	beginNode->Cicle(nodeIteration++);
	currentScript = nullptr;
}

ECS::Script* Scripting::ScriptManager::GetCurrentScript()
{
	return currentScript;
}


