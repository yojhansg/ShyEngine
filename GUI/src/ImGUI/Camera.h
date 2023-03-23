#pragma once
#include <vector>

union SDL_Event;
struct ImVec2;
class Image;

class Camera
{
private:
	float scrollFactor;
	float posX, posY;

	bool rightMouseButtonDown;
	float mousePosX, mousePosY;
	float clickOffsetX, clickOffsetY;

public:
	Camera(ImVec2 position, float scrollFactor);

	void handleInput(SDL_Event* event);

	ImVec2 getPosition();
	float getScrollFactor();
};

