#pragma once

#include "Window.h"

#include <vector>
#include <string>

#define FILEEXPLORER_WIN_HEIGHT_RATIO 0.3f

class Editor;

namespace ShyEditor {

	class Texture;

	class FileExplorer : public Window {

	private:

		Editor* editor;

		Texture* file;
		Texture* script;
		Texture* folder;

		std::string projectPath;
		std::string currentPath;

		struct Entry {

			bool isFolder;
			std::string name;
			std::string path;
			std::string extension;
			Texture* texture;
		};


		int currentlySelected;
		std::vector<Entry> entries;


		void ProcessPath();

		int viewMode;

		void DrawList();
		void DrawIcons();

		void OnItemSelected(Entry& entry);
		void ItemDrag(Entry& entry);


		bool shouldUpdate;

	public:

		FileExplorer();

		void Behaviour() override;


	};
}
