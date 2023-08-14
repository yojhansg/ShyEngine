#pragma once

class ImGUIManager;

struct SDL_Renderer;

namespace PEditor {

	class ProjectsManager {

	public:

		enum Result { OPENED, CREATED, CLOSED };

		ProjectsManager();

		Result ManageProjectSelection(SDL_Renderer* renderer);

	private:

		bool windowClosed;
		bool invalidProjectName;
		bool invalidNewProjectPath;
		bool invalidOpenProjectPath;

		float w, h;

		static char project_name[128]; // Nombre del proyecto
		static char create_project_path[256]; // Ruta del proyecto a crear
		static char open_project_path[256]; // Ruta del proyecto a abrir

		void HandleInput();

		bool ValidateInputNew();
		bool ValidateInputOpen();

		Result NewProject();
		Result OpenProject();

		void SaveProject();

		void MakeFolderToStoreRecentProjects();
		bool AskForPermission();

	};

}

