#pragma once

#include <string>
#include <vector>

class ImGUIManager;
struct SDL_Renderer;

namespace PEditor {

	struct ProjectInfo {

		ProjectInfo() { name = ""; date = ""; path = ""; };
		ProjectInfo(const std::string& name, const std::string& date, const std::string& path) {
			this->name = name; this->date = date; this->path = path;
		};

		std::string name;
		std::string date;
		std::string path;
	};

	class ProjectsManager {

	public:

		enum Result { OPENED, CREATED, CLOSED };

		ProjectsManager();

		Result ManageProjectSelection(SDL_Renderer* renderer);

	private:

		bool windowClosed;

		// Booleans to indicate if the input is valid or not 
		bool invalidProjectName;
		bool invalidNewProjectPath;
		bool invalidOpenProjectPath;

		float w, h;

		// Buffers to store the input values
		static char project_name[128]; 
		static char create_project_path[256]; 
		static char open_project_path[256]; 

		// Folder and files paths information
		static const std::wstring projectfileExtension;
		static const std::wstring projectsfileFolder;
		static const std::wstring projectsfileName;

		std::vector<ProjectInfo> recentProjectsInfo;

		// Path to the file that stores the recent projects paths
		std::wstring recentProjectsFile;

		// Handles de input of the ImGui windows
		void HandleInput();

		// Validate input methods on input forms
		bool ValidateInputOnNewProject();
		bool ValidateInputOnOpenProject();

		// Gets the current date to store it in the project file
		std::string getCurrentDate();

		// Main windows
		Result NewProject();
		Result OpenProject();
		Result CloseEditor();

		// Create a project file and stores it in the specified path
		// and with the specified name given in the form
		void SaveProject();

		// Adds the path of the new project in the recent projects file
		void StoreProjectPath(const std::string& path);

		// Create the folder in AppData to contain the recent projects file
		void MakeFolderToStoreRecentProjects();
		bool AskForPermission();

		// Check if a file has a certain extension
		bool HasExtension(const std::wstring& filePath, const std::wstring& extension);

	};

}

