#pragma once
#include <vector>
#include "Window.h"
#include <string>

namespace PEditor {
	class FileExplorer : public Window
	{
	private:
		std::string projectPath;
		std::string currentPath;
	public:

		FileExplorer();

		virtual void render();

		void drawFileExplorerWindow();
	};
}
