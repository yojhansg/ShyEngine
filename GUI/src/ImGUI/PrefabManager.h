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

		bool open;

		Editor* editor;
		std::unordered_map<int, GameObject*> prefabs;

		//Key = prefab Id, Value = all its instances
		std::unordered_map<int, std::vector<int>> prefabInstances;

		Texture* prefabText;

		void DrawList();
		void DrawPrefab(GameObject* prefab);
		void DrawImage();
		void DrawComponents();

		bool shouldUpdate;

		int currentlySelected;

		void LoadPrefabs();
		void HandleDeletion();
		void UpdatePrefabInstances();

		GameObject* IdIsInOverlays(int id);

	public:

		static std::vector<int> unusedIds;

		PrefabManager();
		~PrefabManager();

		static void Open();

		static void AddPrefab(GameObject* go);
		static void AddInstance(GameObject* prefab, GameObject* prefabInstance);
		static void AddInstance(int prefabId, int prefabInstanceId);
		static void RemoveInstance(int prefabId, int prefabInstanceId);

		static void SavePrefabs(const std::string& path);

		static GameObject* GetPrefabById(int id);

		void Behaviour() override;
	};
}
