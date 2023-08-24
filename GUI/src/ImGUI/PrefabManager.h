#pragma once

#include "Window.h"

#include <unordered_map>
#include <vector>

class Editor;

namespace ShyEditor {

	class Texture;
	class GameObject;

	class PrefabManager: public Window {

	private:

		static PrefabManager* instance;
		static int lastPrefabId;

		static std::vector<int> unusedIds;

		bool open;

		Editor* editor;
		std::unordered_map<int, GameObject*> prefabs;

		Texture* prefabText;

		void DrawList();

		void DrawComponents();

		bool shouldUpdate;

		int currentlySelected;

		void LoadPrefabs();

	public:

		PrefabManager();
		~PrefabManager();

		static void Open();

		static void AddPrefab(GameObject* go);

		static void SavePrefabs(const std::string& path);

		void Behaviour() override;
	};
}
