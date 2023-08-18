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

		enum Result { ENTERED, CLOSED };

		ProjectsManager();

		// Create the folder in AppData to contain the recent projects file
		bool MakeFolderToStoreRecentProjects();

		Result ManageProjectSelection(SDL_Renderer* renderer);

	private:

		ImGUIManager* imGuiManager;

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
		static std::wstring projectfileExtension;
		static std::wstring projectsfileFolder;
		static std::wstring projectsfileName;

		// Project information values
		std::string name;
		std::string createPath;
		std::string openPath;
		std::string creationDate;

		// Assets folders names
		const static std::vector<std::string> assetsFolders;

		// Recent projects information
		std::vector<ProjectInfo> recentProjectsInfo;

		// Path to the file that stores the recent projects paths
		std::wstring recentProjectsFile;

		// String to store the error messages
		std::wstring errorMessage;
		bool showPopUpWindowNewProject;
		bool showPopUpWindowOpenProject;

		void StartImGuiFrame();
		void ImGuiRender(SDL_Renderer* renderer);

		// Handles de input of the ImGui windows
		void HandleInput();

		// Validate input methods on input forms
		bool ValidateInputOnNewProject();
		bool ValidateInputOnOpenProject();

		// Gets the current date to store it in the project file
		std::string getCurrentDate();

		bool ReadInfoFromProjectFile(const std::string& filepath);

		// Main windows
		Result NewProject();
		Result OpenProject();
		Result CloseEditor();

		// Creates a project file and stores it in the specified path
		// and with the specified name given in the form
		// Returns true if everything went correctly, false otherwise
		bool SaveProject();

		// Creates all assets folders needed (Images, Sound, Fonts, Scenes, etc.)
		bool CreateAssetsFolders(const std::string& root);

		// Adds the path of the new project in the recent projects file
		bool StoreProjectPath(const std::string& path);

		// Ask users for permission to create the 'Recent Projects' file in the AppData path.
		bool AskForPermission();

		// Shows a pop-up window with an error message
		void ShowErrorPopup(const std::wstring& errorMessage);

		// Check if a file has a certain extension
		bool HasExtension(const std::wstring& filePath, const std::wstring& extension);
		bool IsDirectoryEmpty(const std::string& directory);
		bool HasNestedDirectoryWithName(const std::string& parentDir, const std::string& dirName);
		std::string GetLastPathComponent(const std::string& path, char delimiter);

	};

}

