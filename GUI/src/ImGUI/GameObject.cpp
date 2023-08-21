#include "GameObject.h"

#include "ResourcesManager.h"
#include "ComponentManager.h"
#include "nlohmann/json.hpp"
#include "ComponentInfo.h"
#include "Preferences.h"
#include "Texture.h"
#include "Editor.h"
#include "Camera.h"
#include "Scene.h"
#include "imgui.h"
#include "SDL.h"

#include <fstream>

#define GizmoImage "gizmo.png"


namespace ShyEditor {

	int GameObject::lastId = 0;

	GameObject::GameObject(std::string& path) {

		editor = Editor::getInstance();

		name = "Empty Gameobject";

		// Transform attributes
		pos = new ImVec2(0, 0);
		scale = new ImVec2(1, 1);
		size = new ImVec2(100, 100);

		imagePath = path;

		parent = nullptr;

		showGizmo = false;
		visible = true;
		renderOrder = 0;

		waitingToDelete = false;

		previousMousePosX = 0;
		previousMousePosY = 0;

		// Gets the list of gameobjects in the scene
		std::unordered_map<int, GameObject*> gameObjects = editor->getScene()->getGameObjects();



		// Advance id if it already exists, can happen when we load a scene BECAUSE the gameObjects saved have an id
		while (gameObjects.find(GameObject::lastId) != gameObjects.end()) {
			GameObject::lastId++;
		}

		id = GameObject::lastId;

		GameObject::lastId++;



		// Gizmo texture
		gizmo = ResourcesManager::GetInstance()->AddTexture(GizmoImage, true);

		// Gameobject texture
		texture = ResourcesManager::GetInstance()->AddTexture(path, false);

		if (texture->getSDLTexture() != NULL) {

			// Sets the size of the gameobject based on the texture width and height
			*size = ImVec2(texture->getWidth(), texture->getHeight());

			// Add component image
			Components::Component imageComponent = Components::ComponentManager::GetAllComponents().find("Image")->second;
			Components::AttributeValue attributeValue;
			attributeValue.valueString = imagePath;
			imageComponent.getAttribute("fileName").SetValue(attributeValue);
			this->addComponent(imageComponent);

			// Set the gameobject name as the image name
			std::size_t extensionPos = path.find_last_of('.');
			name = (extensionPos != std::string::npos) ? path.substr(0, extensionPos) : path;

		}

	}

	GameObject::~GameObject() {

		if (parent != nullptr)
			parent->removeChild(this);

		for (auto child : children)
			child.second->setParent(nullptr);

		components.clear();

		delete pos;
		delete scale;
		delete size;

	}





	// ------------------------------------- Render, update and input -----------------------------------

	void GameObject::render(SDL_Renderer* renderer, Camera* camera) {

		ImVec2 position = getAdjustedPosition();

		float width = size->x * getScale_x();
		float height = size->y * getScale_y();

		// Posicion y tamanios relativos al frame de la escena
		ImVec2 relativePosition = ImVec2((position.x + camera->getPosition().x) * camera->getScrollFactor(),
			(position.y + camera->getPosition().y) * camera->getScrollFactor());

		float relativeWidth = width * camera->getScrollFactor();
		float relativeHeight = height * camera->getScrollFactor();

		SDL_Rect dst = { relativePosition.x, relativePosition.y, relativeWidth, relativeHeight };

		// Image render
		if (visible && texture != NULL)
			SDL_RenderCopyEx(renderer, texture->getSDLTexture(), NULL, &dst, rotation, NULL, SDL_FLIP_NONE);

		// Render outline
		if (this == editor->getScene()->getSelectedGameObject()) {

			// SAVE THE PREVIOUS COLOR TO RESTART IT AFTER DRAWING THE FRAME
			Uint8 r, g, b, a;
			SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

			SDL_Rect outlineRect = { relativePosition.x, relativePosition.y, relativeWidth, relativeHeight };
			int thickness = 3;

			for (int i = 0; i < thickness; i++) {
				SDL_Rect currentRect = { outlineRect.x - i, outlineRect.y - i, outlineRect.w + i * 2, outlineRect.h + i * 2 };
				SDL_RenderDrawRect(renderer, &currentRect);
			}

			if (showGizmo) {
				dst.x += dst.w / 2;
				dst.y -= dst.h / 2;

				SDL_RenderCopy(renderer, gizmo->getSDLTexture(), NULL, &dst);
			}
		}
	}

