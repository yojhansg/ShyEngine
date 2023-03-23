#include "ImGUIManager.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "Window.h"
#include "SDL.h"
#include <iostream>
#include "Scene.h"
#include "GameObject.h"

ImGUIManager* ImGUIManager::instance = nullptr;

ImGUIManager::ImGUIManager() {
    init();
}

void ImGUIManager::initImGUI()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    initSDL();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

}

void ImGUIManager::initSDL()
{
    // Initialize SDL. If something fails it returns -1 and we throw an exception
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        system("pause");

        // ERROR HANDLING
    }

    originalWindowSize = new ImVec2(1080, 720);
    createSDLWindow("PEditor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, originalWindowSize->x, originalWindowSize->y);
    createSDLRenderer();
}

void ImGUIManager::createSDLWindow(const char* name, int posX, int posY, int sizeX, int sizeY) {

	// Create our window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	window = SDL_CreateWindow(name, posX, posY, sizeX, sizeY, window_flags);

	// Make sure creating the window succeeded
	if (!window) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");

		// ERROR HANDLING
	}

	// Get the surface from the window
	winSurface = SDL_GetWindowSurface(window);

	// Make sure getting the surface succeeded
	if (!winSurface) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");

		// ERROR HANDLING
	}
}

void ImGUIManager::createSDLRenderer()
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_Log("Error creating SDL_Renderer!");

		// ERROR HANDLING
	}
}

void ImGUIManager::init()
{
    initImGUI();
}

ImGUIManager* ImGUIManager::getInstance()
{
    if (instance == nullptr) {
        instance = new ImGUIManager();
    }
    
    return instance;

}

void ImGUIManager::loop()
{

    while (!exit)
    {
        update();
        handleInput();
        render();
    }
 
}

void ImGUIManager::update()
{
    for (auto window : windows)
    {
        window->update();
    }
}

void ImGUIManager::render()
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (auto window : windows)
    {
        window->render();
    }


    // Rendering
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

void ImGUIManager::handleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            exit = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            exit = true;
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            exit = true;

        for (auto window : windows)
        {
            window->handleInput(&event);
        }
    }
}

void ImGUIManager::addWindow(PEditor::Window* window)
{
    windows.push_back(window);
}

void ImGUIManager::setScene(PEditor::Scene* scene)
{
    this->scene = scene;
}

SDL_Renderer* ImGUIManager::getRenderer()
{
    return renderer;
}

ImVec2 ImGUIManager::getOriginalWindowSize()
{
    return *originalWindowSize;
}



ImGUIManager::~ImGUIManager()
{
    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    for (auto window : windows)
    {
        delete window;
    }

    delete originalWindowSize;
}

ImVec2 ImGUIManager::getMainWindowSize()
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    return ImVec2(w, h);
}

PEditor::Scene* ImGUIManager::getScene()
{
    return scene;
}
