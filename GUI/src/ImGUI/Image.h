#pragma once
#include "WindowComponent.h"
#include <iostream>

struct ImVec2;
struct SDL_Texture;
struct SDL_Renderer;
class ImGUIManager;

class Image: public WindowComponent
{

	SDL_Texture* text;
	float width, height;

	float posX, posY;

public:

	Image(std::string& path);

	virtual void render();

	SDL_Texture* getTexture();

	int getWidth();
	int getHeight();

	void setPosition(ImVec2 newPos);
	ImVec2 getPosition();
};

