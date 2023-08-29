#pragma once
#include <vector>
#include "Window.h"

#define HIERARCHY_WIN_WIDTH_RATIO 0.4f
#define HIERARCHY_WIN_HEIGHT_RATIO 0.7f

union SDL_Event;
class Editor;

namespace ShyEditor {

	class Entity;

	class Hierarchy: public Window {

	public:
		
		Hierarchy();
		~Hierarchy();

	protected:

		void Behaviour() override;
		void HandleInput(SDL_Event* event) override;
		void ReceiveAssetDrop(Asset& asset) override;

		void handleDragAndDrop(Entity* source, Entity* destination);

		bool isChildrenTheSelectedEntity(Entity* entity);
		bool isParentFromPrefab(Entity* entity);

		void RenderEntity(Entity* entity, const char* type);



		void ShowRenamePopup(Entity* entity);
		void ShowEntityMenu(Entity* entity);

		bool shouldOpenRenamePopup;

	private:

		Entity* copiedEntity;
	};
}
