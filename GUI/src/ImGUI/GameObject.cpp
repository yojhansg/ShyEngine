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
					drawBool(attributeName+ it->first, attr);
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

	ImGui::DragFloat2(("##" + attrName).c_str(), (float*) &attr->value.value.valueVector2, 0.3f, 0.0f, 0.0f, "%.2f");
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
	std::vector<PEditor::GameObject*> gameObjects = imGuiManager->getScene()->getGameObjects();

	std::map<int, GameObject*> gameObjectMap;
	for (GameObject* gameObject : gameObjects) {
		gameObjectMap[gameObject->getId()] = gameObject;
	}

	if(ImGui::BeginCombo(("##" + attrName).c_str(), gameObjectMap.find((int)attr->value.value.valueFloat)->second->getName().c_str())) {
		for (int i = 0; i < gameObjects.size(); i++) {
			if (ImGui::Selectable(gameObjects[i]->getName().c_str()))
				attr->value.value.valueFloat  = gameObjects[i]->getId();
		}
		ImGui::EndCombo();
	}
}

PEditor::GameObject::GameObject(std::string& path)
{
	id = GameObject::lastId;

	GameObject::lastId++;

	imagePath = path;

	prefab = false;

	text = nullptr;

	SDL_Surface* surface = IMG_Load(path.c_str());

	if (surface != nullptr) {
		text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);
	}
	else {
		imagePath = "";
	}

	SDL_FreeSurface(surface);

	if (path != "") {
		//Add component image
		::Components::Component imageComponent = ::Components::ComponentManager::GetAllComponents().find("Image")->second;
		::Components::AttributeValue attributeValue;
		attributeValue.valueString = path;
		imageComponent.getAttribute("fileName").SetValue(attributeValue);
		addComponent(imageComponent);
	}

	if (path != "") {
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
	this->visible = visible;
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
	float width = getWidth();
	float height = getHeight();


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

void PEditor::GameObject::handleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos)
{
	showGizmo = false;

	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_DELETE && imGuiManager->getScene()->getSelectedGameObject() == this) {
		toDelete();
	}

	if (isMouseInsideGameObject) {
		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {

			if (!leftMouseButtonDown) {
				leftMouseButtonDown = true;
			}

			if (visible) {
				imGuiManager->getScene()->setSelectedGameObject(this);
			}
		}
	}


	if (imGuiManager->getScene()->getSelectedGameObject() == this) {
		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_RIGHT) {

			if (!rightMouseButtonDown) {
				rightMouseButtonDown = true;
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

			if (SDL_GetModState() & KMOD_SHIFT) {
				showGizmo = true;

				if (event->type == SDL_MOUSEMOTION)
				{
					if (leftMouseButtonDown)
					{
						pos->x = mousePos.x - size->x / 2;
						pos->y = mousePos.y - size->y / 2;
					}

					if (rightMouseButtonDown)
					{
						rotation += (previousMousePosX - mousePos.x) * 0.5f;
						rotation += (previousMousePosY - mousePos.y) * 0.5f;
					}
				}

				if (event->type == SDL_MOUSEWHEEL) {
					if (event->wheel.y > 0) // scroll up
					{
						size->x += 5;
						size->y += 5;
					}
					else if (event->wheel.y < 0) // scroll down
					{
						size->x -= 5;
						size->y -= 5;
					}
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
		SDL_Surface* surface = IMG_Load(currentImagePath.c_str());

		if (surface != nullptr) {
			text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);
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
		components.emplace(comp.getName(), comp);
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
}

bool PEditor::GameObject::isPrefab()
{
	return prefab;
}

std::string PEditor::GameObject::toJson()
{
	nlohmann::ordered_json j;
	j["name"] = name;
	j["order"] = renderOrder;

	std::string posJson = std::to_string(pos->x) + ", " + std::to_string(pos->y);
	/*nlohmann::ordered_json posJson;
	posJson.push_back(pos->x);
	posJson.push_back(pos->y);*/
	j["localPosition"] = posJson;

	std::string sizeJson = std::to_string(size->x) + ", " + std::to_string(size->y);
	/*nlohmann::ordered_json sizeJson;
	sizeJson.push_back(size->x);
	sizeJson.push_back(size->y);*/
	j["localScale"] = sizeJson;

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


