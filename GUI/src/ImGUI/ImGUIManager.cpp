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
#include "Game.h"
#include "Console.h"
#include "Preferences.h"
#include "ColorPalette.h"

#define _Centered SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED

ImGUIManager* ImGUIManager::instance = nullptr;

ImGUIManager::ImGUIManager() {

	gameSize = nullptr;
	scene = nullptr;
	window = nullptr;
	projecInfo = nullptr;

	state = EDITOR_WINDOW;

}

void ImGUIManager::initImGUI()
{
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


	//Aqui se tienen que cargar las fuentes


	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

}

void ImGUIManager::initWindows()
{
	//MENU BAR
	menuBar = new PEditor::MenuBar();
	addWindow(menuBar);

	//SCENE
	scene = new PEditor::Scene();
	addWindow(scene);

	//HIERARCHY
	hierarchy = new PEditor::Hierarchy();
	addWindow(hierarchy);

	//FILE EXPLORER
	fileExplorer = new PEditor::FileExplorer();
	addWindow(fileExplorer);

	//COMPONENTS
	components = new PEditor::ComponentWindow();
	addWindow(components);

	scriptCreation = new PEditor::ScriptCreation();
	addWindow(scriptCreation);


	console = new PEditor::Console();
	addWindow(console);

	addWindow(new PEditor::Preferences());
}

void ImGUIManager::initSDL()
{
	// Initialize SDL. If something fails it returns -1 and we throw an exception
	//if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
	//	std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
	//	system("pause");

	//	// ERROR HANDLING
	//}

	createSDLWindow("SHY Engine", _Centered, _WindowMainSize);
	createSDLRenderer();



	SDL_Surface* surf = IMG_Load("shyIcon2.png");

	SDL_SetWindowIcon(window, surf);

}

void ImGUIManager::createSDLWindow(const char* name, int posX, int posY, int sizeX, int sizeY) {

	// Create our window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	window = SDL_CreateWindow(name, posX, posY, _ProjectSelectionDialogueSize, window_flags);

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
	//SplashScreen();

	gameSize = new ImVec2{ _WindowMainSize };

	initImGUI();

	Components::ComponentManager::Initialise();
	Components::ComponentManager::ReadComponentInfo("Engine/Components.json");
	Components::ComponentManager::ReadManagerInfo("Engine/Managers.json");
	Components::ComponentManager::ReadScripts("Scripts");

	PEditor::Game::Init("Main_Debug.exe");
}

void ImGUIManager::SplashScreen()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");

		// ERROR HANDLING
	}

	auto window = SDL_CreateWindow("SplashScreen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500,
		SDL_WINDOW_BORDERLESS
	);

	auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetWindowBordered(window, SDL_FALSE);
	SDL_SetWindowOpacity(window, 0.75f);

	auto surf = IMG_Load("SplashScreen.png");
	auto texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Color transparente (RGBA)
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderPresent(renderer);
	SDL_Delay(1500);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(texture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

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
	addWindow(new PEditor::ColorPalette("theme"));

	//SDL_SetWindowSize(window, _ProjectSelectionDialogueSize);
	SDL_SetWindowPosition(window, _Centered);
	SDL_SetWindowResizable(window, SDL_FALSE);

	PEditor::ProjectsManager dialog;

	if (!dialog.MakeFolderToStoreRecentProjects())
		return;

	auto result = dialog.ManageProjectSelection(renderer);

	if (result == PEditor::ProjectsManager::Result::CLOSED)
		return;

	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetWindowSize(window, _WindowMainSize);
	SDL_SetWindowPosition(window, _Centered);
	SDL_SetWindowBordered(window, SDL_TRUE);

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

void ImGUIManager::setProjectInfo(PEditor::ProjectInfo* pInfo) {
	this->projecInfo = pInfo;
}


void ImGUIManager::update()
{
	PEditor::Game::CheckEnd();

	for (auto window : windows)
	{
		if (window->CanBeDrawnOnTop())
			window->update();
		else
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

	PEditor::ColorPalette::ApplyPalette();

	for (auto window : windows)
	{
		if (window->CanBeDrawnOnTop())
			window->render();
		else
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


ImGUIManager::~ImGUIManager()
{
	PEditor::Game::Release();
	Components::ComponentManager::Release();

	// Cleanup
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	for (auto window : windows)
		delete window;

	delete gameSize;
	delete projecInfo;

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

void ImGUIManager::OpenScript(const std::string& script)
{
	scriptCreation->SetName(script);
	changeEditorState(EDITOR_STATE::SCRIPTING_WINDOW);
}

