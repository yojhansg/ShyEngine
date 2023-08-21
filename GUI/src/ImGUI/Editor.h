#pragma once
#include <vector>
#include <string>

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
};

class Editor {

public:

	enum EDITOR_STATE { EDITOR_WINDOW, SCRIPTING_WINDOW };

	static Editor* getInstance();

	// Inits ImGUI and SDL
	static bool Init();

	// Main loop of the editor
	static void Loop();

	// Shuts down the editor
	static void End();

	// Closes ImGUI and SDL
	static void Close();


	void addWindow(ShyEditor::Window* window);
	void setScene(ShyEditor::Scene* scene);

	void changeEditorState(const EDITOR_STATE& state);
	void setProjectInfo(ShyEditor::ProjectInfo* pInfo);
	ShyEditor::ProjectInfo& getProjectInfo();

	SDL_Renderer* getRenderer();
	ImVec2 getMainWindowSize();

	void OpenScript(const std::string& script);

	ShyEditor::Scene* getScene();
	ShyEditor::MenuBar* getMenuBar();
	ShyEditor::Hierarchy* getHierarchy();
	ShyEditor::FileExplorer* getFileExplorer();
	ShyEditor::ComponentWindow* getComponents();
	ShyEditor::ScriptCreation* getScriptCreation();
	ShyEditor::Console* getConsole();

	int Example();
	int Probando();

private:

	Editor();
	~Editor();

	// Loop methods


	void UpdateAndRenderWindows();
	void handleInput();

	// Logic for running the projects management window
	bool runProjectsWindow();

	// Initialisation 
	bool initImGUIAndSDL();
	bool initSDL();

	void SetUpWindows();
	void SplashScreen();

	static Editor* instance;

	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Project info structure 
	ShyEditor::ProjectInfo* projecInfo;

	bool exitEditor;
	EDITOR_STATE state;

	// All editor windows
	std::vector<ShyEditor::Window*> windows;

	// References to the ImGUI windows
	ShyEditor::Scene* scene;
	ShyEditor::MenuBar* menuBar;
	ShyEditor::Hierarchy* hierarchy;
	ShyEditor::FileExplorer* fileExplorer;
	ShyEditor::ComponentWindow* components;
	ShyEditor::ScriptCreation* scriptCreation;
	ShyEditor::Console* console;

};
