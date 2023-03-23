#include "GameObject.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "ImGUIManager.h"
#include "Transform.h"

PEditor::GameObject::GameObject(std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

	tr = new Transform();
	tr->setPosition(0, 0);
	tr->setSize(surface->w, surface->h);
	tr->type = transform;

	components.emplace(transform, tr);
}

void PEditor::GameObject::render()
{
	ImGui::Image(text, *tr->getSize());
}

SDL_Texture* PEditor::GameObject::getTexture()
{
	return text;
}
int PEditor::GameObject::getWidth()
{
	return tr->getSize()->x;
}

int PEditor::GameObject::getHeight()
{
	return tr->getSize()->y;
}

std::unordered_map<int, Component*>* PEditor::GameObject::getComponents()
{
	return &components;
}

void PEditor::GameObject::setPosition(ImVec2 newPos)
{
	tr->setPosition(newPos.x, newPos.y);
}

ImVec2 PEditor::GameObject::getPosition()
{
	return *tr->getPosition();
}


