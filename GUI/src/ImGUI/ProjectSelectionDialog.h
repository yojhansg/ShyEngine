#pragma once

#include <string>



struct SDL_Renderer;

namespace PEditor {



	class ProjectSelectionDialog {



	private:

		bool windowClosed;
		std::string path;



		void HandleInput();

	public:

		enum class Result {

			Selected, Created, Closed
		};

		ProjectSelectionDialog();
		Result ManageProjectSelectionDialog(SDL_Renderer* rend);
	};

}
