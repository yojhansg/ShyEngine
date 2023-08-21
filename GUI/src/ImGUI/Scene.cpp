#include "Scene.h"

#include <nlohmann/json.hpp>
#include "ProjectsManager.h"
#include "FileExplorer.h"
#include "Preferences.h"
#include "Components.h"
#include "GameObject.h"
#include "Hierarchy.h"
#include "Window.h"
#include "Editor.h"
#include "Camera.h"
#include "imgui.h"
#include "SDL.h"
#include "ResourcesManager.h"

#include <string>
#include <fstream>


namespace ShyEditor {

	bool Scene::mouseInsideWindow(ImVec2 mousePos)
	{
		float mouseX = mousePos.x;
		float mouseY = mousePos.y;

		return (mouseX > windowPosX && mouseX < windowPosX + windowWidth && mouseY > windowPosY + 15 && mouseY < windowPosY + windowHeight);
	}

	bool Scene::mouseInsideGameObject(GameObject* go, ImVec2 mousePos)
	{

		mousePos = getMousePosInsideScene(mousePos);

		ImVec2 gOPos = go->getAdjustedPosition();

		return (mousePos.x > gOPos.x && mousePos.x < gOPos.x + go->getSize().x * go->getScale_x() && mousePos.y > gOPos.y && mousePos.y < gOPos.y + go->getSize().y * go->getScale_y());
	}

	ImVec2 Scene::getMousePosInsideScene(ImVec2 mousePos)
	{
		float gameSizeX = Preferences::GetData().width;
		float gameSizeY = Preferences::GetData().height;

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

	bool Scene::compareGameObjectsRenderOrder(GameObject* a, GameObject* b)
	{
		return a->getRenderOrder() < b->getRenderOrder();
	}

	Scene::Scene() : Window("Scene",NoCollapse | NoScrollbar | NoScrollWithMouse)
	{
		Editor* editor = Editor::getInstance();
		editor->setScene(this);

		ImVec2 mainWindowSize = editor->getMainWindowSize();

		windowOriWidth = mainWindowSize.x * SCENE_WIN_WIDTH_RATIO;
		windowOriHeight = mainWindowSize.y * SCENE_WIN_HEIGHT_RATIO;

		float menuBarHeight = ImGui::GetFrameHeight();

		windowOriPosX = TARGET_WIDTH / 2 - windowOriWidth / 2;
		windowOriPosY = menuBarHeight;

		setSize(ImVec2(windowOriWidth, windowOriHeight));
		setPosition(ImVec2(windowOriPosX, windowOriPosY));

		path = "Scenes/" + Preferences::GetData().initialScene + ".scene";


		float scrollFactor = 1.f;
		camera = new Camera(
			ImVec2(
				(windowOriWidth - Preferences::GetData().width * scrollFactor) * 0.5f,
				(windowOriHeight - Preferences::GetData().height * scrollFactor) * 0.5f
			), scrollFactor);

		renderer = editor->getRenderer();

		targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowOriWidth, windowOriHeight);

		selectedGameObject = nullptr;

		if (std::filesystem::exists(path))
			loadScene(path);

		docked = true;
	}

	Scene::~Scene()
	{

		SDL_DestroyTexture(targetTexture);

		for (const auto& pair : gameObjects) {
			delete pair.second;
		}
		gameObjects.clear();

		delete camera;
	}

	GameObject* Scene::addGameObject(std::string path)
	{
		GameObject* go = (new GameObject(path));
		gameObjects.emplace(go->getId(), go);
		return go;
	}

	void Scene::addGameObject(GameObject* go)
	{
		gameObjects.emplace(go->getId(), go);
	}

	std::unordered_map<int, GameObject*> Scene::getGameObjects()
	{
		return gameObjects;
	}

	GameObject* Scene::getSelectedGameObject()
	{
		return selectedGameObject;
	}

	void Scene::setSelectedGameObject(GameObject* go)
	{
		selectedGameObject = go;
	}


	void Scene::renderChildGameObjects(GameObject* go)
	{
		for (auto pair : go->getChildren()) {
			renderChildGameObjects(pair.second);
			pair.second->render(renderer, camera);
		}
	}

	void Scene::renderGameObjects()
	{
		std::vector<GameObject*> sortedGameObjects;
		for (const auto& pair : gameObjects) {
			if (pair.second->getParent() == nullptr)
				sortedGameObjects.push_back(pair.second);
		}

		std::sort(sortedGameObjects.begin(), sortedGameObjects.end(), compareGameObjectsRenderOrder);

		for (auto gO : sortedGameObjects) {
			gO->render(renderer, camera);
			renderChildGameObjects(gO);
		}
	}