	void GameObject::update() {

		auto image = components.find("Image");

		// If the gameobject has no image component
		if (image == components.end()) {
			texture = nullptr;
			return;
		}

		std::string currentImagePath = (image->second).getAttribute("fileName").value.valueString;

		// Checks if the current path exists in the filesystem
		//if (!std::filesystem::exists(currentImagePath)) return;

		// If the image path have changed, checks if the new path is valid
		if (currentImagePath != imagePath) {

			texture = ResourcesManager::GetInstance()->AddTexture(currentImagePath, false);

			if (texture->getSDLTexture() != NULL) {
				imagePath = currentImagePath;
				*size = ImVec2(texture->getWidth(), texture->getHeight());
			}
			else {
				*size = ImVec2(100, 100);
				imagePath = "Empty Gameobject";
			}
		}

	}

	bool GameObject::handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos) {

		showGizmo = false;

		if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_DELETE && editor->getScene()->getSelectedGameObject() == this) {
			toDelete();
		}

		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {

			if (!leftMouseButtonDown) 
				leftMouseButtonDown = true;

			if (visible && isMouseInsideGameObject) {
				editor->getScene()->setSelectedGameObject(this);
				return true;
			}
		}

		if (event->type == SDL_MOUSEBUTTONUP) {

			if (leftMouseButtonDown && event->button.button == SDL_BUTTON_LEFT)
				leftMouseButtonDown = false;

			if (rightMouseButtonDown && event->button.button == SDL_BUTTON_RIGHT)
				rightMouseButtonDown = false;
		}

		if (editor->getScene()->getSelectedGameObject() == this) {

			if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_RIGHT) {

				if (!rightMouseButtonDown) {
					rightMouseButtonDown = true;
				}
			}

			if (SDL_GetModState() & KMOD_SHIFT) {
				showGizmo = true;

				if (event->type == SDL_MOUSEMOTION)
				{
					if (leftMouseButtonDown)
					{

						ImVec2* previousPos = new ImVec2{ pos->x, pos->y };

						pos->x = mousePos.x - scale->x / 2;
						pos->y = mousePos.y - scale->y / 2;

						translateChildren(this, previousPos);

						delete previousPos;
					}

					if (rightMouseButtonDown)
					{
						float rotationAngle = (previousMousePosX - mousePos.x) * 0.5f + (previousMousePosY - mousePos.y) * 0.5f;
						rotation += rotationAngle;

						rotateChildren(this, this, rotationAngle);

					}
				}

				if (event->type == SDL_MOUSEWHEEL) {

					int scaleFactor = 0;

					if (event->wheel.y > 0) // scroll up
					{
						scale->x += 5;
						scale->y += 5;

						scaleFactor = 5;

					}
					else if (event->wheel.y < 0) // scroll down
					{
						scale->x -= 5;
						scale->y -= 5;

						scaleFactor = -5;
					}

					scaleChildren(this, scaleFactor);
				}
			}
		}

		previousMousePosX = mousePos.x;
		previousMousePosY = mousePos.y;

		return false;
	}





	// ----------------------------- Name, ID and texture getters/setters ---------------------------

	std::string GameObject::getName() {
		return name;
	}

	void GameObject::setName(const std::string& newName) {
		name = newName;
	}

	Texture* GameObject::getTexture() {
		return texture;
	}

	int GameObject::getId() {
		return id;
	}






	// ----------------------------------- Visibility getters/setters -----------------------------------

	int GameObject::getRenderOrder() {
		return renderOrder;
	}

	bool GameObject::isVisible() {
		return visible;
	}

	void GameObject::setVisible(bool visible) {
		if (this->parent == nullptr) {
			this->visible = visible;
			setChildrenVisible(this, visible);
		}
	}






	// ----------------------------------- Components and Scripts logic --------------------------------------

	void GameObject::addComponent(::Components::Component comp) {

		if (components.find(comp.getName()) == components.end())
			components.insert({ comp.getName(), comp });

	}

	void GameObject::addScript(::Components::Script script) {

		if (scripts.contains(script.GetName())) return;

		scripts.emplace(script.GetName(), script);
	}

	std::unordered_map<std::string, ::Components::Component>* GameObject::getComponents() {
		return &components;
	}

	std::unordered_map<std::string, ::Components::Script>* GameObject::getScripts() {
		return &scripts;
	}





	// --------------------------------- Tranform attributes getters/setters -----------------------------------

	void GameObject::setPosition(ImVec2 newPos) {
		pos->x = newPos.x;
		pos->y = newPos.y;
	}

	ImVec2 GameObject::getPosition() {
		return *pos;
	}

	float GameObject::getRotation() {
		return rotation;
	}

	ImVec2 GameObject::getAdjustedPosition() {

		ImVec2 position = *pos;

		// The origin is the center of the frame
		position.x += Preferences::GetData().width * 0.5f;
		position.y += Preferences::GetData().height * 0.5f;

		float width = size->x * getScale_x();
		float height = size->y * getScale_y();

		// The game objects have their origin at the center
		position.x -= width * 0.5f;
		position.y -= height * 0.5f;

		return position;
	}

	ImVec2 GameObject::getSize() {
		return *size;
	}

	float GameObject::getScale_x() {
		return scale->x;
	}

	float GameObject::getScale_y() {
		return scale->y;
	}






	// --------------------------------------- Deleting gameobject logic ---------------------------------------

	bool GameObject::isWaitingToDelete() {
		return waitingToDelete;
	}

	void GameObject::toDelete() {
		waitingToDelete = true;

		for (auto child : children) {
			child.second->toDelete();
		}
	}





	// --------------------------------- Gameobject children and parent logic ------------------------------------

	void GameObject::setParent(GameObject* go) {
		parent = go;
	}

	GameObject* GameObject::getParent() {
		return parent;
	}

	void GameObject::removeChild(GameObject* go) {
		go->setParent(nullptr);
		children.erase(go->getId());
	}

	void GameObject::addChild(GameObject* go) {
		go->setParent(this);
		children.insert({ go->getId(), go });
	}

	std::unordered_map<int, GameObject*> GameObject::getChildren() {
		return children;
	}

	bool GameObject::isAscendant(GameObject* go) {

		for (auto childPair : children) {
			GameObject* child = childPair.second;

			if (child == go || child->isAscendant(go))
				return true;
		}

		return false;
	}






	// ---------------------------------------- Gameobject drawing logic ----------------------------------------------

	void GameObject::drawTransformInEditor() {

		if (ImGui::CollapsingHeader("Transform")) {

			ImGui::Text("Position");
			ImGui::DragFloat2("##position_drag", (float*)pos, 0.3f, 0.0f, 0.0f, "%.2f");

			ImGui::Text("Scale");
			ImGui::DragFloat2("##scale_drag", (float*)scale, 0.02f, 0.0f, FLT_MAX, "%.2f");

			ImGui::Text("Rotation");
			ImGui::DragFloat("##rotation_drag", &rotation, 0.1f, 0.0f, 0.0f, "%.2f");

			ImGui::Text("Render order");
			ImGui::InputInt("##render_order", &renderOrder);
		}
	}

	void GameObject::drawComponentsInEditor() {

		for (auto it = components.begin(); it != components.end();) {
			std::string componentName = (*it).second.getName();

			if (ImGui::CollapsingHeader(componentName.c_str())) {

				for (auto& attribute : (*it).second.getAllAttributes()) {
					std::string attributeName = attribute.first;
					Components::Attribute* attr = &attribute.second;

					ImGui::Text(attributeName.c_str());

					switch (attr->getType()) {

						case Components::AttributesType::FLOAT:
							drawFloat(attributeName + it->first, attr);
							break;
						case Components::AttributesType::VECTOR2:
							drawVector2(attributeName + it->first, attr);
							break;
						case Components::AttributesType::STRING:
							drawString(attributeName + it->first, attr);
							break;
						case Components::AttributesType::BOOL:
							drawBool(attributeName + it->first, attr);
							break;
						case Components::AttributesType::COLOR:
							drawColor(attributeName + it->first, attr);
							break;
						case Components::AttributesType::CHAR:
							drawChar(attributeName + it->first, attr);
							break;
						case Components::AttributesType::GAMEOBJECT:
							drawGameobject(attributeName + it->first, attr);
							break;
						default:
							break;
					}
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1.0f));

				if (ImGui::Button(("Delete component##" + componentName).c_str(), ImVec2(ImGui::GetWindowSize().x, 40)))
					it = components.erase(it);
				else
					++it;

				ImGui::PopStyleColor(2);

			}
			else ++it;
		}
	}

	void GameObject::drawScriptsInEditor() {

		for (auto it = scripts.begin(); it != scripts.end();) {

			std::string scriptName = (*it).first;
			if (ImGui::CollapsingHeader(scriptName.c_str())) {

				for (auto& attribute : (*it).second.getAllAttributes()) {
					std::string attributeName = attribute.first;
					Components::Attribute* attr = &attribute.second;

					ImGui::Text(attributeName.c_str());

					switch (attr->getType()) {

						case Components::AttributesType::FLOAT:
							drawFloat(attributeName + it->first, attr);
							break;
						case Components::AttributesType::VECTOR2:
							drawVector2(attributeName + it->first, attr);
							break;
						case Components::AttributesType::STRING:
							drawString(attributeName + it->first, attr);
							break;
						case Components::AttributesType::BOOL:
							drawBool(attributeName + it->first, attr);
							break;
						case Components::AttributesType::COLOR:
							drawColor(attributeName + it->first, attr);
							break;
						case Components::AttributesType::CHAR:
							drawChar(attributeName + it->first, attr);
							break;
						case Components::AttributesType::GAMEOBJECT:
							drawGameobject(attributeName + it->first, attr);
							break;
						default:
							break;
					}
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255.0f, 255.0f, 255.0f, 1.0f));

				if (ImGui::Button(("Edit script##" + scriptName).c_str(), ImVec2(ImGui::GetWindowSize().x, 40)))
					editor->OpenScript(scriptName);

				if (ImGui::Button(("Delete script##" + scriptName).c_str(), ImVec2(ImGui::GetWindowSize().x, 40)))
					it = scripts.erase(it);
				else
					++it;

				ImGui::PopStyleColor(2);

			}
			else ++it;
		}

	}

	void GameObject::drawFloat(std::string attrName, ::Components::Attribute* attr) {

		ImGui::DragFloat(("##" + attrName).c_str(), &attr->value.value.valueFloat, 0.3f, 0.0f, 0.0f, "%.2f");
	}

	void GameObject::drawVector2(std::string attrName, ::Components::Attribute* attr) {

		ImGui::DragFloat2(("##" + attrName).c_str(), (float*)&attr->value.value.valueVector2, 0.3f, 0.0f, 0.0f, "%.2f");
	}

	void GameObject::drawString(std::string attrName, ::Components::Attribute* attr) {

		char inputBuffer[256];
		strncpy_s(inputBuffer, attr->value.valueString.c_str(), sizeof(inputBuffer));

		if (ImGui::InputText(("##" + attrName).c_str(), inputBuffer, sizeof(inputBuffer)))
			attr->value.valueString = inputBuffer;

	}

	void GameObject::drawBool(std::string attrName, ::Components::Attribute* attr) {

		ImGui::Checkbox(("##" + attrName).c_str(), &attr->value.value.valueBool);
	}

	void GameObject::drawColor(std::string attrName, ::Components::Attribute* attr) {

		ImGui::ColorEdit3(("##" + attrName).c_str(), (float*)&attr->value.value.valueColor);
	}

	void GameObject::drawChar(std::string attrName, ::Components::Attribute* attr) {

		if (ImGui::InputText(("##" + attrName).c_str(), &attr->value.value.valueChar, 2, ImGuiInputTextFlags_CharsNoBlank)) {

		}
	}

	void GameObject::drawGameobject(std::string attrName, ::Components::Attribute* attr) {

		std::unordered_map<int, GameObject*> gameObjects = editor->getScene()->getGameObjects();

		GameObject* go = gameObjects.find((int)attr->value.value.valueFloat) != gameObjects.end() ? gameObjects.find((int)attr->value.value.valueFloat)->second : nullptr;

		std::string name = "";
		if (attr->value.valueString != "")
			name = attr->value.valueString;

		if (go != nullptr) {
			name = go->getName().c_str();
		}

		if (ImGui::BeginCombo(("##" + attrName).c_str(), name != "" ? name.c_str() : nullptr)) {
			for (auto go : gameObjects) {
				if (ImGui::Selectable(go.second->getName().c_str()))
					attr->value.value.valueFloat = go.second->getId();
			}

			if (std::filesystem::is_directory("Prefabs"))
				for (const auto& entry : std::filesystem::directory_iterator("Prefabs")) {
					if (entry.path().extension() == ".prefab") {
						std::string prefabFileName = entry.path().filename().string();
						if (ImGui::Selectable(prefabFileName.c_str())) {

							std::ifstream inputFile("Prefabs/" + prefabFileName);

							nlohmann::ordered_json jsonData;
							try {
								inputFile >> jsonData;
							}
							catch (const nlohmann::json::parse_error& e) {
								std::cerr << "JSON parse error: " << e.what() << std::endl;
								return;
							}
							inputFile.close();

							attr->value.value.valueFloat = jsonData["id"];
							attr->value.valueString = prefabFileName;

						}
					}
				}

			ImGui::EndCombo();
		}

		ImGui::SameLine();

		if (ImGui::Button(("X##" + attrName).c_str())) {
			attr->value.value.valueFloat = -1;
			attr->value.valueString = "";
		}
	}





	// ---------------------- Gameobject children settings (Transform and visibility) ----------------------

	void GameObject::translateChildren(GameObject* go, ImVec2* previousPos) {

		ImVec2 parentPreviousPos = { previousPos->x, previousPos->y };

		for (auto childPair : go->getChildren()) {

			ImVec2 childPos = childPair.second->getPosition();

			float xDiff = go->getPosition().x - parentPreviousPos.x;
			float yDiff = go->getPosition().y - parentPreviousPos.y;

			previousPos->x = childPos.x;
			previousPos->y = childPos.y;

			childPair.second->setPosition(ImVec2(childPos.x + xDiff, childPos.y + yDiff));

			translateChildren(childPair.second, previousPos);
		}
	}

	void GameObject::scaleChildren(GameObject* go, int scaleFactor) {

		for (auto childPair : go->getChildren()) {

			childPair.second->scale->x += scaleFactor;
			childPair.second->scale->y += scaleFactor;

			scaleChildren(childPair.second, scaleFactor);
		}
	}

	void GameObject::setChildrenVisible(GameObject* go, bool visible) {

		for (auto childPair : go->getChildren()) {

			childPair.second->visible = visible;

			setChildrenVisible(childPair.second, visible);
		}
	}

	void GameObject::rotateChildren(GameObject* go, GameObject* goCenter, float rotationAngle) {

		float angleRadians = rotationAngle * (3.14159265359f / 180.0f);

		for (auto child : go->getChildren())
		{
			float newX = cos(angleRadians) * (child.second->pos->x - goCenter->pos->x) - sin(angleRadians) * (child.second->pos->y - goCenter->pos->y) + goCenter->pos->x;
			float newY = sin(angleRadians) * (child.second->pos->x - goCenter->pos->x) + cos(angleRadians) * (child.second->pos->y - goCenter->pos->y) + goCenter->pos->y;

			child.second->pos->x = newX;
			child.second->pos->y = newY;
			child.second->rotation += rotationAngle;

			rotateChildren(child.second, goCenter, rotationAngle);
		}
	}





	// ------------------------ Serialization and deseralization logic -------------------------

	std::string GameObject::toJson(bool isPrefab) {

		nlohmann::ordered_json j;
		j["name"] = name;

		nlohmann::ordered_json childsJson = nlohmann::json::array();
		for (auto childPair : children) {
			auto child = j.parse(childPair.second->toJson());

			childsJson.push_back(child);
		}

		if (isPrefab) {
			//if its a prefab it needs a new Id
			std::unordered_map<int, GameObject*> gameObjects = editor->getScene()->getGameObjects();
			while (gameObjects.find(GameObject::lastId) != gameObjects.end()) {
				GameObject::lastId++;
			}

			j["id"] = GameObject::lastId;

			GameObject::lastId++;
		}
		else {
			j["id"] = id;
		}

		j["childs"] = childsJson;

		j["order"] = renderOrder;

		j["localPosition"] = std::to_string(pos->x) + ", " + std::to_string(pos->y);
		j["localScale"] = std::to_string(scale->x) + ", " + std::to_string(scale->y);
		j["localRotation"] = std::to_string(rotation);

		nlohmann::ordered_json componentsJson = nlohmann::json::array();
		for (auto it = components.begin(); it != components.end(); it++) {

			auto comp = j.parse(it->second.toJson());

			componentsJson.push_back(comp);
		}

		j["components"] = componentsJson;


		nlohmann::ordered_json scriptsJson = nlohmann::json();
		for (auto it = scripts.begin(); it != scripts.end(); it++) {
			auto scriptJson = j.parse(it->second.ToJson());

			if (scriptJson.is_null())
				scriptJson = nlohmann::json::array();

			scriptsJson[it->second.GetName()] = scriptJson;
		}

		j["scripts"] = scriptsJson;


		return j.dump(2);
	}

	GameObject* GameObject::fromJson(std::string json, bool isPrefab) {

		nlohmann::ordered_json jsonData;
		try {
			jsonData = nlohmann::json::parse(json);
		}
		catch (const nlohmann::json::parse_error& e) {
			std::cerr << "JSON parse error: " << e.what() << std::endl;
			return nullptr;
		}

		std::string goName = jsonData["name"];

		GameObject* gameObject = new GameObject(goName);
		gameObject->name = goName;

		//if its prefab we leave the autoassigned id
		if (!isPrefab) {
			gameObject->id = jsonData["id"];
		}

		for (const auto& childJson : jsonData["childs"]) {
			GameObject* child = GameObject::fromJson(childJson.dump());

			gameObject->addChild(child);
			child->setParent(gameObject);
		}

		gameObject->renderOrder = jsonData["order"];

		// Deserialize localPosition, localScale, and localRotation
		std::string localPositionStr = jsonData["localPosition"];
		std::string localScaleStr = jsonData["localScale"];
		std::string localRotation = jsonData["localRotation"];

		// Parse localPosition and localScale
		ImVec2* localPosition;
		ImVec2* localScale;
		localPosition = new ImVec2();
		localScale = new ImVec2();

		sscanf_s(localPositionStr.c_str(), "%f, %f", &localPosition->x, &localPosition->y);
		sscanf_s(localScaleStr.c_str(), "%f, %f", &localScale->x, &localScale->y);

		gameObject->pos = localPosition;
		gameObject->scale = localScale;
		gameObject->rotation = std::stof(localRotation);


		for (const auto& compJson : jsonData["components"]) {
			Components::Component component = Components::Component::fromJson(compJson.dump());
			gameObject->addComponent(component);
		}


		for (const auto& scriptJson : jsonData["scripts"].items()) {
			Components::Script script = Components::Script::fromJson(scriptJson.key(), scriptJson.value().dump());
			gameObject->addScript(script);
		}

		return gameObject;
	}

}


