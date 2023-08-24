#include "Scene.h"

#include "ScriptCreationUtilities.h"
#include "ResourcesManager.h"
#include "ProjectsManager.h"
#include "FileExplorer.h"
#include "Preferences.h"
#include "Components.h"
#include "LogManager.h"
#include "GameObject.h"
#include "Hierarchy.h"
#include "Window.h"
#include "Editor.h"
#include "Camera.h"
#include "imgui.h"
#include "PrefabManager.h"
#include "SDL.h"

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

#include "CheckML.h"


namespace ShyEditor {

	Scene::Scene() : Window("Scene", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse) {

		Editor* editor = Editor::getInstance();
		editor->setScene(this);

		renderer = editor->getRenderer();

		// Scene path and name
		scenePath = ResourcesManager::EDITORASSETSFOLDER + "\\Scenes\\" + Preferences::GetData().initialScene + ".scene";
		sceneName = "Scene";
		name = sceneName.c_str();

		sceneCamera = new Camera(ImVec2(0, 0), 1, renderer);
		sceneCamera->SetConstrains(.1, 10.0);

		uiTexture = nullptr;
		uiWidth = 0; uiHeight = 0;
		ResizeOverlayIfNeccesary();

		// Pointer to the selected gameobject in the scene
		selectedGameObject = nullptr;

		docked = true;
		viewMode = 0;

		acceptAssetDrop = true;

		// Load the default scene or the last opene scene
		loadScene(sceneName);

	}

	Scene::~Scene() {

		// Delete gameobjects in the scene
		for (const auto& pair : gameObjects)
			delete pair.second;

		gameObjects.clear();

		// Delete overlays in the scene
		for (const auto& o : overlays)
			delete o;

		overlays.clear();

		delete sceneCamera;
	}

	GameObject* Scene::AddGameObject(std::string path) {

		GameObject* go = new GameObject(path, true);
		gameObjects.emplace(go->getId(), go);

		return go;
	}

	void Scene::AddGameObject(GameObject* go) {
		gameObjects.emplace(go->getId(), go);
	}

	void Scene::AddChildsToScene(GameObject* go)
	{
		for (auto pair : go->getChildren()) {
			gameObjects.emplace(pair.second->getId(), pair.second);

			AddChildsToScene(pair.second);
		}
	}

	GameObject* Scene::AddOverlay(std::string path)
	{
		GameObject* go = new GameObject(path, false);
		overlays.push_back(go);

		return go;
	}

	std::unordered_map<int, GameObject*>& Scene::getGameObjects() {
		return gameObjects;
	}

	std::vector<GameObject*>& Scene::getOverlays()
	{
		return overlays;
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

		std::ofstream outputFile(Editor::getInstance()->getProjectInfo().path + "\\Assets\\" + scenePath);
		if (outputFile.is_open()) {
			outputFile << j.dump(4);
			outputFile.close();
		}
		else LogManager::LogError("Could not open the file to save the scene.");

	}

	void Scene::loadScene(const std::string& sceneName) {

		// Delete info of previous scene
		selectedGameObject = nullptr;

		for (const auto& pair : gameObjects)
			delete pair.second;

		gameObjects.clear();

		for (const auto& o : overlays)
			delete o;

		overlays.clear();

		this->scenePath = sceneName + ".scene";
		this->sceneName = sceneName;
		name = this->sceneName.c_str();

		std::ifstream inputFile(Editor::getInstance()->getProjectInfo().path + "\\Assets\\" + scenePath);

		if (!inputFile.is_open()) {
			LogManager::LogError("Could not open the file to load the scene.");
			return;
		}

		nlohmann::ordered_json jsonData;
		inputFile >> jsonData;
		inputFile.close();


		if (!jsonData.contains("objects")) {
			LogManager::LogError("The scene file has not the expected format.");
			return;
		}

		nlohmann::json gameObjectsJson = jsonData["objects"];

		// Iterate through the game objects JSON array
		for (const auto& gameObjectJson : gameObjectsJson) {
			GameObject* gameObject = GameObject::fromJson(gameObjectJson.dump(), true);
			gameObjects.insert({ gameObject->getId(), gameObject });
			AddChildsToScene(gameObject);
		}

		if (!jsonData.contains("overlays")) {
			LogManager::LogError("The scene file has not the expected format.");
			return;
		}

		nlohmann::json overlaysJson = jsonData["overlays"];

		// Iterate through the overlay objects JSON array
		for (const auto& overlayJson : overlaysJson) {
			GameObject* overlay = GameObject::fromJson(overlayJson.dump(), false);
			overlays.push_back(overlay);
			AddChildsToScene(overlay);
		}

	}

