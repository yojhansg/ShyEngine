#pragma once
#include "WindowComponent.h"
#include <vector>

struct ImVec2;
class Image;

class Camera: public WindowComponent
{
private:
	int sizeX, sizeY;
	int posX, posY;

	std::vector<Image*> images;

public:

	Camera(ImVec2 size, ImVec2 position);

	virtual void render();
};

