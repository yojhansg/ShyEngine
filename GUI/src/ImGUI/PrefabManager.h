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
		static int lastPrefabId; // Starts at -1 and goes down

		static void AssignId(GameObject* prefab);

		Editor* editor;
		Texture* prefabText;

		bool open;
		int currentlySelected; // Currently selected prefab

		//Key = prefab Id, Value = all its instances in the scene
		std::unordered_map<int, std::vector<int>> prefabInstances;

		std::unordered_map<int, GameObject*> prefabs;

		void DrawPrefabList();
		void DrawPrefab(GameObject* prefab);
		void DrawImage();
		void DrawComponents();

		//Loading prefabs from prefabs json
		void LoadPrefabs();

		void HandlePrefabDeletion();
		void UpdatePrefabInstances();

		//Check if prefab is an overlay from the scene
		GameObject* IdIsInOverlays(int id);

	public:

		// Vector with Ids than can be reutilised
		static std::vector<int> unusedIds;

		PrefabManager();
		~PrefabManager();
		
		void Behaviour() override;

		static void Open();

		// Saves the prefabs info into a json
		static void SavePrefabs(const std::string& path);

		static void AddPrefab(GameObject* go);
		static void AddInstance(int prefabId, int instanceId);
		static void RemoveInstance(GameObject* instance);
		static void RemoveInstance(int prefabId, int instanceId);

		// Returns a prefab given its id
		static GameObject* GetPrefabById(int id);
	};
}
