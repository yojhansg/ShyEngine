#include "Scene.h"
#include "SDL.h"
#include "GameObject.h"
#include "Window.h"
#include "imgui.h"
#include "Camera.h"
#include "ImGUIManager.h"
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include "Hierarchy.h"
#include "FileExplorer.h"
#include "Components.h"

bool PEditor::Scene::mouseInsideWindow(ImVec2 mousePos)
{
	float mouseX = mousePos.x;
	float mouseY = mousePos.y;

	return (mouseX > windowPosX && mouseX < windowPosX + windowWidth && mouseY > windowPosY + 15 && mouseY < windowPosY + windowHeight);
}

bool PEditor::Scene::mouseInsideGameObject(GameObject* go, ImVec2 mousePos)
{

	mousePos = getMousePosInsideScene(mousePos);

	ImVec2 gOPos = go->getPosition();

	return (mousePos.x > gOPos.x && mousePos.x < gOPos.x + go->getWidth() && mousePos.y > gOPos.y && mousePos.y < gOPos.y + go->getHeight());
}

ImVec2 PEditor::Scene::getMousePosInsideScene(ImVec2 mousePos)
{
	//In case the editor is rescaled we need this
	float windowScaleFactor = (float)windowWidth / windowOriWidth;

	//The gameFrame size and position inside the ImGUI window
	ImVec2 framePosition = ImVec2(camera->getPosition().x * camera->getScrollFactor() * windowScaleFactor, camera->getPosition().y * camera->getScrollFactor() * windowScaleFactor);
	float frameWidth = gameSizeX * camera->getScrollFactor() * windowScaleFactor;
	float frameHeight = gameSizeY * camera->getScrollFactor() * windowScaleFactor;

	//Mouse position inside the IMGUIWindow
	float mouseX = (mousePos.x - windowPosX - 8 - framePosition.x) / frameWidth * gameSizeX;
	float mouseY = (mousePos.y - windowPosY - 27 - framePosition.y) / frameHeight * gameSizeY;

	return ImVec2(mouseX, mouseY);
}

bool PEditor::Scene::compareGameObjectsRenderOrder(GameObject* a, GameObject* b)
{
	return a->getRenderOrder() < b->getRenderOrder();
}

PEditor::Scene::Scene() : Window("Scene", NoMove | NoResize | NoCollapse | NoScrollbar | NoScrollWithMouse
	| ImGuiWindowFlags_NoBringToFrontOnFocus)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	imGUIManager->setScene(this);

	gameSizeX = imGUIManager->getGameSize().x;
	gameSizeY = imGUIManager->getGameSize().y;

	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	windowOriWidth = mainWindowSize.x * SCENE_WIN_WIDTH_RATIO;
	windowOriHeight = mainWindowSize.y * SCENE_WIN_HEIGHT_RATIO;

	float menuBarHeight = ImGui::GetFrameHeight();

	windowOriPosX = SDL_WIN_WIDTH / 2 - windowOriWidth / 2;
	windowOriPosY = menuBarHeight;

	setSize(ImVec2(windowOriWidth, windowOriHeight));
	setPosition(ImVec2(windowOriPosX, windowOriPosY));


	camera = new Camera(ImVec2(50, 50), 0.5);

	renderer = imGUIManager->getRenderer();

	targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowOriWidth, windowOriHeight);

	selectedGameObject = nullptr;
}

PEditor::Scene::~Scene()
{

	SDL_DestroyTexture(targetTexture);

	for (const auto& pair : gameObjects) {
		delete pair.second;
	}
	gameObjects.clear();

	delete camera;
}

void PEditor::Scene::addGameObject(std::string path)
{
	GameObject* go = (new PEditor::GameObject(path));
	gameObjects.emplace(go->getId(), go);
}

std::unordered_map<int, PEditor::GameObject*> PEditor::Scene::getGameObjects()
{
	return gameObjects;
}

PEditor::GameObject* PEditor::Scene::getSelectedGameObject()
{
	return selectedGameObject;
}

void PEditor::Scene::setSelectedGameObject(GameObject* go)
{
	selectedGameObject = go;
}


void PEditor::Scene::renderChildGameObjects(GameObject* go)
{
	for (auto pair : go->getChildren()) {
		renderChildGameObjects(pair.second);
		pair.second->render(renderer, camera);
	}
}

