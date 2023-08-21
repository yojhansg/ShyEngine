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


		Editor* editor;

		void handleDragAndDrop(GameObject* source, GameObject* destination);

		bool isChildrenTheSelectedObject(GameObject* go);

		void renderGameObjectHierarchy(GameObject* gameObject, int indentLevel);
		void showRenamePopup(GameObject* gameObject);
		void showGameObjectMenu(GameObject* gameObject);
		void showSavePrefabPopup(GameObject* gameObject);

		bool shouldOpenRenamePopup;
		bool shouldOpenSavePrefabPopup;

	};
}
