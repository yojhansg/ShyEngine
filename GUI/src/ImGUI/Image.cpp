#include "GameObject.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "ImGUIManager.h"


PEditor::GameObject::GameObject(std::string& path)
{
	posX = 0;
	posY = 0;

	SDL_Surface* surface = IMG_Load(path.c_str());
	text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

	width = surface->w;
	height = surface->h;
}

void PEditor::GameObject::render()
{
	ImGui::Image(text, ImVec2(width, height));
}

SDL_Texture* PEditor::GameObject::getTexture()
{
	return text;
}

int PEditor::GameObject::getWidth()
{
	return width;
}

int PEditor::GameObject::getHeight()
{
	return height;
}

void PEditor::GameObject::setPosition(ImVec2 newPos)
{
	posX = newPos.x;
	posY = newPos.y;
}

ImVec2 PEditor::GameObject::getPosition()
{
	return ImVec2(posX,posY);
}


