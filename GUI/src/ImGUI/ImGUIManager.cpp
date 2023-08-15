#include "ImGUIManager.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "Window.h"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "Scene.h"
#include "GameObject.h"
#include "MenuBar.h"
#include "Hierarchy.h"
#include "FileExplorer.h"
#include "Components.h"
#include <fstream>
#include "ScriptCreation.h"
#include "ComponentManager.h"
#include "ProjectsManager.h"

ImGUIManager* ImGUIManager::instance = nullptr;

ImGUIManager::ImGUIManager() {

	gameSize = nullptr;
	originalWindowSize = nullptr;
	scene = nullptr;
	window = nullptr;

	state = EDITOR_WINDOW;

}

struct ImVec3 { float x, y, z; ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) { x = _x; y = _y; z = _z; } };

void ImGUIManager::initImGUI()
{
	std::cout << "INITIALISING IMGUI\n";
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = true;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	initSDL();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	//Cargar las fuentes

	auto defaultFont = io.Fonts->AddFontFromFileTTF("Fonts/Montserrat-Regular.ttf", 18.0f);
	io.FontDefault = defaultFont;


	ImVec3 color_for_text = ImVec3(236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
	ImVec3 color_for_head = ImVec3(41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
	ImVec3 color_for_area = ImVec3(57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
	ImVec3 color_for_body = ImVec3(44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
	ImVec3 color_for_pops = ImVec3(33.f / 255.f, 46.f / 255.f, 60.f / 255.f);


	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.58f);
	style.Colors[ImGuiCol_Border] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.32f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	//style.Colors[ImGuiCol_ColumnActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.16f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.39f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);

	style.WindowRounding = 10;


	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

}

void ImGUIManager::initWindows()
{

	std::cout << "INITIALISING WINDOWS\n";

	//MENU BAR
	menuBar = new PEditor::MenuBar();
	addWindow(menuBar);

	//SCENE
	scene = new PEditor::Scene();
	addWindow(scene);

	//HIERARCHY
	hierarchy = new PEditor::Hierarchy();
	addWindow(hierarchy);

	////FILE EXPLORER
	fileExplorer = new PEditor::FileExplorer();
	addWindow(fileExplorer);

	//COMPONENTS
	components = new PEditor::ComponentWindow();
	addWindow(components);

	scriptCreation = new PEditor::ScriptCreation();
	addWindow(scriptCreation);

}

void ImGUIManager::initSDL()
{
	std::cout << "INITIALISING SDL\n";

	// Initialize SDL. If something fails it returns -1 and we throw an exception
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");

		// ERROR HANDLING
	}

	originalWindowSize = new ImVec2(SDL_WIN_WIDTH, SDL_WIN_HEIGHT);
	createSDLWindow("PEditor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SDL_WIN_WIDTH, SDL_WIN_HEIGHT);
	createSDLRenderer();



	SDL_Surface* surf = IMG_Load("shyIcon2.png");

	SDL_SetWindowIcon(window, surf);

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
	gameSize = new ImVec2{ GAME_WIDTH, GAME_HEIGHT };

	initImGUI();

    Components::ComponentManager::Initialise();
    Components::ComponentManager::ReadComponentInfo("Engine/Components.json");
    Components::ComponentManager::ReadManagerInfo("Engine/Managers.json");
	Components::ComponentManager::ReadScripts("Scripts");
}

ImGUIManager* ImGUIManager::getInstance()
{
	if (instance == nullptr) {
		instance = new ImGUIManager();
		instance->init();
	}

	return instance;
}

void ImGUIManager::loop()
{
    SDL_SetWindowSize(window, 1080, 720);
	SDL_SetWindowResizable(window, SDL_FALSE);

    PEditor::ProjectsManager dialog;
    auto result = dialog.ManageProjectSelection(renderer);

    if (result == PEditor::ProjectsManager::Result::CLOSED)
        return;

	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetWindowSize(window, originalWindowSize->x, originalWindowSize->y);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	initWindows();

	while (!exitGame)
	{
		update();
		handleInput();
		render();
	}

}

void ImGUIManager::exit()
{
	exitGame = true;
}

void ImGUIManager::changeEditorState(const EDITOR_STATE& state) {

    switch (state)
    {
        case ImGUIManager::EDITOR_WINDOW:
            break;
        case ImGUIManager::SCRIPTING_WINDOW:
            scriptCreation->Load();
            break;
        default:
            break;
    }

	this->state = state;
}


void ImGUIManager::update()
{
    for (auto window : windows)
    {
        switch (state)
        {
            case ImGUIManager::SCRIPTING_WINDOW:
                if (window == scriptCreation) 
                    window->update();
                break;
            case ImGUIManager::EDITOR_WINDOW:
                if (window != scriptCreation)
                    window->update();
                break;
            default:
                break;
        }
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
        switch (state)
        {
        case ImGUIManager::SCRIPTING_WINDOW:
            if (window == scriptCreation) 
                window->render();
            break;
        case ImGUIManager::EDITOR_WINDOW:
            if (window != scriptCreation)
                window->render();
            break;
        default:
            break;
        }
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
			exitGame = true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
			exitGame = true;
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			exitGame = true;

        for (auto window : windows)
            window->handleInput(&event);
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
	Components::ComponentManager::Release();
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
    delete gameSize;

}

ImVec2 ImGUIManager::getMainWindowSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	return ImVec2(w, h);
}

ImVec2 ImGUIManager::getGameSize()
{
	return *gameSize;
}

PEditor::Scene* ImGUIManager::getScene()
{
	return scene;
}

PEditor::MenuBar* ImGUIManager::getMenuBar()
{
	return menuBar;
}

PEditor::Hierarchy* ImGUIManager::getHierarchy()
{
	return hierarchy;
}

PEditor::FileExplorer* ImGUIManager::getFileExplorer()
{
	return fileExplorer;
}

PEditor::ComponentWindow* ImGUIManager::getComponents()
{
	return components;
}

PEditor::ScriptCreation* ImGUIManager::getScriptCreation()
{
	return scriptCreation;
}

