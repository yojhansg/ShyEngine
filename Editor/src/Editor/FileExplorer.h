#pragma once

#include "Window.h"

#include <filesystem>
#include <string>
#include <deque>

#define FILEEXPLORER_WIN_HEIGHT_RATIO 0.3f

class Editor;

namespace ShyEditor {

	class Texture;

	class FileExplorer : public Window {

	public:

		FileExplorer();

		void Behaviour() override;
		void HandleInput(SDL_Event* event) override;
		void Refresh();

	private:

		Editor* editor;

		Texture* file;
		Texture* script;
		Texture* folder;
		Texture* scene;
		Texture* audio;

		std::string assetPath;
		std::string currentPath;
		std::string relativePath;

		struct Entry {

			bool isFolder;
			std::string name;
			std::string path;
			std::string extension;
			Texture* texture;

			bool operator<(const Entry& otro) const {
				return name < otro.name;
			}

		};


		int currentlySelected;
		std::deque<Entry> entries;

		void ProcessPath();

		int viewMode;

		void DrawList();
		void DrawIcons();

		void OnItemSelected(Entry& entry);
		void ItemDrag(Entry& entry);

		void ShowDeleteFilePopup();
		void ShowFileMenuPopup();

		Texture* SetTextureToFile(Entry& entry);

		Entry entryToDelete;

		bool shouldUpdate;
		bool shouldOpenDeleteFilePopup;
		bool shouldOpenFileMenu;
	};
}