	void Scene::renderFrame()
	{
		// SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		ImVec2 position = ImVec2(camera->getPosition().x * camera->getScrollFactor(), camera->getPosition().y * camera->getScrollFactor());

		float gameSizeX = Preferences::GetData().width;
		float gameSizeY = Preferences::GetData().height;

		float width = gameSizeX * camera->getScrollFactor();
		float height = gameSizeY * camera->getScrollFactor();

		SDL_Rect frameRect = { position.x, position.y, width, height };
		SDL_RenderDrawRect(renderer, &frameRect);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	void Scene::saveScene(std::string path)
	{
		nlohmann::ordered_json j;

		j = j.parse(toJson());

		this->path = path;

		std::ofstream outputFile(Editor::getInstance()->getProjectInfo().path + "/Scenes/scene.scene");
		if (outputFile.is_open()) {
			outputFile << j.dump(4);
			outputFile.close();
		}
		else {
			//ERROR HANDLING

			std::cout << "Error al guardar la escena" << std::endl;

		}

	}


	void Scene::HandleInput(SDL_Event* event)
	{
		ImVec2 mousePos = ImGui::GetMousePos();

		bool insideWindow = mouseInsideWindow(mousePos);
		bool anyGoSelected = false;
		for (const auto& pair : gameObjects) {
			if (insideWindow)
				anyGoSelected |= pair.second->handleInput(event, mouseInsideGameObject(pair.second, mousePos), getMousePosInsideScene(mousePos));
		}

		if (insideWindow && !anyGoSelected && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			setSelectedGameObject(nullptr);
		}

		if (!(SDL_GetModState() & KMOD_SHIFT)) {
			camera->handleInput(event, insideWindow);
		}

	}

	void Scene::Behaviour()
	{
		auto it = gameObjects.begin();
		while (it != gameObjects.end()) {
			GameObject* go = it->second;

			go->update();

			if (go->isWaitingToDelete()) {
				selectedGameObject = nullptr;

				delete go;
				it = gameObjects.erase(it);
			}
			else {
				++it;
			}
		}


		Editor* editor = Editor::getInstance();
		FileExplorer* fileExplorer = editor->getFileExplorer();

		ImVec2 hierarchyWindowSize = editor->getHierarchy()->getSize();
		ImVec2 componentsWindowPos = editor->getComponents()->getPosition();
		ImVec2 mainWindowSize = editor->getMainWindowSize();

		//ImGui::SetNextWindowSizeConstraints(ImVec2(componentsWindowPos.x - hierarchyWindowSize.x, mainWindowSize.y - fileExplorer->getSize().y - 24), ImVec2(componentsWindowPos.x - hierarchyWindowSize.x, mainWindowSize.y - fileExplorer->getSize().y - 24));

		size_t lastPathSlashPosition = path.find_last_of("/\\");
		size_t lastPathDotPosition = path.find_last_of(".");

		std::string sceneFilename = path.substr(lastPathSlashPosition + 1, lastPathDotPosition - lastPathSlashPosition - 1);

		if (ResourcesManager::IsAnyAssetSelected() && ImGui::IsMouseReleased(0)) {

			ImVec2 max = ImVec2(windowWidth + windowPosX, windowPosY + windowHeight);

			if (ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max)) {
				auto& asset = ResourcesManager::SelectedAsset();

				if (asset.extension == ".png" || asset.extension == ".jpg") {

					GameObject* go = editor->getScene()->addGameObject(asset.relativePath);

					ImVec2 position = getMousePosInsideScene(ImGui::GetMousePos());
					position.x -= Preferences::GetData().width * 0.5f;
					position.y -= Preferences::GetData().height * 0.5f;

					go->setPosition(position);
					selectedGameObject = go;
				}
			}
		}


		SDL_SetRenderTarget(renderer, targetTexture);
		SDL_RenderClear(renderer);

		renderGameObjects();
		renderFrame();

		SDL_SetRenderTarget(renderer, NULL);

		ImGui::Image(targetTexture, ImVec2(windowWidth, windowOriHeight * windowWidth / windowOriWidth));
	}

	std::string Scene::getPath()
	{
		return path;
	}

	std::string Scene::toJson()
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

	void Scene::loadScene(std::string path) {
		//Delete info of previous scene;
		selectedGameObject = nullptr;

		for (const auto& pair : gameObjects) {
			delete pair.second;
		}
		gameObjects.clear();

		this->path = path;

		std::ifstream inputFile(Editor::getInstance()->getProjectInfo().path + "/Scenes/scene.scene");

		if (!inputFile.is_open()) {

			return;
		}

		nlohmann::ordered_json jsonData;
		try {
			inputFile >> jsonData;
		}
		catch (const nlohmann::json::parse_error& e) {
			std::cerr << "JSON parse error: " << e.what() << std::endl;
			return;
		}

		inputFile.close();

		//Add scene name property

		nlohmann::json gameObjectsJson = jsonData["objects"];

		// Iterate through the game objects JSON array
		for (const auto& gameObjectJson : gameObjectsJson) {
			GameObject* gameObject = GameObject::fromJson(gameObjectJson.dump());

			gameObjects.insert({ gameObject->getId(), gameObject });
		}

	}

}