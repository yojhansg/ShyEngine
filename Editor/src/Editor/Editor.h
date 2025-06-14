#pragma once
#include <vector>
#include <string>

#include "Vector2.h"

#define _ProjectSelectionDialogueSize 1080, 720
#define _WindowMainSize 1920, 1080

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Window;
struct ImVec2;

namespace ShyEditor {
	class ProjectsManager;
	class ComponentWindow;
	class ScriptCreation;
	class FileExplorer;
	struct ProjectInfo;
	class Hierarchy;
	class MenuBar;
	class Console;
	class Window;
	class Scene;

	class WindowLayout;
	class Build;
};

class Editor {

public:

	enum EDITOR_STATE { EDITOR_WINDOW, SCRIPTING_WINDOW };

	static Editor* GetInstance();

	// Inits ImGUI and SDL
	static bool Init();

	// Main loop of the editor
	static void Loop();

	// Shuts down the editor
	static void End();

	// Closes ImGUI and SDL
	static void Close();


	void AddWindow(ShyEditor::Window* window);
	void SetScene(ShyEditor::Scene* scene);

	void ChangeEditorState(const EDITOR_STATE& state);
	void SetProjectInfo(ShyEditor::ProjectInfo* pInfo);
	ShyEditor::ProjectInfo& GetProjectInfo();

	void SetLastOpenedScene(const std::string& scene);
	std::string GetLastOpenedScene();

	void SetAnySceneOpened(bool opened);
	bool IsAnySceneOpened();

	SDL_Renderer* GetRenderer();
	ImVec2 GetMainWindowSize();

	void OpenScript(const std::string& script);

	ShyEditor::Scene* GetScene();
	ShyEditor::FileExplorer* GetFileExplorer();
	ShyEditor::Console* GetConsole();

	ShyEditor::WindowLayout* GetWindowLayout();
	ShyEditor::Build* GetBuildManager();

	int Example();

	std::vector<ShyEditor::Window*>& GetAllWindows();


	/*
		Auixiliary function for the editor
	*/
	static void HelpMarker(const std::string& str);
	static std::string Beautify(const std::string& str);

private:

	Editor();
	~Editor();

	// Loop methods
	void UpdateAndRenderWindows();
	void HandleInput();

	// Logic for running the projects management window
	bool RunProjectsWindow();

	// Initialisation 
	bool InitImGUIAndSDL();
	bool InitSDL();

	void CreateWindows();
	bool SplashScreen();

	// Imgui.ini configuration
	void StoreImGuiINIFile();
	void LoadImGuiINIFile();

	void StoreWindowsData();
	void LoadWindowsData();

	static Editor* instance;

	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Renderer* renderer;

	ShyEditor::Vector2 userResolution;

	// Project info structure 
	ShyEditor::ProjectInfo* projecInfo;

	bool exitEditor;
	EDITOR_STATE state;

	// All editor windows
	std::vector<ShyEditor::Window*> windows;

	// Path to the last opene scene
	std::string lastOpenedScene;
	bool anySceneOpened;

	// References to the ImGUI windows
	ShyEditor::Scene* scene;
	ShyEditor::MenuBar* menuBar;
	ShyEditor::Hierarchy* hierarchy;
	ShyEditor::FileExplorer* fileExplorer;
	ShyEditor::ComponentWindow* components;
	ShyEditor::ScriptCreation* scriptCreation;
	ShyEditor::Console* console;
	ShyEditor::WindowLayout* layout;
	ShyEditor::Build* build;
};
