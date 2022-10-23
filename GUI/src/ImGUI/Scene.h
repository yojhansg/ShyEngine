#pragma once
#include <vector>
#include "WindowComponent.h"
#include <iostream>

struct SDL_Renderer;
struct SDL_Texture;
class Image;
class string;

namespace PEditor {
	class Window;
};

class Scene : public WindowComponent
{
private:

	PEditor::Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* targetTexture = nullptr;

	std::vector<Image*> images;

	int width, height;

public:

	Scene(PEditor::Window* window, SDL_Renderer* renderer);

	void addImage(std::string path);

	virtual void render();
};

