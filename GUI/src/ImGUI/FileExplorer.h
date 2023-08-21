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

	public:

		FileExplorer();

		void Behaviour() override;

		void drawFileExplorerWindow();
	};
}
