#pragma once

#include "Window.h"

#include <unordered_map>
#include <vector>

class Editor;

namespace ShyEditor {

	class Texture;
	class Entity;

	class PrefabManager: public Window {

	private:

		static PrefabManager* instance;
		static int lastPrefabId; // Starts at -1 and goes down

		static void AssignId(Entity* prefab);
		static bool IsIdAlreadyUsed(int id);

		Editor* editor;
		Texture* prefabText;

		int currentlySelected; // Currently selected prefab

		//Key = prefab Id, Value = all its instances in the scene
		std::unordered_map<int, std::vector<int>> prefabInstances;

		std::unordered_map<int, Entity*> prefabs;

		void DrawPrefabList();
		void DrawPrefab(Entity* prefab);
		void DrawImage();
		void DrawComponents();

		void HandlePrefabDeletion();
		void UpdatePrefabInstances();

		//Check if prefab is an overlay from the scene
		Entity* IdIsInOverlays(int id);

		//If the prefab contains a scirpt that references an entity, we remove that reference
		static void RemoveScriptReferences(Entity* prefab);

	public:

		// Vector with Ids than can be reutilised
		static std::vector<int> unusedIds;

		PrefabManager();
		~PrefabManager();

		void Behaviour() override;

		static void Open();

		// Saves the prefabs info into a json
		static void SavePrefabs();

		//Loading prefabs from prefabs json
		static void LoadPrefabs();

		static void AddPrefab(Entity* entity);
		static void AddInstance(int prefabId, int instanceId);
		static void RemoveInstance(Entity* instance);
		static void RemoveInstance(int prefabId, int instanceId);
		static std::unordered_map<int, Entity*>& GetPrefabs();

		// Returns a prefab given its id
		static Entity* GetPrefabById(int id);
	};
}
