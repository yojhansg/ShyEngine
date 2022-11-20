#pragma once
#include <vector>

struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;
struct ImVec2;

namespace PEditor {
	class Window;
};

class ImGUIManager
{
private:

	static ImGUIManager* instance;

	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Surface* winSurface = nullptr;
	SDL_Renderer* renderer = nullptr;
	
	bool exit = false;

	std::vector<PEditor::Window*> windows;

	void initImGUI();
	void initSDL();

	void createWindow(const char* name, int posX, int posY, int sizeX, int sizeY);
	void createRenderer();

public:

	static ImGUIManager* getInstance();
	
	ImGUIManager();

	void init();
	void loop();

	void handleInput();

	void addWindow(PEditor::Window* window);

	SDL_Renderer* getRenderer();

	ImVec2 getMainWindowSize();

	~ImGUIManager();
};

