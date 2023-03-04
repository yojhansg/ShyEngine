#include "Image.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "ImGUIManager.h"


PEditor::Image::Image(std::string& path)
{
	posX = 0;
	posY = 0;

	SDL_Surface* surface = IMG_Load(path.c_str());
	text = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

	width = surface->w;
	height = surface->h;
}

void PEditor::Image::render()
{
	ImGui::Image(text, ImVec2(width, height));
}

SDL_Texture* PEditor::Image::getTexture()
{
	return text;
}

int PEditor::Image::getWidth()
{
	return width;
}

int PEditor::Image::getHeight()
{
	return height;
}

void PEditor::Image::setPosition(ImVec2 newPos)
{
	posX = newPos.x;
	posY = newPos.y;
}

ImVec2 PEditor::Image::getPosition()
{
	return ImVec2(posX,posY);
}


