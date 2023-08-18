#include "GameObject.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "ImGUIManager.h"
#include "Camera.h"
#include "Scene.h"
#include "SDL.h"
#include "nlohmann/json.hpp"
#include "ComponentManager.h";
#include "ComponentInfo.h"
#include "Preferences.h"
#include <fstream>

int PEditor::GameObject::lastId = 0;

void PEditor::GameObject::drawComponentsInEditor()
{
	for (auto it = components.begin(); it != components.end();) {
		std::string componentName = (*it).second.getName();

		if (ImGui::CollapsingHeader(componentName.c_str()))
		{
			for (auto& attribute : (*it).second.getAllAttributes()) {
				std::string attributeName = attribute.first;
				::Components::Attribute* attr = &attribute.second;

				ImGui::Text(attributeName.c_str());

				switch (attr->getType())
				{
				case ::Components::AttributesType::FLOAT:
					drawFloat(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::VECTOR2:
					drawVector2(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::STRING:
					drawString(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::BOOL:
					drawBool(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::COLOR:
					drawColor(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::CHAR:
					drawChar(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::GAMEOBJECT:
					drawGameobject(attributeName + it->first, attr);
					break;
				default:
					break;
				}
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1.0f));

			if (ImGui::Button(("Delete component##" + componentName).c_str(), ImVec2(ImGui::GetWindowSize().x, 40))) {
				it = components.erase(it);
			}
			else {
				++it;
			}

			ImGui::PopStyleColor(2);

		}
		else {
			++it;
		}
	}
}

void PEditor::GameObject::drawScriptsInEditor()
{
	for (auto it = scripts.begin(); it != scripts.end();) {
		std::string scriptName = (*it).first;
		if (ImGui::CollapsingHeader(scriptName.c_str()))
		{
			for (auto& attribute : (*it).second.getAllAttributes()) {
				std::string attributeName = attribute.first;
				::Components::Attribute* attr = &attribute.second;

				ImGui::Text(attributeName.c_str());

				switch (attr->getType())
				{
				case ::Components::AttributesType::FLOAT:
					drawFloat(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::VECTOR2:
					drawVector2(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::STRING:
					drawString(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::BOOL:
					drawBool(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::COLOR:
					drawColor(attributeName + it->first, attr);
					break;
				case ::Components::AttributesType::CHAR:
					drawChar(attributeName + it->first, attr);
				case ::Components::AttributesType::GAMEOBJECT:
					drawGameobject(attributeName + it->first, attr);
					break;
				default:
					break;
				}
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255.0f, 255.0f, 255.0f, 1.0f));


			if (ImGui::Button(("Edit script##" + scriptName).c_str(), ImVec2(ImGui::GetWindowSize().x, 40))) {

				ImGUIManager::getInstance()->OpenScript(scriptName);
			}



			if (ImGui::Button(("Delete script##" + scriptName).c_str(), ImVec2(ImGui::GetWindowSize().x, 40))) {
				it = scripts.erase(it);
			}
			else {
				++it;
			}

			ImGui::PopStyleColor(2);

		}
		else {
			++it;
		}
	}

}



void PEditor::GameObject::drawFloat(std::string attrName, ::Components::Attribute* attr)
{
	ImGui::DragFloat(("##" + attrName).c_str(), &attr->value.value.valueFloat, 0.3f, 0.0f, 0.0f, "%.2f");
}

void PEditor::GameObject::drawVector2(std::string attrName, ::Components::Attribute* attr)
{

	ImGui::DragFloat2(("##" + attrName).c_str(), (float*)&attr->value.value.valueVector2, 0.3f, 0.0f, 0.0f, "%.2f");
}

void PEditor::GameObject::drawString(std::string attrName, ::Components::Attribute* attr)
{
	char inputBuffer[256];
	strncpy_s(inputBuffer, attr->value.valueString.c_str(), sizeof(inputBuffer));

	if (ImGui::InputText(("##" + attrName).c_str(), inputBuffer, sizeof(inputBuffer))) {
		attr->value.valueString = inputBuffer;
	}
}

void PEditor::GameObject::drawBool(std::string attrName, ::Components::Attribute* attr)
{
	ImGui::Checkbox(("##" + attrName).c_str(), &attr->value.value.valueBool);
}

void PEditor::GameObject::drawColor(std::string attrName, ::Components::Attribute* attr)
{
	ImGui::ColorEdit3(("##" + attrName).c_str(), (float*)&attr->value.value.valueColor);
}

void PEditor::GameObject::drawChar(std::string attrName, ::Components::Attribute* attr)
{
	if (ImGui::InputText(("##" + attrName).c_str(), &attr->value.value.valueChar, 2, ImGuiInputTextFlags_CharsNoBlank)) {
	}
}

void PEditor::GameObject::drawGameobject(std::string attrName, ::Components::Attribute* attr)
{
	std::unordered_map<int, PEditor::GameObject*> gameObjects = imGuiManager->getScene()->getGameObjects();

	GameObject* go = gameObjects.find((int)attr->value.value.valueFloat) != gameObjects.end() ? gameObjects.find((int)attr->value.value.valueFloat)->second : nullptr;

	std::string name = "";
	if (attr->value.valueString != "") {
		name = attr->value.valueString;
	}

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


PEditor::GameObject::GameObject(std::string& path)
{
	Scene* scene = ImGUIManager::getInstance()->getScene();
	std::unordered_map<int, PEditor::GameObject*> gameObjects = scene->getGameObjects();

	//Advance id if it already exists, can happen when we load a scene BECAUSE the gameObjects saved have an id
	while (gameObjects.find(GameObject::lastId) != gameObjects.end()) {
		GameObject::lastId++;
	}

	id = GameObject::lastId;

	GameObject::lastId++;

	imagePath = path;

	parent = nullptr;

	text = nullptr;

	SDL_Surface* surface = IMG_Load(path.c_str());

	if (surface != nullptr) {
		text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);
	}
	else {
		imagePath = "";
	}

	SDL_FreeSurface(surface);

	if (imagePath != "") {
		//Add component image
		::Components::Component imageComponent = ::Components::ComponentManager::GetAllComponents().find("Image")->second;
		::Components::AttributeValue attributeValue;
		attributeValue.valueString = path;
		imageComponent.getAttribute("fileName").SetValue(attributeValue);
		addComponent(imageComponent);

		//Hacemos que el nombre inicial del gameObject sea el nombre de la imagen
		std::size_t extensionPos = path.find_last_of('.');
		name = (extensionPos != std::string::npos) ? path.substr(0, extensionPos) : path;
	}
	else {
		name = "Empty gameobject";
	}

	pos = new ImVec2(0, 0);
	size = new ImVec2(100, 100);

	imGuiManager = ImGUIManager::getInstance();

	//Gizmo texture
	surface = IMG_Load("gizmo.png");
	gizmoText = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

	SDL_FreeSurface(surface);

	showGizmo = false;
	visible = true;

	waitingToDelete = false;

	previousMousePosX = 0;
	previousMousePosY = 0;

	renderOrder = 0;
}

PEditor::GameObject::~GameObject()
{
	if (parent != nullptr) {
		parent->removeChild(this);
	}

	for (auto child : children) {
		child.second->setParent(nullptr);
	}

	components.clear();

	delete pos;
	delete size;
}

SDL_Texture* PEditor::GameObject::getTexture()
{
	return text;
}
std::string PEditor::GameObject::getName()
{
	return name;
}
bool PEditor::GameObject::isVisible()
{
	return visible;
}
int PEditor::GameObject::getId()
{
	return id;
}
int PEditor::GameObject::getRenderOrder()
{
	return renderOrder;
}
void PEditor::GameObject::setVisible(bool visible)
{
	if (this->parent == nullptr) {
		this->visible = visible;
		setChildrenVisible(this, visible);
	}
}
int PEditor::GameObject::getWidth()
{
	return size->x;
}

int PEditor::GameObject::getHeight()
{
	return size->y;
}

void PEditor::GameObject::drawTransformInEditor()
{
	if (ImGui::CollapsingHeader("Transform"))
	{

		ImGui::Text("Position");
		ImGui::DragFloat2("##position_drag", (float*)pos, 0.3f, 0.0f, 0.0f, "%.2f");

		ImGui::Text("Scale");
		ImGui::DragFloat2("##scale_drag", (float*)size, 0.1f, 0.0f, 0.0f, "%.2f");


		ImGui::Text("Rotation");
		ImGui::DragFloat("##rotation_drag", &rotation, 0.1f, 0.0f, 0.0f, "%.2f");


		ImGui::Text("Render order");
		ImGui::InputInt("##render_order", &renderOrder);

	}
}

void PEditor::GameObject::render(SDL_Renderer* renderer, Camera* camera)
{
	ImVec2 position = getPosition();

	//El punto 0, 0 es el centro del frame
	position.x += Preferences::GetData().width * 0.5f;
	position.y += Preferences::GetData().height * 0.5f;

	int tWidth = 1, tHeight = 1;
	SDL_Texture* texture = getTexture();

	if (texture != nullptr)
		SDL_QueryTexture(texture, NULL, NULL, &tWidth, &tHeight);

	float width = tWidth * getWidth();
	float height = tHeight * getHeight();

	//El origen de los objetos es el centro
	position.x -= width * 0.5f;
	position.y -= height * 0.5f;


	//Posicion y tama�os relativos al frame de la escena
	ImVec2 relativePosition = ImVec2((position.x + camera->getPosition().x) * camera->getScrollFactor(),
		(position.y + camera->getPosition().y) * camera->getScrollFactor());

	float relativeWidth = width * camera->getScrollFactor();
	float relativeHeight = height * camera->getScrollFactor();

	SDL_Rect dst = { relativePosition.x, relativePosition.y, relativeWidth, relativeHeight };

	if (visible) {
		SDL_RenderCopyEx(renderer, getTexture(), NULL, &dst, rotation, NULL, SDL_FLIP_NONE);
	}

	//Render outline
	if (this == imGuiManager->getScene()->getSelectedGameObject())
	{
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

			SDL_RenderCopy(renderer, gizmoText, NULL, &dst);
		}
	}
}

void PEditor::GameObject::translateChildren(GameObject* go, ImVec2* previousPos)
{
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

void PEditor::GameObject::scaleChildren(GameObject* go, int scaleFactor)
{
	for (auto childPair : go->getChildren()) {

		childPair.second->size->x += scaleFactor;
		childPair.second->size->y += scaleFactor;

		scaleChildren(childPair.second, scaleFactor);
	}
}

void PEditor::GameObject::setChildrenVisible(GameObject* go, bool visible)
{
	for (auto childPair : go->getChildren()) {

		childPair.second->visible = visible;

		setChildrenVisible(childPair.second, visible);
	}
}

void PEditor::GameObject::rotateChildren(GameObject* go, GameObject* goCenter, float rotationAngle)
{
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


void PEditor::GameObject::handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos)
{
	showGizmo = false;

	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_DELETE && imGuiManager->getScene()->getSelectedGameObject() == this) {
		toDelete();
	}

	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {

		if (!leftMouseButtonDown) {
			leftMouseButtonDown = true;
		}

		if (visible && isMouseInsideGameObject) {
			imGuiManager->getScene()->setSelectedGameObject(this);
		}
	}

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (leftMouseButtonDown && event->button.button == SDL_BUTTON_LEFT)
		{
			leftMouseButtonDown = false;
		}

		if (rightMouseButtonDown && event->button.button == SDL_BUTTON_RIGHT)
		{
			rightMouseButtonDown = false;
		}
	}


	if (imGuiManager->getScene()->getSelectedGameObject() == this) {
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

					pos->x = mousePos.x - size->x / 2;
					pos->y = mousePos.y - size->y / 2;

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
					size->x += 5;
					size->y += 5;

					scaleFactor = 5;

				}
				else if (event->wheel.y < 0) // scroll down
				{
					size->x -= 5;
					size->y -= 5;

					scaleFactor = -5;
				}

				scaleChildren(this, scaleFactor);
			}
		}
	}

	previousMousePosX = mousePos.x;
	previousMousePosY = mousePos.y;
}

void PEditor::GameObject::update()
{
	if (components.find("Image") == components.end()) {
		text = nullptr;
		return;
	}

	Components::Component* imageComponent = &components.find("Image")->second;
	std::string currentImagePath = imageComponent->getAttribute("fileName").value.valueString;

	if (currentImagePath != imagePath) {
		SDL_Surface* surface = IMG_Load(std::string("Images/" + currentImagePath).c_str());

		if (surface != nullptr) {
			text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);
			imagePath = currentImagePath;
		}
		else {
			text = nullptr;
			imagePath = "";
		}

		SDL_FreeSurface(surface);
	}

}

void PEditor::GameObject::addComponent(::Components::Component comp)
{
	if (components.find(comp.getName()) == components.end()) {
		components.insert({ comp.getName(), comp });
	}
}

void PEditor::GameObject::addScript(::Components::Script script)
{
	if (scripts.contains(script.GetName()))
	{
		return;
	}

	scripts.emplace(script.GetName(), script);
}

std::unordered_map<std::string, ::Components::Component>* PEditor::GameObject::getComponents()
{
	return &components;
}

std::unordered_map<std::string, ::Components::Script>* PEditor::GameObject::getScripts()
{
	return &scripts;
}

void PEditor::GameObject::setPosition(ImVec2 newPos)
{
	pos->x = newPos.x;
	pos->y = newPos.y;
}

void PEditor::GameObject::setName(const std::string newName)
{
	name = newName;
}

ImVec2 PEditor::GameObject::getPosition()
{
	return *pos;
}

float PEditor::GameObject::getRotation()
{
	return rotation;
}

bool PEditor::GameObject::isWaitingToDelete()
{
	return waitingToDelete;
}

void PEditor::GameObject::toDelete()
{
	waitingToDelete = true;

	for (auto child : children) {
		child.second->toDelete();
	}
}

void PEditor::GameObject::setParent(GameObject* go)
{
	parent = go;
}

PEditor::GameObject* PEditor::GameObject::getParent()
{
	return parent;
}

void PEditor::GameObject::removeChild(GameObject* go)
{
	go->setParent(nullptr);
	children.erase(go->getId());
}

void PEditor::GameObject::addChild(GameObject* go)
{
	go->setParent(this);
	children.insert({ go->getId(), go });
}

std::unordered_map<int, PEditor::GameObject*> PEditor::GameObject::getChildren()
{
	return children;
}

bool PEditor::GameObject::isAscendant(GameObject* go)
{
	for (auto childPair : children)
	{
		GameObject* child = childPair.second;
		if (child == go || child->isAscendant(go))
		{
			return true;
		}
	}
	return false;
}

std::string PEditor::GameObject::toJson(bool isPrefab)
{
	nlohmann::ordered_json j;
	j["name"] = name;

	nlohmann::ordered_json childsJson = nlohmann::json::array();
	for (auto childPair : children) {
		auto child = j.parse(childPair.second->toJson());

		childsJson.push_back(child);
	}

	if (isPrefab) {
		//if its a prefab it needs a new Id
		std::unordered_map<int, PEditor::GameObject*> gameObjects = ImGUIManager::getInstance()->getScene()->getGameObjects();
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
	j["localScale"] = std::to_string(size->x) + ", " + std::to_string(size->y);
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

PEditor::GameObject* PEditor::GameObject::fromJson(std::string json, bool isPrefab)
{
	nlohmann::ordered_json jsonData;
	try {
		jsonData = nlohmann::json::parse(json);
	}
	catch (const nlohmann::json::parse_error& e) {
		std::cerr << "JSON parse error: " << e.what() << std::endl;
		return nullptr;
	}

	std::string goName = jsonData["name"];

	PEditor::GameObject* gameObject = new PEditor::GameObject(goName);
	gameObject->name = goName;

	//if its prefab we leave the autoassigned id
	if (!isPrefab) {
		gameObject->id = jsonData["id"];
	}

	for (const auto& childJson : jsonData["childs"]) {
		PEditor::GameObject* child = PEditor::GameObject::fromJson(childJson.dump());

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
	gameObject->size = localScale;
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


