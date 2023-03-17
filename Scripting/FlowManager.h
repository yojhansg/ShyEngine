#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace CFlat {
	class IBox;
}

class Script;

using std::string;

namespace CFlat {

	class FlowManager
	{
	public:
		struct ScriptInfo {

			CFlat::IBox* init;
			CFlat::IBox* update;
		};

		static FlowManager* instance;

	private:


		const string fileExtension = ".cf";
		const string path = "./";

		std::vector<CFlat::IBox*> boxes;
		std::unordered_map <string, ScriptInfo* > scripts;
	public:

		FlowManager();

		FlowManager::ScriptInfo* getBoxes(std::string file);
		FlowManager::ScriptInfo* loadScript(string file, Script* script);
	};

}