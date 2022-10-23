#pragma once
#include <vector>

struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;

namespace PEditor {
	class Window;
};

class ImGUIManager
{
private:
	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Surface* winSurface = nullptr;
	SDL_Renderer* renderer = nullptr;
	
	std::vector<PEditor::Window*> windows;

	void initImGUI();
	void initSDL();

	void createWindow(const char* name, int posX, int posY, int sizeX, int sizeY);
	void createRenderer();

public:
	
	void init();
	void loop();

	void addWindow(PEditor::Window* window);

	~ImGUIManager();
};

