#pragma once


#include "Singleton.h"
#include <map>
#include <string>

#include "EditorExport.h"
#include "Scripting/Variable.h"

namespace ECS {

	//Como funcionan los saves
	//TODO: autosave
	EditorManager SaveManager : public Utilities::Singleton<SaveManager> {

	public:

		const std::string path = "Saves/sav";
		const std::string extension = ".json";

		SaveManager();
		~SaveManager();

	publish:

		void SaveAll();

		void Save(int slot);
		void Load(int slot);

		void SetSlot(std::string name, Scripting::Variable value);
		Scripting::Variable GetSlot(std::string name);

		void Set(std::string name, Scripting::Variable value);
		Scripting::Variable Get(std::string name);

		void ClearSlot();
		bool Exists(int slot);
		void DeleteSave(int slot);

	private:

		std::string GetSlotPath();
		std::string GetSlotPath(int slot);

		int currentLoaded;
		std::map<std::string, Scripting::Variable> loaded;
		std::map<std::string, Scripting::Variable> implicit;
	};

}