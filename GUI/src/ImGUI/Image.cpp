#include "Image.h"
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"



Image::Image(std::string& path, SDL_Renderer* renderer)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	text = SDL_CreateTextureFromSurface(renderer, surface);

	width = surface->w;
	height = surface->h;
}

void Image::render()
{
	ImGui::Image(text, ImVec2(width, height));
}

SDL_Texture* Image::getTexture()
{
	return text;
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}


