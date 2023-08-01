#pragma once
#include <vector>

struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;
struct ImVec2;

namespace PEditor {
	class Window;
	class Scene;
	class MenuBar;
	class Hierarchy;
	class FileExplorer;
	class Components;
};

namespace PEditor {

	class Window;
	class Scene;
	class MenuBar;
	class Hierarchy;
	class FileExplorer;
	class Components;

}

class ImGUIManager
{
private:

	static ImGUIManager* instance;

	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Surface* winSurface = nullptr;
	SDL_Renderer* renderer = nullptr;


	bool exitGame = false;
	bool isCreatingScript = false;

	std::vector<PEditor::Window*> windows;

	ImVec2* originalWindowSize;
	ImVec2* gameSize;

	void initImGUI();
	void initSDL();
	void initWindows();
	void createSDLWindow(const char* name, int posX, int posY, int sizeX, int sizeY);
	void createSDLRenderer();

	PEditor::Scene* scene;
	PEditor::MenuBar* menuBar = nullptr;
	PEditor::Hierarchy* hierarchy = nullptr;
	PEditor::FileExplorer* fileExplorer = nullptr;
	PEditor::Components* components = nullptr;


public:

	static ImGUIManager* getInstance();

	ImGUIManager();

	void init();
	void loop();
	void exit();

	void creatingScript(bool isCreating);

	void update();
	void render();
	void handleInput();

	void addWindow(PEditor::Window* window);
	void setScene(PEditor::Scene* scene);


	SDL_Renderer* getRenderer();
	ImVec2 getOriginalWindowSize();
	ImVec2 getMainWindowSize();

	ImVec2 getGameSize();

	PEditor::Scene* getScene();
	PEditor::MenuBar* getMenuBar();
	PEditor::Hierarchy* getHierarchy();
	PEditor::FileExplorer* getFileExplorer();
	PEditor::Components* getComponents();

	~ImGUIManager();
};
