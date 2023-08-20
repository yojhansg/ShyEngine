#pragma once

#include "Window.h"

#include <vector>
#include <string>

#define FILEEXPLORER_WIN_HEIGHT_RATIO 0.3f

struct SDL_Texture;
class Editor;

namespace ShyEditor {

	class FileExplorer : public Window {

	private:

		Editor* editor;

		std::string projectPath;
		std::string currentPath;

		SDL_Texture* fileTexture;
		SDL_Texture* scriptTexture;
		SDL_Texture* folderTexture;

	public:

		FileExplorer();

		virtual void render();

		void drawFileExplorerWindow();
	};
}
