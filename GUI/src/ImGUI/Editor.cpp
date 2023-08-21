#include "Editor.h"

#include "imgui_impl_sdlrenderer.h"
#include "ComponentManager.h"
#include "ResourcesManager.h" 
#include "ProjectsManager.h"
#include "imgui_impl_sdl.h"
#include "ScriptCreation.h"
#include "FileExplorer.h"
#include "ColorPalette.h"
#include "Preferences.h"
#include "Components.h"
#include "GameObject.h"
#include "SDL_image.h"
#include "Hierarchy.h"
#include "MenuBar.h"
#include "Console.h"
#include "Window.h"
#include "imgui.h"
#include "Scene.h"
#include "Game.h"
#include "SDL.h"

#include "ResourcesManager.h"

#include <fstream>
#include <iostream>

#define _Centered SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED

Editor* Editor::instance = nullptr;

Editor::Editor() {

	window = nullptr;
	renderer = nullptr;
	projecInfo = nullptr;

	scene = nullptr;
	menuBar = nullptr;
	hierarchy = nullptr;
	fileExplorer = nullptr;
	components = nullptr;
	scriptCreation = nullptr;
	console = nullptr;

	exitEditor = false;
	state = EDITOR_WINDOW;
}

Editor::~Editor() {}

Editor* Editor::getInstance()
{
	if (instance == nullptr) {
		instance = new Editor();
	}

	return instance;
}

bool Editor::Init() {



	instance->SplashScreen();

	if (!instance->initImGUIAndSDL())
		return false;

	Components::ComponentManager::Initialise();
	Components::ComponentManager::ReadComponentInfo("Engine/Components.json");
	Components::ComponentManager::ReadManagerInfo("Engine/Managers.json");

	ShyEditor::Game::Init("Main_Debug.exe", "Main_Release.exe");

	ShyEditor::ResourcesManager::Init();

	return true;
}

void Editor::Loop() {

	// The projects management window
	if (!instance->runProjectsWindow())
		instance->exitEditor = true;

	if (!instance->exitEditor) {
		// Configure the SDL window to start the editor
		SDL_SetWindowResizable(instance->window, SDL_TRUE);
		SDL_SetWindowSize(instance->window, _WindowMainSize);
		SDL_SetWindowPosition(instance->window, _Centered);

		// Init the ImGUI windows in the editor
		instance->SetUpWindows();
	}

	// Editor main loop
	while (!instance->exitEditor) {

		ShyEditor::Game::CheckEnd();

		instance->handleInput();
		instance->UpdateAndRenderWindows();
	}

}

void Editor::End() {
	instance->exitEditor = true;
}

void Editor::Close() {

	ShyEditor::ResourcesManager::Release();

	ShyEditor::Game::Release();
	Components::ComponentManager::Release();

	// Cleanup
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(instance->renderer);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();

	for (auto window : instance->windows)
		delete window;

	delete instance->projecInfo;
}


bool Editor::initImGUIAndSDL() {

	// ImGUI

		// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = true;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();


// SDL

	if (!initSDL())
		return false;

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);


	return true;
}

bool Editor::initSDL() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		// Avisar por el log
		return false;
	}

	// WINDOW

		// Create our window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	window = SDL_CreateWindow("SHY Engine", _Centered, _ProjectSelectionDialogueSize, window_flags);

	// Make sure creating the window succeeded
	if (window == NULL) {
		SDL_Quit();
		// Avisar por el log
		return false;
	}

	// RENDERER

		// Create our renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		// Avisar por el log
		return false;
	}

	SDL_Surface* s = IMG_Load("shyIcon2.png");
	SDL_SetWindowIcon(window, IMG_Load("shyIcon2.png"));
	SDL_FreeSurface(s);

	return true;

}

