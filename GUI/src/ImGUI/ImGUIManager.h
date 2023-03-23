#pragma once
#include <vector>

struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;
struct ImVec2;

namespace PEditor {
	class Window;
	class Scene;
};

class ImGUIManager
{
private:

	static ImGUIManager* instance;

	// Pointers to our window and surface
	SDL_Window* window;
	SDL_Surface* winSurface = nullptr;
	SDL_Renderer* renderer = nullptr;

	PEditor::Scene* scene;
	
	bool exit = false;

	std::vector<PEditor::Window*> windows;
	
	ImVec2* originalWindowSize;

	void initImGUI();
	void initSDL();
	void createSDLWindow(const char* name, int posX, int posY, int sizeX, int sizeY);
	void createSDLRenderer();

public:

	static ImGUIManager* getInstance();
	
	ImGUIManager();

	void init();
	void loop();

	void update();
	void render();
	void handleInput();

	void addWindow(PEditor::Window* window);
	void setScene(PEditor::Scene* scene);
		

	SDL_Renderer* getRenderer();
	ImVec2 getOriginalWindowSize();
	ImVec2 getMainWindowSize();

	PEditor::Scene* getScene();

	~ImGUIManager();
};

