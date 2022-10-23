#include "Scene.h"
#include "SDL.h"
#include "Image.h"
#include "Window.h"
#include "imgui.h"

Scene::Scene (PEditor::Window* w, SDL_Renderer* r)
{
	window = w;
	window->addComponent(this);

	renderer = r;

	width = window->getWidth();
	height = window->getHeight() - 35;

	targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	
}

void Scene::addImage(std::string path)
{
	images.push_back(new Image(path, renderer));
}

void Scene::render()
{
	SDL_SetRenderTarget(renderer, targetTexture);
	SDL_RenderClear(renderer);

	int i = 0;
	for (auto image : images) {
		SDL_Rect dst = { i,i, image->getWidth(), image->getHeight() };
		SDL_RenderCopy(renderer, image->getTexture(), NULL, &dst);
		i += 50;
	}

	SDL_SetRenderTarget(renderer, NULL);

	ImGui::Image(targetTexture, ImVec2(width, height));
}

