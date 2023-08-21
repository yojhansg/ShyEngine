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
#include "ScriptCreationUtilities.h"

#include <string>
#include <fstream>


namespace ShyEditor {



	Scene::Scene() : Window("Scene", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)
	{
		Editor* editor = Editor::getInstance();
		editor->setScene(this);

		ImVec2 mainWindowSize = editor->getMainWindowSize();

		float menuBarHeight = ImGui::GetFrameHeight();

		scenePath = "Scenes/" + Preferences::GetData().initialScene + ".scene";

		renderer = editor->getRenderer();

		camera = new Camera(
			ImVec2(0, 0), 1, renderer);


		camera->SetConstrains(.1, 10.0);

		selectedGameObject = nullptr;

		if (std::filesystem::exists(scenePath))
			loadScene(scenePath);

		docked = true;
	}


	Scene::~Scene()
	{
		
		for (const auto& pair : gameObjects) {
			delete pair.second;
		}
		gameObjects.clear();

		delete camera;
	}


	void Scene::Behaviour()
	{
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

			ImVec2 max = ImVec2(windowWidth + windowPosX, windowPosY + windowHeight);

			if (ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max)) {
				auto& asset = ResourcesManager::SelectedAsset();

				if (asset.extension == ".png" || asset.extension == ".jpg") {

					std::cout << "TODO: arrastrar un objeto";
					//GameObject* go = editor->getScene()->AddGameObject(asset.relativePath);

					//ImVec2 position = getMousePosInsideScene(ImGui::GetMousePos());
					//position.x -= Preferences::GetData().width * 0.5f;
					//position.y -= Preferences::GetData().height * 0.5f;

					//go->setPosition(position);
					//selectedGameObject = go;
				}
			}
		}

		camera->PrepareCameraRender();

		RenderGameObjects();
		RenderFrame();

		camera->StopCameraRender();

		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Image(camera->GetTexture(), ImVec2(windowWidth, windowHeight));


		//cambiar las propiedades del grid dependiendo del orden de magnitud


		float spacing = 50 * camera->GetScale();
		int interval = 5;

		ScriptCreationUtilities::Grid::SetSpacing(spacing);
		ScriptCreationUtilities::Grid::SetInterval(interval);
		ScriptCreationUtilities::Grid::SetOffset(camera->GetPosition().x, camera->GetPosition().y);
		ScriptCreationUtilities::Grid::Draw();


		ImGui::SetCursorPos(ImVec2(10, ImGui::GetFrameHeight() + 10));
		ImGui::SliderFloat("Zoom (-/+)", &camera->GetScale(), camera->GetMinScale(), camera->GetMaxScale(), "%.3f", ImGuiSliderFlags_Logarithmic);
	}


	bool Scene::CompareGameObjectsRenderOrder(GameObject* a, GameObject* b)
	{
		return a->getRenderOrder() < b->getRenderOrder();
	}


	GameObject* Scene::AddGameObject(std::string path)
	{
		GameObject* go = (new GameObject(path));
		gameObjects.emplace(go->getId(), go);
		return go;
	}

	void Scene::AddGameObject(GameObject* go)
	{
		gameObjects.emplace(go->getId(), go);
	}

	std::unordered_map<int, GameObject*>& Scene::getGameObjects()
	{
		return gameObjects;
	}

	GameObject* Scene::GetSelectedGameObject()
	{
		return selectedGameObject;
	}

	void Scene::SetSelectedGameObject(GameObject* go)
	{
		selectedGameObject = go;
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

		ImVec2 position = ImVec2(camera->GetPosition().x * camera->GetScale(), camera->GetPosition().y * camera->GetScale());

		float gameSizeX = Preferences::GetData().width;
		float gameSizeY = Preferences::GetData().height;

		float width = gameSizeX * camera->GetScale();
		float height = gameSizeY * camera->GetScale();

		SDL_Rect frameRect = { position.x, position.y, width, height };
		SDL_RenderDrawRect(renderer, &frameRect);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	void Scene::RenderUI()
	{
	}

	void Scene::saveScene(std::string path)
	{
		nlohmann::ordered_json j;

		j = j.parse(toJson());

		this->scenePath = path;

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
		bool insideWindow = IsMouseHoveringWindow();
		bool anyGoSelected = false;
		for (const auto& pair : gameObjects) {

			//if (insideWindow)
			//	anyGoSelected |= pair.second->handleInput(event, mouseInsideGameObject(pair.second, mousePos), getMousePosInsideScene(mousePos));
		}

		if (insideWindow && !anyGoSelected && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			SetSelectedGameObject(nullptr);
		}

		if (!(SDL_GetModState() & KMOD_SHIFT)) {
			camera->handleInput(event, insideWindow);
		}

	}

	std::string Scene::getPath()
	{
		return scenePath;
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

		this->scenePath = path;

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