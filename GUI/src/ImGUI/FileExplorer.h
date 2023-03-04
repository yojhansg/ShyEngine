#pragma once
#include <vector>
#include "Window.h"
#include <string>

namespace PEditor {
	class FileExplorer : public Window
	{
	private:
		 std::string current_path;
	public:

		FileExplorer();

		virtual void render();

		void drawFileExplorerWindow();
	};
}
