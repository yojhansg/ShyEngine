#include "Scene.h"

#include "ScriptCreationUtilities.h"
#include "ResourcesManager.h"
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

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>


namespace ShyEditor {

	Scene::Scene() : Window("Scene", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse) {

		Editor* editor = Editor::getInstance();
		editor->setScene(this);

		renderer = editor->getRenderer();

		// Scene path and name
		scenePath = ResourcesManager::ASSETSFOLDER + "Scenes\\" + Preferences::GetData().initialScene + ".scene";
		sceneName = "Scene";
		name = sceneName.c_str();

		camera = new Camera(ImVec2(0, 0), 1, renderer);
		camera->SetConstrains(.1, 10.0);

		// Pointer to the selected gameobject in the scene
		selectedGameObject = nullptr;

		docked = true;
		viewMode = 0;

		// Load the default scene or the last opene scene
		loadScene(sceneName);

	}

	Scene::~Scene() {

		// Delete gameobjects in the scene
		for (const auto& pair : gameObjects)
			delete pair.second;

		gameObjects.clear();

		delete camera;
	}

	GameObject* Scene::AddGameObject(std::string path) {

		GameObject* go = new GameObject(path);
		gameObjects.emplace(go->getId(), go);

		return go;
	}

	void Scene::AddGameObject(GameObject* go) {
		gameObjects.emplace(go->getId(), go);
	}

	std::unordered_map<int, GameObject*>& Scene::getGameObjects() {
		return gameObjects;
	}

	std::string Scene::getSceneName() {
		return sceneName;
	}

	GameObject* Scene::GetSelectedGameObject() {
		return selectedGameObject;
	}

	void Scene::SetSelectedGameObject(GameObject* go) {
		selectedGameObject = go;
	}

	void Scene::saveScene(const std::string& sceneName) {

		nlohmann::ordered_json j;

		this->scenePath = sceneName + ".scene";
		this->sceneName = sceneName;
		name = this->sceneName.c_str();

		j = j.parse(toJson());

		std::ofstream outputFile(Editor::getInstance()->getProjectInfo().path + ResourcesManager::ASSETSFOLDER + scenePath);
		if (outputFile.is_open()) {
			outputFile << j.dump(4);
			outputFile.close();
		}
		else {

		}

	}

	void Scene::loadScene(const std::string& sceneName) {

		// Delete info of previous scene
		selectedGameObject = nullptr;

		for (const auto& pair : gameObjects)
			delete pair.second;

		gameObjects.clear();

		this->scenePath = sceneName + ".scene";
		this->sceneName = sceneName;
		name = this->sceneName.c_str();

		std::ifstream inputFile(Editor::getInstance()->getProjectInfo().path + ResourcesManager::ASSETSFOLDER + scenePath);

		if (!inputFile.is_open()) {

			return;
		}

		nlohmann::ordered_json jsonData;
		inputFile >> jsonData;
		inputFile.close();

		nlohmann::json gameObjectsJson = jsonData["objects"];

		// Iterate through the game objects JSON array
		for (const auto& gameObjectJson : gameObjectsJson) {
			GameObject* gameObject = GameObject::fromJson(gameObjectJson.dump());
			gameObjects.insert({ gameObject->getId(), gameObject });
		}

	}

	void Scene::RenderChildGameObjects(GameObject* go)
	{
		for (auto pair : go->getChildren()) {
			RenderChildGameObjects(pair.second);
			pair.second->render(renderer, camera);
		}
	}

	void Scene::RenderGameObjects()
	{
		std::vector<GameObject*> sortedGameObjects;
		for (const auto& pair : gameObjects) {
			if (pair.second->getParent() == nullptr)
				sortedGameObjects.push_back(pair.second);
		}

		std::sort(sortedGameObjects.begin(), sortedGameObjects.end(), CompareGameObjectsRenderOrder);

		for (auto gO : sortedGameObjects) {
			gO->render(renderer, camera);
			RenderChildGameObjects(gO);
		}
	}

