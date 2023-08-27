#include "Scene.h"

#include "ScriptCreationUtilities.h"
#include "ResourcesManager.h"
#include "ProjectsManager.h"
#include "FileExplorer.h"
#include "Preferences.h"
#include "Components.h"
#include "LogManager.h"
#include "Entity.h"
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

		// Load the default scene or the last opene scene
		bool load = LoadScene();
		editor->SetAnySceneOpened(load);

		sceneCamera = new Camera(ImVec2(0, 0), 1, renderer);
		sceneCamera->SetConstrains(.1, 10.0);

		uiTexture = nullptr;
		uiWidth = 0; uiHeight = 0;
		ResizeOverlayIfNeccesary();

		// Pointer to the selected entity in the scene
		selectedEntity = nullptr;

		docked = true;
		viewMode = 0;

		acceptAssetDrop = true;

		editor->getFileExplorer()->Refresh();

	}

	Scene::~Scene() {

		// Delete entities in the scene
		for (const auto& pair : entities)
			delete pair.second;

		entities.clear();

		// Delete overlays in the scene
		for (const auto& o : overlays)
			delete o;

		overlays.clear();

		delete sceneCamera;
	}

	Entity* Scene::AddEntity(std::string path) {

		Entity* entity = new Entity(path, true);
		entities.emplace(entity->GetId(), entity);

		return entity;
	}

	void Scene::AddOverlayChildsToScene(Entity* entity)
	{
		for (auto& pair : entity->GetChildren()) {
			overlays.push_back(pair.second);

			AddOverlayChildsToScene(pair.second);
		}
	}

	void Scene::AddEntity(Entity* entity) {
		entities.emplace(entity->GetId(), entity);
	}

	void Scene::AddEntityChildsToScene(Entity* entity)
	{
		for (auto& pair : entity->GetChildren()) {
			entities.emplace(pair.second->GetId(), pair.second);

			AddEntityChildsToScene(pair.second);
		}
	}

	Entity* Scene::AddOverlay(std::string path)
	{
		Entity* entity = new Entity(path, false);
		overlays.push_back(entity);

		return entity;
	}

	void Scene::AddOverlay(Entity* overlay) {
		overlays.push_back(overlay);
	}

	std::map<int, Entity*>& Scene::GetEntities() {
		return entities;
	}

	std::vector<Entity*>& Scene::GetOverlays()
	{
		return overlays;
	}

	std::string Scene::GetSceneName() {
		return sceneName;
	}

	Entity* Scene::GetSelectedEntity() {
		return selectedEntity;
	}

	void Scene::SetSelectedEntity(Entity* entity) {
		selectedEntity = entity;
	}

	void Scene::NewScene(const std::string& name) {

		Editor::getInstance()->getFileExplorer()->Refresh();

		nlohmann::ordered_json j;

		scenePath = Editor::getInstance()->getProjectInfo().assetPath + "\\" + name + ".scene";
		sceneName = name;
		this->name = sceneName.c_str();

		j = j.parse(ToJson());

		std::ofstream outputFile(scenePath);
		if (outputFile.is_open()) {
			outputFile << j.dump(4);
			outputFile.close();
		}
		else LogManager::LogError("Could not open the file to create the scene.");

	}

	void Scene::SaveScene() {

		nlohmann::ordered_json j;
		j = j.parse(ToJson());

		std::ofstream outputFile(scenePath);
		if (outputFile.is_open()) {
			outputFile << j.dump(4);
			outputFile.close();
		}
		else LogManager::LogError("Could not open the file to save the scene.");

	}

	bool Scene::LoadScene() {

		scenePath = Editor::getInstance()->getProjectInfo().assetPath + "\\" + Editor::getInstance()->GetLastOpenedScene() + ".scene";
		std::filesystem::path p(scenePath);
		sceneName = p.filename().stem().string();
		name = sceneName.c_str();

		// Delete info of previous scene
		selectedEntity = nullptr;

		for (const auto& pair : entities)
			delete pair.second;

		entities.clear();

		for (const auto& o : overlays)
			delete o;

		overlays.clear();

		std::ifstream inputFile(scenePath);

		if (!inputFile.good() || !nlohmann::ordered_json::accept(inputFile)) {
			LogManager::LogError("The scene file has not the expected format.");
			return false;
		}

		inputFile.clear();
		inputFile.seekg(0);

		nlohmann::ordered_json jsonData = nlohmann::ordered_json::parse(inputFile);
		inputFile.close();


		if (!jsonData.contains("objects")) {
			LogManager::LogError("The scene file has not the expected format.");
			return false;
		}

		nlohmann::ordered_json& entitiesJson = jsonData["objects"];

		// Iterate through the entities JSON array
		for (nlohmann::ordered_json& entityJson : entitiesJson) {
			Entity* entity = Entity::FromJson(entityJson);
			entities.insert({ entity->GetId(), entity });
			AddEntityChildsToScene(entity);
		}

		if (!jsonData.contains("overlays")) {
			LogManager::LogError("The scene file has not the expected format.");
			return false;
		}

		nlohmann::ordered_json& overlaysJson = jsonData["overlays"];

		// Iterate through the overlay entities JSON array
		for (nlohmann::ordered_json& overlayJson : overlaysJson) {
			Entity* overlay = Entity::FromJson(overlayJson);
			overlays.push_back(overlay);
			AddOverlayChildsToScene(overlay);
		}

		return true;

	}

	void Scene::RenderChildEntities(Entity* entity)
	{
		for (auto& pair : entity->GetChildren()) {
			RenderChildEntities(pair.second);
			pair.second->RenderTransform(renderer, sceneCamera);
		}
	}

	void Scene::RenderEntities()
	{
		std::vector<Entity*> sortedEntities;
		for (const auto& pair : entities) {
			if (pair.second->GetParent() == nullptr)
				sortedEntities.push_back(pair.second);
		}

		std::sort(sortedEntities.begin(), sortedEntities.end(), CompareEntitiesRenderOrder);

		for (auto entity : sortedEntities) {
			entity->RenderTransform(renderer, sceneCamera);
			RenderChildEntities(entity);
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


		bool inScene = IsMouseHoveringWindow();
		auto mouse = MousePositionInScene();

		float cameraScale = sceneCamera->GetScale();

		for (auto overlayEntity : overlays) {

			Overlay* overlay = overlayEntity->GetOverlay();


			//TODO: aplicar color y escala

			SDL_Rect dest{};
			overlay->CalculateRectangle(dest.x, dest.y, dest.w, dest.h);


			bool mouseclicked = ImGui::IsMouseClicked(0);

			const int thickness = 10;


			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			RenderRectangle(dest.x, dest.y, dest.w, dest.h, 5);

			overlay->Render(renderer, dest.x, dest.y, dest.w, dest.h);

			if (!inScene) continue;

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



					selectedEntity = overlayEntity;
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

						//Lo que hayamos aumentado de posicion hay que reducirlo de tama�o y viceversa
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

						//Lo que hayamos aumentado de posicion hay que reducirlo de tama�o y viceversa
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

						//Lo que haya incrementado de tama�o hay que compensarlo con la posicion, para de esta forma
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

						//Lo que haya incrementado de tama�o hay que compensarlo con la posicion, para de esta forma
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
		bool anyEntitySelected = false;

		if (insideWindow && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
		{
			for (const auto& pair : entities) {

				if (IsMouseHoveringEntity(pair.second)) {

					anyEntitySelected = true;
					selectedEntity = pair.second;
					dragging = true;
				}

			}
		}


		if (insideWindow && !anyEntitySelected && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			SetSelectedEntity(nullptr);
		}

		if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
		{
			dragging = false;
		}

		if (dragging && selectedEntity != nullptr && selectedEntity->IsTransform() && event->type == SDL_MOUSEMOTION) {

			float invCameraScale = 1.f / sceneCamera->GetScale();

			if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {

				const float incrementSpeed = .3f;

				float r = selectedEntity->GetRotation();

				r += event->motion.xrel * incrementSpeed;

				selectedEntity->SetRotation(r);
			}
			else if (ImGui::IsKeyDown(ImGuiKey_LeftAlt)) {

				const float incrementSpeed = .03f;

				float x = selectedEntity->GetScaleX();
				float y = selectedEntity->GetScaleY();

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

				selectedEntity->SetScale(x, y);
			}
			else {

				auto pos = selectedEntity->GetPosition();
				pos.x += event->motion.xrel * invCameraScale;
				pos.y += event->motion.yrel * invCameraScale;


				selectedEntity->SetPosition(pos);
			}
		}


		sceneCamera->handleInput(event, insideWindow, focused);

		if (event->type == SDL_KEYUP && event->key.keysym.scancode == SDL_SCANCODE_SPACE) {


			if (selectedEntity != nullptr) {

				if (selectedEntity->IsTransform()) {

					auto pos = selectedEntity->GetPosition();
					sceneCamera->SetPosition(pos.x, pos.y);
				}
				else {

					float camScale = sceneCamera->GetScale();
					auto pos = selectedEntity->GetOverlay()->CalculateCenterPoint();
					sceneCamera->SetPosition(pos.x * camScale, pos.y * camScale);

				}
			}
		}

		if (event->type == SDL_KEYDOWN) {

			if (event->key.keysym.scancode == SDL_SCANCODE_S) {

				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
					SaveScene();
			}
		}

	}

	void Scene::Behaviour() {

		ResizeOverlayIfNeccesary();

		if (sceneCamera->ShouldResize(windowWidth, windowHeight))
			sceneCamera->Resize(windowWidth, windowHeight);

		auto it = entities.begin();
		while (it != entities.end()) {
			Entity* entity = it->second;

			entity->Update();

			if (entity->IsWaitingToDelete()) {
				selectedEntity = nullptr;

				Entity::unusedIds.push_back(entity->GetId());

				delete entity;
				it = entities.erase(it);
			}
			else {
				++it;
			}
		}


		auto it2 = overlays.begin();
		while (it2 != overlays.end()) {
			Entity* entity = *it2;

			entity->Update();

			if (entity->IsWaitingToDelete()) {
				selectedEntity = nullptr;

				Entity::unusedIds.push_back(entity->GetId());

				delete entity;
				it2 = overlays.erase(it2);
			}
			else {
				++it2;
			}
		}



		sceneCamera->PrepareCameraRender();

		switch (viewMode) {

		case 0:

			RenderEntities();
			RenderUI();
			break;
		case 1:

			RenderEntities();

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


			Entity* entity = AddEntity(asset.relativePath);
			entity->SetName(asset.name);

			ImVec2 position = MousePositionInScene();

			entity->SetPosition(position);
			selectedEntity = entity;
		}

		if (asset.isPrefab) {

			Entity* prefab = PrefabManager::GetPrefabById(asset.prefabId);

			//Create a copy of the prefab
			Entity* entity = new Entity(*prefab);
			if (entity->IsTransform()) {
				AddEntity(entity);

				AddEntityChildsToScene(entity);

				ImVec2 position = MousePositionInScene();

				entity->SetPosition(position);
			}
			else {
				AddOverlay(entity);
				AddOverlayChildsToScene(entity);
			}
			
			selectedEntity = entity;
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

	bool Scene::IsMouseHoveringEntity(Entity* entity) {

		auto mousePos = MousePositionInScene();

		auto entityPos = entity->GetAdjustedPosition();
		auto entitySize = entity->GetSize();

		entitySize.x *= entity->GetScaleX();
		entitySize.y *= entity->GetScaleY();

		return mousePos.x > entityPos.x && mousePos.x < entityPos.x + entitySize.x &&
			mousePos.y > entityPos.y && mousePos.y < entityPos.y + entitySize.y;
	}

	bool Scene::CompareEntitiesRenderOrder(Entity* a, Entity* b) {
		return a->GetRenderOrder() < b->GetRenderOrder();
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


	std::string Scene::GetPath() {
		return scenePath;
	}

	std::string Scene::ToJson() {

		nlohmann::ordered_json j;

		j["name"] = sceneName;

		nlohmann::ordered_json entitiesJson = nlohmann::json::array();
		for (const auto& pair : entities) {
			if (pair.second->GetParent() == nullptr)
				entitiesJson.push_back(j.parse(pair.second->ToJson()));
		}

		j["objects"] = entitiesJson;

		nlohmann::ordered_json overlayEntitiesJson = nlohmann::json::array();
		for (const auto& o : overlays) {
			if (o->GetParent() == nullptr)
				overlayEntitiesJson.push_back(j.parse(o->ToJson()));
		}

		j["overlays"] = overlayEntitiesJson;

		return j.dump(2);
	}

}