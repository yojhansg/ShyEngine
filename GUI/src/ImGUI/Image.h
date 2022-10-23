#pragma once
#include "WindowComponent.h"
#include <iostream>

struct SDL_Texture;
struct SDL_Renderer;

class Image: public WindowComponent
{

	SDL_Texture* text;
	int width, height;

public:

	Image(std::string& path, SDL_Renderer* renderer);

	virtual void render();

	SDL_Texture* getTexture();

	int getWidth();
	int getHeight();
};

