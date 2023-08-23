#pragma once

#include "Window.h"

#include <deque>
#include <string>

#define FILEEXPLORER_WIN_HEIGHT_RATIO 0.3f

class Editor;

namespace ShyEditor {

	class Texture;

	class FileExplorer : public Window {

	public:

		FileExplorer();

		void Behaviour() override;
		void HandleInput(SDL_Event* event) override;

	private:

		Editor* editor;

		Texture* file;
		Texture* script;
		Texture* folder;
		Texture* scene;

		std::string projectPath;
		std::string currentPath;
		std::string relativePath;

		struct Entry {

			bool isFolder;
			std::string name;
			std::string path;
			std::string extension;
			Texture* texture;
		};


		int currentlySelected;
		std::deque<Entry> entries;


		void ProcessPath();
		std::string GetParentPath(const std::string& path);

		int viewMode;

		void DrawList();
		void DrawIcons();

		void OnItemSelected(Entry& entry);
		void ItemDrag(Entry& entry);


		bool shouldUpdate;

	};
}
