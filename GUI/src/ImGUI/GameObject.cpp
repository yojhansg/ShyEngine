#include "GameObject.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "ImGUIManager.h"
#include "Transform.h"
#include "Camera.h"
#include "Scene.h"
#include "SDL.h"


PEditor::GameObject::GameObject(std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

	SDL_FreeSurface(surface);

	//Temporal: hacemos que el nombre del gameObject sea el nombre de la imagen
	std::size_t extensionPos = path.find_last_of('.');
    name = (extensionPos != std::string::npos) ? path.substr(0, extensionPos) : path;

	tr = new Transform();
	tr->setPosition(0, 0);
	tr->setSize(100, 100);
	tr->type = transform;

	components.emplace(transform, tr);

	imGuiManager = ImGUIManager::getInstance();
}

PEditor::GameObject::~GameObject()
{
	for (auto it = components.begin(); it != components.end(); ++it) {
		delete it->second;
	}
	components.clear();
}

void PEditor::GameObject::render()
{
	ImGui::Image(text, *tr->getSize());
}

SDL_Texture* PEditor::GameObject::getTexture()
{
	return text;
}
std::string PEditor::GameObject::getName()
{
	return name;
}
int PEditor::GameObject::getWidth()
{
	return tr->getSize()->x;
}

int PEditor::GameObject::getHeight()
{
	return tr->getSize()->y;
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
	SDL_RenderCopy(renderer, getTexture(), NULL, &dst);


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
	}

	//TODO: render gizmos
}

void PEditor::GameObject::handleInput(SDL_Event* event, bool isMouseInsideGameObject)
{
	if (isMouseInsideGameObject) {
		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			imGuiManager->getScene()->setSelectedGameObject(this);
		}


		if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			if (!rightMouseButtonDown && event->button.button == SDL_BUTTON_LEFT)
			{
				rightMouseButtonDown = true;

				clickOffsetX = mousePosX - getPosition().x;
				clickOffsetY = mousePosY - getPosition().y;
			}
		}
	}

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (rightMouseButtonDown && event->button.button == SDL_BUTTON_LEFT)
		{
			rightMouseButtonDown = false;
		}
	}

	if (event->type == SDL_MOUSEMOTION && (SDL_GetModState() & KMOD_SHIFT))
	{
		mousePosX = event->motion.x;
		mousePosY = event->motion.y;

		if (rightMouseButtonDown)
		{
			tr->setPosition(mousePosX - clickOffsetX, mousePosY - clickOffsetY);
		}
	}

	if (event->type == SDL_MOUSEWHEEL && (SDL_GetModState() & KMOD_SHIFT)) {
		if (event->wheel.y > 0) // scroll up
		{
			ImVec2 size = *tr->getSize();
			tr->setSize(size.x + 5, size.y + 5);
		}
		else if (event->wheel.y < 0) // scroll down
		{
			ImVec2 size = *tr->getSize();
			tr->setSize(size.x - 5, size.y - 5);
		}
	}
}

std::unordered_map<int, Component*>* PEditor::GameObject::getComponents()
{
	return &components;
}

void PEditor::GameObject::setPosition(ImVec2 newPos)
{
	tr->setPosition(newPos.x, newPos.y);
}

void PEditor::GameObject::setName(const std::string newName)
{
	name = newName;
}

ImVec2 PEditor::GameObject::getPosition()
{
	return *tr->getPosition();
}