void Editor::SetUpWindows() {

	// PREFERENCES
	addWindow(new ShyEditor::Preferences());

	// MENU BAR
	menuBar = new ShyEditor::MenuBar();

	// GAME SCENE
	scene = new ShyEditor::Scene();
	addWindow(scene);

	// HIERARCHY
	hierarchy = new ShyEditor::Hierarchy();
	addWindow(hierarchy);

	// FILE EXPLORER
	fileExplorer = new ShyEditor::FileExplorer();
	addWindow(fileExplorer);

	// COMPONENTS
	components = new ShyEditor::ComponentWindow();
	addWindow(components);

	// SCRIPTING
	scriptCreation = new ShyEditor::ScriptCreation();
	addWindow(scriptCreation);

	// CONSOLE
	console = new ShyEditor::Console();
	addWindow(console);


	scene->SetLeft(hierarchy);
	scene->SetRight(components);
	scene->SetBottom(fileExplorer);
}

void Editor::SplashScreen() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		// Avisar por el log
		return;
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


bool Editor::runProjectsWindow() {

	instance->addWindow(new ShyEditor::ColorPalette("theme"));

	SDL_SetWindowPosition(instance->window, _Centered);

	ShyEditor::ProjectsManager dialog;

	if (!dialog.MakeFolderToStoreRecentProjects())
		return false;

	auto result = dialog.ManageProjectSelection(instance->renderer);

	if (result == ShyEditor::ProjectsManager::Result::CLOSED)
		return false;

	Components::ComponentManager::ReadScripts(instance->projecInfo->path + "/Scripts");

	return true;

}


void Editor::UpdateAndRenderWindows()
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ShyEditor::ColorPalette::ApplyPalette();

	for (auto window : windows)
	{
		if (window->CanBeDrawnOnTop())
			window->UpdateWindow();
		else
			switch (state)
			{
			case Editor::SCRIPTING_WINDOW:
				if (window == scriptCreation)
					window->UpdateWindow();
				break;
			case Editor::EDITOR_WINDOW:
				if (window != scriptCreation)
					window->UpdateWindow();
				break;
			default:
				break;
			}
	}

	if (state == Editor::EDITOR_WINDOW)
		menuBar->Update();

	if (ImGui::IsMouseReleased(0)) {

		ShyEditor::ResourcesManager::UnselectAsset();
	}

	// Rendering
	ImGui::Render();
	SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
}

void Editor::handleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		if (event.type == SDL_QUIT)
			exitEditor = true;

		else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
			exitEditor = true;

		for (auto window : windows)
			window->HandleInput(&event);
	}
}

void Editor::changeEditorState(const EDITOR_STATE& state) {

	switch (state) {
	case Editor::EDITOR_WINDOW:
		break;
	case Editor::SCRIPTING_WINDOW:
		scriptCreation->Load();
		break;
	default:
		break;
	}

	this->state = state;
}

void Editor::setProjectInfo(ShyEditor::ProjectInfo* pInfo) {
	this->projecInfo = pInfo;
}

ShyEditor::ProjectInfo& Editor::getProjectInfo() {
	return *projecInfo;
}

void Editor::addWindow(ShyEditor::Window* window) {
	windows.push_back(window);
}

void Editor::setScene(ShyEditor::Scene* scene) {
	this->scene = scene;
}

SDL_Renderer* Editor::getRenderer() {
	return renderer;
}

ImVec2 Editor::getMainWindowSize() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	return ImVec2(w, h);
}

ShyEditor::Scene* Editor::getScene() {
	return scene;
}

ShyEditor::MenuBar* Editor::getMenuBar() {
	return menuBar;
}

ShyEditor::Hierarchy* Editor::getHierarchy() {
	return hierarchy;
}

ShyEditor::FileExplorer* Editor::getFileExplorer() {
	return fileExplorer;
}

ShyEditor::ComponentWindow* Editor::getComponents() {
	return components;
}

ShyEditor::ScriptCreation* Editor::getScriptCreation() {
	return scriptCreation;
}

ShyEditor::Console* Editor::getConsole()
{
	return console;
}

void Editor::OpenScript(const std::string& script) {
	scriptCreation->SetName(script);
	changeEditorState(EDITOR_STATE::SCRIPTING_WINDOW);
}

