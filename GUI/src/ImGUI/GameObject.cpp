#include "GameObject.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "ImGUIManager.h"
#include "Camera.h"
#include "Scene.h"
#include "SDL.h"
#include "Component.h"
#include "nlohmann/json.hpp"

PEditor::GameObject::GameObject(std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

	SDL_FreeSurface(surface);

	//Temporal: hacemos que el nombre del gameObject sea el nombre de la imagen
	std::size_t extensionPos = path.find_last_of('.');
    name = (extensionPos != std::string::npos) ? path.substr(0, extensionPos) : path;

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
}

PEditor::GameObject::~GameObject()
{
	for (auto it = components.begin(); it != components.end(); ++it) {
		delete *it;
	}
	components.clear();

	delete pos;
	delete size;
}

void PEditor::GameObject::render()
{
	ImGui::Image(text, *size);
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
	}
}

void PEditor::GameObject::render(SDL_Renderer* renderer, Camera* camera)
{
	ImVec2 position = getPosition();
	float width = getWidth();
	float height = getHeight();


	//Posicion y tamaños relativos al frame de la escena
	ImVec2 relativePosition = ImVec2((position.x + camera->getPosition().x) * camera->getScrollFactor(),
		(position.y + camera->getPosition().y) * camera->getScrollFactor());

	float relativeWidth = width * camera->getScrollFactor();
	float relativeHeight = height * camera->getScrollFactor();

	SDL_Rect dst = { relativePosition.x, relativePosition.y, relativeWidth, relativeHeight };

	if (visible) {
		SDL_RenderCopy(renderer, getTexture(), NULL, &dst);
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

	if(imGuiManager->getScene()->getSelectedGameObject() == this) {
		if (event->type == SDL_MOUSEBUTTONUP)
		{
			if (leftMouseButtonDown && event->button.button == SDL_BUTTON_LEFT)
			{
				leftMouseButtonDown = false;
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
}

std::list<Component*>* PEditor::GameObject::getComponents()
{
	return &components;
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

bool PEditor::GameObject::isWaitingToDelete()
{
	return waitingToDelete;
}

void PEditor::GameObject::toDelete()
{
	waitingToDelete = true;
}

std::string PEditor::GameObject::toJson()
{
	nlohmann::ordered_json j;

	j["Name"] = name;

	nlohmann::ordered_json componentsJson;
	for (auto component : components) {
		componentsJson.push_back(nlohmann::ordered_json::parse(component->toJson()));
	}

	j["Components"] = componentsJson;

	return j.dump(2);
}