	void Scene::RenderChildGameObjects(GameObject* go)
	{
		for (auto& pair : go->getChildren()) {
			RenderChildGameObjects(pair.second);
			pair.second->RenderTransform(renderer, sceneCamera);
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
			gO->RenderTransform(renderer, sceneCamera);
			RenderChildGameObjects(gO);
		}
	}

	void Scene::RenderFrame()
	{
		// SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_Rect frameRect{};
		CalculateFrameRect(frameRect.x, frameRect.y, frameRect.w, frameRect.h);

		RenderRectangle(frameRect.x, frameRect.y, frameRect.w, frameRect.h, 10);

		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	void Scene::RenderUI() {

		if (uiTexture == nullptr) return;

		auto currentTarget = SDL_GetRenderTarget(renderer);
		SDL_SetRenderTarget(renderer, uiTexture);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(renderer);


		auto mouse = MousePositionInScene();

		float cameraScale = sceneCamera->GetScale();

		for (auto go : overlays) {

			Overlay* overlay = go->GetOverlay();


			//TODO: aplicar color y escala

			SDL_Rect dest{};
			overlay->CalculateRectangle(dest.x, dest.y, dest.w, dest.h);


			bool mouseclicked = ImGui::IsMouseClicked(0);

			const int thickness = 10;


			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			RenderRectangle(dest.x, dest.y, dest.w, dest.h, 5);

			overlay->Render(renderer, dest.x, dest.y, dest.w, dest.h);

			if (selectedOverlay.overlay == nullptr)
			{

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

				if (PointInsideHorizontalSegment(mouse.x, mouse.y, dest.x, dest.y, dest.w, thickness)) {

					RenderHorizontalSegment(dest.x, dest.y, dest.w, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_TOP;
				}

				if (PointInsideHorizontalSegment(mouse.x, mouse.y, dest.x, dest.y + dest.h, dest.w, thickness)) {

					RenderHorizontalSegment(dest.x, dest.y + dest.h, dest.w, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_BOTTOM;
				}

				if (PointInsideVerticalSegment(mouse.x, mouse.y, dest.x, dest.y, dest.h, thickness)) {

					RenderVerticalSegment(dest.x, dest.y, dest.h, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_LEFT;
				}

				if (PointInsideVerticalSegment(mouse.x, mouse.y, dest.x + dest.w, dest.y, dest.h, thickness)) {

					RenderVerticalSegment(dest.x + dest.w, dest.y, dest.h, thickness);
					if (mouseclicked) selectedOverlay.dir |= DIR_RIGHT;
				}
			}



			if (mouseclicked) {

				if (PointInsideRect(mouse.x, mouse.y, dest.x, dest.y, dest.w, dest.h, thickness)) {



					selectedGameObject = go;
					selectedOverlay.overlay = overlay;

					selectedOverlay.offset_x = dest.x - mouse.x;
					selectedOverlay.offset_y = dest.y - mouse.y;
				}
			}

		}

		SDL_SetRenderTarget(renderer, currentTarget);


		SDL_Rect frameRect = { };
		CalculateFrameRect(frameRect.x, frameRect.y, frameRect.w, frameRect.h);

		SDL_RenderCopy(renderer, uiTexture, NULL, &frameRect);






		if (ImGui::IsMouseReleased(0))
		{
			selectedOverlay.overlay = nullptr;
			selectedOverlay.dir = 0;
		}



		if (selectedOverlay.overlay != nullptr) {

			bool changeSize = selectedOverlay.dir != 0;
			bool isPositioned = selectedOverlay.overlay->GetPlacement() == 0;

			if (changeSize) {


				if (selectedOverlay.dir & DIR_TOP) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().y;
						float anchor = selectedOverlay.overlay->GetAnchor().y;
						float& size = selectedOverlay.overlay->GetSize().y;
						float mousePos = mouse.y;

						//Punto central del overlay para posicionarlo correctamente
						float centerOffset = anchor * size;

						//Guardamos la posicion previa del overlay
						float py = position;
						position = mousePos + centerOffset;

						//El incremento es la diferencia entre la posicion actual y la anterior
						float increment = py - position;

						//Para que parezca que se esta moviendo arrastrandolo de un lado, el incremento hay que multiplicarlo por la posicion de anclaje
						position += increment * anchor;

						//Lo que hayamos aumentado de posicion hay que reducirlo de tamaño y viceversa
						size += increment;
					}
					else
						selectedOverlay.overlay->GetTop() = mouse.y;
				}

				if (selectedOverlay.dir & DIR_LEFT) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().x;
						float anchor = selectedOverlay.overlay->GetAnchor().x;
						float& size = selectedOverlay.overlay->GetSize().x;
						float mousePos = mouse.x;


						//Punto central del overlay para posicionarlo correctamente
						float centerOffset = anchor * size;

						//Guardamos la posicion previa del overlay
						float py = position;
						position = mousePos + centerOffset;

						//El incremento es la diferencia entre la posicion actual y la anterior
						float increment = py - position;

						//Para que parezca que se esta moviendo arrastrandolo de un lado, el incremento hay que multiplicarlo por la posicion de anclaje
						position += increment * anchor;

						//Lo que hayamos aumentado de posicion hay que reducirlo de tamaño y viceversa
						size += increment;
					}
					else
						selectedOverlay.overlay->GetLeft() = mouse.x;
				}

				if (selectedOverlay.dir & DIR_RIGHT) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().x;
						float anchor = selectedOverlay.overlay->GetAnchor().x;
						float& size = selectedOverlay.overlay->GetSize().x;
						float mousePos = mouse.x;

						float centerOffset = anchor * size;

						float psize = size;

						//Calculo las dimensiones del elemento a partir de la esquina superior izquierda al raton
						size = mousePos - (position - centerOffset);

						float increment = psize - size;

						//Lo que haya incrementado de tamaño hay que compensarlo con la posicion, para de esta forma
						//dar la sensacion de que estamos arrastrando desde un lado
						position -= anchor * increment;
					}
					else
						selectedOverlay.overlay->GetRight() = Preferences::GetData().width - mouse.x;
				}

				if (selectedOverlay.dir & DIR_BOTTOM) {

					if (isPositioned)
					{
						float& position = selectedOverlay.overlay->GetPosition().y;
						float anchor = selectedOverlay.overlay->GetAnchor().y;
						float& size = selectedOverlay.overlay->GetSize().y;
						float mousePos = mouse.y;

						float centerOffset = anchor * size;

						float psize = size;

						//Calculo las dimensiones del elemento a partir de la esquina superior izquierda al raton
						size = mousePos - (position - centerOffset);

						float increment = psize - size;

						//Lo que haya incrementado de tamaño hay que compensarlo con la posicion, para de esta forma
						//dar la sensacion de que estamos arrastrando desde un lado
						position -= anchor * increment;
					}
					else
						selectedOverlay.overlay->GetBottom() = Preferences::GetData().height - mouse.y;
				}


			}
			else {


				if (isPositioned) {

					auto& position = selectedOverlay.overlay->GetPosition();
					auto& anchor = selectedOverlay.overlay->GetAnchor();
					auto& size = selectedOverlay.overlay->GetSize();


					position.x = mouse.x + selectedOverlay.offset_x + anchor.x * size.x;
					position.y = mouse.y + selectedOverlay.offset_y + anchor.y * size.y;
				}

				else {

					int& left = selectedOverlay.overlay->GetLeft();
					int& right = selectedOverlay.overlay->GetRight();
					int& top = selectedOverlay.overlay->GetTop();
					int& bottom = selectedOverlay.overlay->GetBottom();


					int pleft = left;
					int ptop = top;

					left = mouse.x + selectedOverlay.offset_x;
					top = mouse.y + selectedOverlay.offset_y;

					right += pleft - left;
					bottom += ptop - top;
				}
			}

		}

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
					dragging = true;
				}

			}
		}

		if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
		{
			dragging = false;
		}

		if (dragging && selectedGameObject != nullptr && event->type == SDL_MOUSEMOTION) {
			
			float invCameraScale = 1.f / sceneCamera->GetScale();

			if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {

				const float incrementSpeed = .3f;

				float r = selectedGameObject->getRotation();

				r += event->motion.xrel * incrementSpeed;

				selectedGameObject->SetRotation(r);
			}
			else if (ImGui::IsKeyDown(ImGuiKey_LeftAlt)) {

				const float incrementSpeed = .03f;

				float x = selectedGameObject->getScale_x();
				float y = selectedGameObject->getScale_y();

				float xspeed = std::log10(1 + x) * incrementSpeed;
				float yspeed = std::log10(1 + y) * incrementSpeed;

				event->motion.yrel *= -1;

				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {

					event->motion.yrel = event->motion.xrel;
				}

				x += event->motion.xrel * invCameraScale * xspeed;
				y += event->motion.yrel * invCameraScale * yspeed;

				x = std::clamp(x, 0.f, FLT_MAX);
				y = std::clamp(y, 0.f, FLT_MAX);

				selectedGameObject->SetScale(x, y);
			}
			else {

				auto pos = selectedGameObject->getPosition();
				pos.x += event->motion.xrel * invCameraScale;
				pos.y += event->motion.yrel * invCameraScale;


				selectedGameObject->setPosition(pos);
			}
		}


		if (insideWindow && !anyGoSelected && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			SetSelectedGameObject(nullptr);
		}

		if (!(SDL_GetModState() & KMOD_SHIFT)) {
			if (focused) {
				sceneCamera->handleInput(event, insideWindow);
			}
		}

	}

	void Scene::Behaviour() {

		ResizeOverlayIfNeccesary();

		if (sceneCamera->ShouldResize(windowWidth, windowHeight))
			sceneCamera->Resize(windowWidth, windowHeight);

		auto it = gameObjects.begin();
		while (it != gameObjects.end()) {
			GameObject* go = it->second;

			go->update();

			if (go->isWaitingToDelete()) {
				selectedGameObject = nullptr;

				GameObject::unusedIds.push_back(go->getId());

				delete go;
				it = gameObjects.erase(it);
			}
			else {
				++it;
			}
		}


		auto it2 = overlays.begin();
		while (it2 != overlays.end()) {
			GameObject* go = *it2;

			go->update();

			if (go->isWaitingToDelete()) {
				selectedGameObject = nullptr;

				GameObject::unusedIds.push_back(go->getId());

				delete go;
				it2 = overlays.erase(it2);
			}
			else {
				++it2;
			}
		}



		sceneCamera->PrepareCameraRender();

		switch (viewMode) {

		case 0:

			RenderGameObjects();
			RenderUI();
			break;
		case 1:

			RenderGameObjects();

			break;
		case 2:

			RenderUI();

			break;

		default:
			break;
		}

		RenderFrame();


		sceneCamera->StopCameraRender();


		// Cambiar las propiedades del grid dependiendo del orden de magnitud

		float camScale = sceneCamera->GetScale();

		float spacing = 50 * sceneCamera->GetScale();
		int interval = 5;

		int offset_x = windowWidth * 0.5f;
		int offset_y = windowHeight * 0.5f;


		if (camScale < 0.7f) {

			spacing *= interval;
		}

		ScriptCreationUtilities::Grid::SetAlpha(0.5f);

		ScriptCreationUtilities::Grid::SetSpacing(spacing);
		ScriptCreationUtilities::Grid::SetInterval(interval);
		ScriptCreationUtilities::Grid::SetOffset(offset_x + sceneCamera->GetPosition().x, offset_y + sceneCamera->GetPosition().y);
		ScriptCreationUtilities::Grid::Draw();


		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Image(sceneCamera->GetTexture(), ImVec2(windowWidth, windowHeight));

		ImGui::SetCursorPos(ImVec2(10, ImGui::GetFrameHeight() + 10));
		ImGui::Text(name);

		ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(100, ImGui::GetFrameHeight() + 10));
		ImGui::SliderFloat("Zoom (-/+)", &sceneCamera->GetScale(), sceneCamera->GetMinScale(), sceneCamera->GetMaxScale(), "%.3f", ImGuiSliderFlags_Logarithmic);

		ImGui::SameLine();

		ImGui::RadioButton("##Scene view - Both", &viewMode, 0);
		ImGui::SameLine();
		ImGui::RadioButton("##Scene view - World", &viewMode, 1);
		ImGui::SameLine();
		ImGui::RadioButton("##Scene view - UI", &viewMode, 2);

	}

	void Scene::ReceiveAssetDrop(Asset& asset)
	{
		std::string extension = asset.extension;

		if (extension == ".png" || extension == ".jpg") {


			GameObject* go = AddGameObject(asset.relativePath);
			go->setName(asset.name);

			ImVec2 position = MousePositionInScene();

			go->setPosition(position);
			selectedGameObject = go;
		}

		if (asset.isPrefab) {

			GameObject* prefab = PrefabManager::GetPrefabById(asset.prefabId);

			GameObject* go = new GameObject(*prefab);
			AddGameObject(go);

			AddChildsToScene(go);

			ImVec2 position = MousePositionInScene();

			go->setPosition(position);
			selectedGameObject = go;
		}
	}

	ImVec2 Scene::MousePositionInScene() {

		ImVec2 mousepos = ImGui::GetMousePos();

		mousepos.x -= windowPosX;
		mousepos.y -= windowPosY;


		//Trasladar el origen de coordenadas de la esquina superior al centro
		mousepos.x -= windowWidth * 0.5f;
		mousepos.y -= windowHeight * 0.5f;

		ImVec2 cameraPosition = sceneCamera->GetPosition();
		mousepos.x -= cameraPosition.x;
		mousepos.y -= cameraPosition.y;

		float cameraScale = sceneCamera->GetScale();

		mousepos.x /= cameraScale;
		mousepos.y /= cameraScale;




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

	void Scene::ResizeOverlayIfNeccesary()
	{
		auto& prefData = Preferences::GetData();

		if (uiTexture == nullptr || uiWidth != prefData.width || uiHeight != prefData.height) {

			if (uiTexture != nullptr)
				SDL_DestroyTexture(uiTexture);

			uiWidth = prefData.width;
			uiHeight = prefData.height;

			uiTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, uiWidth, uiHeight);
			SDL_SetTextureBlendMode(uiTexture, SDL_BLENDMODE_BLEND);
		}
	}

	void Scene::CalculateFrameRect(int& x, int& y, int& w, int& h)
	{
		ImVec2 position = sceneCamera->GetPosition();

		float gameSizeX = Preferences::GetData().width;
		float gameSizeY = Preferences::GetData().height;

		float width = gameSizeX * sceneCamera->GetScale();
		float height = gameSizeY * sceneCamera->GetScale();


		sceneCamera->CenterPosition(position.x, position.y);


		x = position.x;
		y = position.y;
		w = width;
		h = height;
	}

	bool Scene::PointInsideHorizontalSegment(int x, int y, int sx, int sy, int w, int thickness)
	{
		int ht = std::round(thickness * 0.5f);
		return x + ht > sx && x - ht< sx + w && y + ht > sy && y - ht < sy;
	}

	bool Scene::PointInsideVerticalSegment(int x, int y, int sx, int sy, int h, int thickness)
	{
		int ht = std::round(thickness * 0.5f);
		return x + ht > sx && x - ht < sx && y + ht> sy && y - ht < sy + h;
	}

	bool Scene::PointInsideRect(int x, int y, int rx, int ry, int rw, int rh, int thickness)
	{
		int ht = std::round(thickness * 0.5f);
		return x + ht > rx && x - ht< rx + rw && y + ht > ry && y - ht < ry + rh;
	}

	void Scene::RenderRectangle(int x, int y, int w, int h, int thickness)
	{
		int ht = std::round(thickness * 0.5f);

		SDL_Rect frameRect = { x - ht, y - ht, w + ht * 2, h + ht * 2 };


		for (int i = 0; i < thickness; i++) //SDL no tiene soporte para cambiar el grosor de la linea
		{

			SDL_RenderDrawRect(renderer, &frameRect);

			frameRect.x++;
			frameRect.y++;
			frameRect.w -= 2;
			frameRect.h -= 2;
		}


	}

	void Scene::RenderHorizontalSegment(int x, int y, int w, int thickness)
	{
		int ht = std::round(thickness * 0.5f);

		x -= ht;
		y -= ht;

		w += ht * 2;

		for (int i = 0; i < ht * 2; i++)
		{

			SDL_RenderDrawLine(renderer, x, y + i, x + w, y + i);
		}
	}

	void Scene::RenderVerticalSegment(int x, int y, int h, int thickness)
	{
		int ht = std::round(thickness * 0.5f);

		x -= ht;
		y -= ht;

		h += ht * 2;

		for (int i = 0; i < ht * 2; i++)
		{

			SDL_RenderDrawLine(renderer, x + i, y, x + i, y + h);
		}
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

		nlohmann::ordered_json overlayObjectsJson = nlohmann::json::array();
		for (const auto& o : overlays) {
			if (o->getParent() == nullptr)
				gameObjectsJson.push_back(j.parse(o->toJson()));
		}

		j["overlays"] = overlayObjectsJson;


		return j.dump(2);
	}

}