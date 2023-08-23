#pragma once
#include <vector>
#include "Window.h"

#define HIERARCHY_WIN_WIDTH_RATIO 0.4f
#define HIERARCHY_WIN_HEIGHT_RATIO 0.7f

union SDL_Event;
class Editor;

namespace ShyEditor {

	class GameObject;

	class Hierarchy: public Window {

	public:
		
		Hierarchy();


	protected:

		void Behaviour() override;
		void ReceiveAssetDrop(Asset& asset) override;

		void handleDragAndDrop(GameObject* source, GameObject* destination);

		bool isChildrenTheSelectedObject(GameObject* go);

		void RenderGameObject(GameObject* gameObject, const char* type);



		void showRenamePopup(GameObject* gameObject);
		void showGameObjectMenu(GameObject* gameObject);
		void showSavePrefabPopup(GameObject* gameObject);

		bool shouldOpenRenamePopup;
		bool shouldOpenSavePrefabPopup;

	};
}
