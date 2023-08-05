#pragma once
#include <vector>
#include "Window.h"
#include <string>


struct SDL_Texture;

namespace PEditor {
	class FileExplorer : public Window
	{
	private:
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