	void Scene::RenderFrame()
	{
		// SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		ImVec2 position = camera->GetPosition();

		float gameSizeX = Preferences::GetData().width;
		float gameSizeY = Preferences::GetData().height;

		float width = gameSizeX * camera->GetScale();
		float height = gameSizeY * camera->GetScale();


		camera->CenterPosition(position.x, position.y);

		SDL_Rect frameRect = { position.x, position.y, width, height };

		int lineThickness = 10;

		for (int i = 0; i < lineThickness; i++) //SDL no tiene soporte para cambiar el grosor de la linea
		{

			SDL_RenderDrawRect(renderer, &frameRect);

			frameRect.x++;
			frameRect.y++;
			frameRect.w -= 2;
			frameRect.h -= 2;
		}

		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	void Scene::RenderUI() {

	}

	void Scene::HandleInput(SDL_Event* event) {

		ImVec2 mousePos = ImGui::GetMousePos();
		bool insideWindow = IsMouseHoveringWindow();
		bool anyGoSelected = false;

		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
		{
			for (const auto& pair : gameObjects) {

				if (IsMouseHoveringGameObject(pair.second)) {

					anyGoSelected = true;
					selectedGameObject = pair.second;
				}
			}
		}

		if (insideWindow && !anyGoSelected && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			SetSelectedGameObject(nullptr);
		}

		if (!(SDL_GetModState() & KMOD_SHIFT)) {
			camera->handleInput(event, insideWindow);
		}

	}

	void Scene::Behaviour() {

		if (camera->ShouldResize(windowWidth, windowHeight))
			camera->Resize(windowWidth, windowHeight);

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

		if (ResourcesManager::IsAnyAssetSelected() && ImGui::IsMouseReleased(0)) {

			if (IsMouseHoveringWindow()) {
				auto& asset = ResourcesManager::SelectedAsset();

				if (asset.extension == ".png" || asset.extension == ".jpg") {

					GameObject* go = AddGameObject(asset.relativePath);
					go->setName(asset.name);

					ImVec2 position = MousePositionInScene();

					go->setPosition(position);
					selectedGameObject = go;
				}
			}
		}

		camera->PrepareCameraRender();

		switch (viewMode) {

			case 0:

				RenderGameObjects();
				RenderUI();
				break;
			case 1:

				RenderGameObjects();
				RenderFrame();

				break;
			case 2:

				RenderUI();

				break;

			default:
				break;
		}

		camera->StopCameraRender();


		// Cambiar las propiedades del grid dependiendo del orden de magnitud

		float spacing = 50 * camera->GetScale();
		int interval = 5;

		int offset_x = windowWidth * 0.5f;
		int offset_y = windowHeight * 0.5f;


		ScriptCreationUtilities::Grid::SetSpacing(spacing);
		ScriptCreationUtilities::Grid::SetInterval(interval);
		ScriptCreationUtilities::Grid::SetOffset(offset_x + camera->GetPosition().x, offset_y + camera->GetPosition().y);
		ScriptCreationUtilities::Grid::Draw();


		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Image(camera->GetTexture(), ImVec2(windowWidth, windowHeight));

		ImGui::SetCursorPos(ImVec2(10, ImGui::GetFrameHeight() + 10));
		ImGui::Text(name);

		ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(100, ImGui::GetFrameHeight() + 10));
		ImGui::SliderFloat("Zoom (-/+)", &camera->GetScale(), camera->GetMinScale(), camera->GetMaxScale(), "%.3f", ImGuiSliderFlags_Logarithmic);

		ImGui::SameLine();

		ImGui::RadioButton("##Scene view - Both", &viewMode, 0);
		ImGui::SameLine();
		ImGui::RadioButton("##Scene view - World", &viewMode, 1);
		ImGui::SameLine();
		ImGui::RadioButton("##Scene view - UI", &viewMode, 2);

	}

	ImVec2 Scene::MousePositionInScene() {

		ImVec2 mousepos = ImGui::GetMousePos();

		mousepos.x -= windowPosX;
		mousepos.y -= windowPosY;

		ImVec2 cameraPosition = camera->GetPosition();

		mousepos.x -= cameraPosition.x;
		mousepos.y -= cameraPosition.y;

		//Trasladar el origen de coordenadas de la esquina superior al centro
		mousepos.x -= windowWidth * 0.5f;
		mousepos.y -= windowHeight * 0.5f;

		return mousepos;
	}

	bool Scene::IsMouseHoveringGameObject(GameObject* gameObject) {

		auto mousePos = MousePositionInScene();

		auto goPos = gameObject->getAdjustedPosition();
		auto goSize = gameObject->getSize();

		goSize.x *= gameObject->getScale_x();
		goSize.y *= gameObject->getScale_y();

		return mousePos.x > goPos.x && mousePos.x < goPos.x + goSize.x &&
			mousePos.y > goPos.y && mousePos.y < goPos.y + goSize.y;
	}

	bool Scene::CompareGameObjectsRenderOrder(GameObject* a, GameObject* b) {
		return a->getRenderOrder() < b->getRenderOrder();
	}

	std::string Scene::getPath() {
		return scenePath;
	}

	std::string Scene::toJson() {

		nlohmann::ordered_json j;

		j["name"] = sceneName;

		nlohmann::ordered_json gameObjectsJson = nlohmann::json::array();
		for (const auto& pair : gameObjects) {
			if (pair.second->getParent() == nullptr)
				gameObjectsJson.push_back(j.parse(pair.second->toJson()));
		}

		j["objects"] = gameObjectsJson;

		return j.dump(2);
	}

}