void PEditor::Scene::renderGameObjects()
{
	std::vector<GameObject*> sortedGameObjects;
	for (const auto& pair : gameObjects) {
		if (pair.second->getParent() == nullptr)
			sortedGameObjects.push_back(pair.second);
	}

	std::sort(sortedGameObjects.begin(), sortedGameObjects.end(), compareGameObjectsRenderOrder);

	for (auto gO : sortedGameObjects) {
		renderChildGameObjects(gO);
		gO->render(renderer, camera);
	}
}

void PEditor::Scene::renderFrame()
{
	//SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	ImVec2 position = ImVec2(camera->getPosition().x * camera->getScrollFactor(), camera->getPosition().y * camera->getScrollFactor());

	float width = gameSizeX * camera->getScrollFactor();
	float height = gameSizeY * camera->getScrollFactor();

	SDL_Rect frameRect = { position.x, position.y, width, height };
	SDL_RenderDrawRect(renderer, &frameRect);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void PEditor::Scene::saveScene()
{
	nlohmann::ordered_json j;

	j = j.parse(toJson());

	std::string path = "Scenes/scene.json";

	std::ofstream outputFile(path);
	if (outputFile.is_open()) {
		outputFile << j.dump(4);
		outputFile.close();
	}
	else {
		//ERROR HANDLING
	}

}

void PEditor::Scene::loadScene() {

	// TODO Abrir explorador de archivos para seleccionar scene.json

}

void PEditor::Scene::update()
{

	auto it = gameObjects.begin();
	while (it != gameObjects.end()) {
		GameObject* currentGameObject = it->second;

		currentGameObject->update();

		if (currentGameObject->isWaitingToDelete()) {
			selectedGameObject = nullptr;

			delete currentGameObject;
			it = gameObjects.erase(it);
		}
		else {
			++it;
		}
	}
}

void PEditor::Scene::handleInput(SDL_Event* event)
{
	ImVec2 mousePos = ImGui::GetMousePos();

	for (const auto& pair : gameObjects) {
		if (mouseInsideWindow(mousePos))
			pair.second->handleInput(event, mouseInsideGameObject(pair.second, mousePos), getMousePosInsideScene(mousePos));
	}

	if (!(SDL_GetModState() & KMOD_SHIFT)) {
		camera->handleInput(event, mouseInsideWindow(mousePos));
	}

}

void PEditor::Scene::render()
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	PEditor::FileExplorer* fileExplorer = imGUIManager->getFileExplorer();

	ImVec2 hierarchyWindowSize = imGUIManager->getHierarchy()->getSize();
	ImVec2 componentsWindowPos = imGUIManager->getComponents()->getPosition();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImGui::SetNextWindowSizeConstraints(ImVec2(componentsWindowPos.x - hierarchyWindowSize.x, mainWindowSize.y - fileExplorer->getSize().y - 24), ImVec2(componentsWindowPos.x - hierarchyWindowSize.x, mainWindowSize.y - fileExplorer->getSize().y - 24));

	ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);

	ImVec2 imGUIWindowSize = ImGui::GetWindowSize();
	ImVec2 imGUIWindowPos = ImGui::GetWindowPos();
	windowWidth = imGUIWindowSize.x;
	windowHeight = imGUIWindowSize.y;
	windowPosX = imGUIWindowPos.x;
	windowPosY = imGUIWindowPos.y;

	float menuBarHeight = ImGui::GetFrameHeight();

	ImGui::SetWindowPos(ImVec2(hierarchyWindowSize.x, menuBarHeight));

	SDL_SetRenderTarget(renderer, targetTexture);
	SDL_RenderClear(renderer);

	renderGameObjects();
	renderFrame();

	SDL_SetRenderTarget(renderer, NULL);


	ImGui::Image(targetTexture, ImVec2(windowWidth, windowOriHeight * windowWidth / windowOriWidth));

	ImGui::End();

}

std::string PEditor::Scene::toJson()
{
	nlohmann::ordered_json j;

	j["name"] = "scene";

	nlohmann::ordered_json gameObjectsJson = nlohmann::json::array();
	for (const auto& pair : gameObjects) {
		if (pair.second->getParent() == nullptr)
			gameObjectsJson.push_back(j.parse(pair.second->toJson()));
	}

	j["objects"] = gameObjectsJson;

	return j.dump(2);

}